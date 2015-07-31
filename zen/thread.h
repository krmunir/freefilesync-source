// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl-3.0        *
// * Copyright (C) Zenju (zenju AT gmx DOT de) - All Rights Reserved        *
// **************************************************************************

#ifndef STD_THREAD_WRAP_H_7896323423432
#define STD_THREAD_WRAP_H_7896323423432

#include <thread>
#include <future>
#include <zen/scope_guard.h>
#include <zen/type_traits.h>

namespace zen
{
class InterruptionStatus;


class InterruptibleThread
{
public:
    InterruptibleThread() {}
    InterruptibleThread           (InterruptibleThread&& tmp) = default;
    InterruptibleThread& operator=(InterruptibleThread&& tmp) = default;

    template <class Function>
    InterruptibleThread(Function f);

    bool joinable () const { return stdThread.joinable(); }
    void interrupt();
    void join     () { stdThread.join(); }
    void detach   () { stdThread.detach(); }

    template <class Rep, class Period>
    bool tryJoinFor(const std::chrono::duration<Rep, Period>& relTime)
    {
        if (threadCompleted.wait_for(relTime) == std::future_status::ready)
        {
            stdThread.join(); //runs thread-local destructors => this better be fast!!!
            return true;
        }
        return false;
    }

private:
    std::thread stdThread;
    std::shared_ptr<InterruptionStatus> intStatus_;
    std::future<void> threadCompleted;
};

//context of worker thread:
void interruptionPoint(); //throw ThreadInterruption

template<class Predicate>
void interruptibleWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lock, Predicate pred); //throw ThreadInterruption

template <class Rep, class Period>
void interruptibleSleep(const std::chrono::duration<Rep, Period>& relTime); //throw ThreadInterruption

//------------------------------------------------------------------------------------------

/*
std::async replacement without crappy semantics:
	1. guaranteed to run asynchronously
	2. does not follow C++11 [futures.async], Paragraph 5, where std::future waits for thread in destructor

Example:
        Zstring dirpath = ...
        auto ft = zen::runAsync([=](){ return zen::dirExists(dirpath); });
        if (ft.wait_for(std::chrono::milliseconds(200)) == std::future_status::ready && ft.get())
            //dir exising
*/
template <class Function>
auto runAsync(Function fun) -> std::future<decltype(fun())>;

//wait for all with a time limit: return true if *all* results are available!
template<class InputIterator, class Duration>
bool wait_for_all_timed(InputIterator first, InputIterator last, const Duration& wait_duration);

template<typename T> inline
bool isReady(const std::future<T>& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }
//------------------------------------------------------------------------------------------

//wait until first job is successful or all failed: substitute until std::when_any is available
template <class T>
class GetFirstResult
{
public:
    GetFirstResult();

    template <class Fun>
    void addJob(Fun f); //f must return a std::unique_ptr<T> containing a value if successful

    template <class Duration>
    bool timedWait(const Duration& duration) const; //true: "get()" is ready, false: time elapsed

    //return first value or none if all jobs failed; blocks until result is ready!
    std::unique_ptr<T> get() const; //may be called only once!

private:
    class AsyncResult;
    std::shared_ptr<AsyncResult> asyncResult_;
    size_t jobsTotal_;
};

//------------------------------------------------------------------------------------------

//value associated with mutex and guaranteed protected access:
template <class T>
class Protected
{
public:
    Protected()               : value_() {}
    Protected(const T& value) : value_(value) {}

    template <class Function>
    void access(Function fun)
    {
        std::lock_guard<std::mutex> dummy(lockValue);
        fun(value_);
    }

private:
    Protected           (const Protected&) = delete;
    Protected& operator=(const Protected&) = delete;

    std::mutex lockValue;
    T value_;
};









//###################### implementation ######################

template <class Function> inline
auto runAsync(Function fun) -> std::future<decltype(fun())>
{
    typedef decltype(fun()) ResultType;

    std::packaged_task<ResultType()> pt(std::move(fun));
    auto fut = pt.get_future();
    std::thread(std::move(pt)).detach(); //we have to explicitly detach since C++11: [thread.thread.destr] ~thread() calls std::terminate() if joinable()!!!
    return fut;
}


template<class InputIterator, class Duration> inline
bool wait_for_all_timed(InputIterator first, InputIterator last, const Duration& duration)
{
    const std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now() + duration;
    for (; first != last; ++first)
        if (first->wait_until(endTime) != std::future_status::ready)
            return false; //time elapsed
    return true;
}


template <class T>
class GetFirstResult<T>::AsyncResult
{
public:
    AsyncResult() :
#ifndef NDEBUG
        returnedResult(false),
#endif
        jobsFinished(0) {}

    //context: worker threads
    void reportFinished(std::unique_ptr<T>&& result)
    {
        {
            std::lock_guard<std::mutex> dummy(lockResult);
            ++jobsFinished;
            if (!result_)
                result_ = std::move(result);
        }
        conditionJobDone.notify_all(); //better notify all, considering bugs like: https://svn.boost.org/trac/boost/ticket/7796
    }

    //context: main thread
    template <class Duration>
    bool waitForResult(size_t jobsTotal, const Duration& duration)
    {
        std::unique_lock<std::mutex> dummy(lockResult);
        return conditionJobDone.wait_for(dummy, duration, [&] { return this->jobDone(jobsTotal); });
    }

    std::unique_ptr<T> getResult(size_t jobsTotal)
    {
        std::unique_lock<std::mutex> dummy(lockResult);
        conditionJobDone.wait(dummy, [&] { return this->jobDone(jobsTotal); });

#ifndef NDEBUG
        assert(!returnedResult);
        returnedResult = true;
#endif
        return std::move(result_);
    }

private:
    bool jobDone(size_t jobsTotal) const { return result_ || (jobsFinished >= jobsTotal); } //call while locked!

#ifndef NDEBUG
    bool returnedResult;
#endif

    std::mutex lockResult;
    size_t jobsFinished;        //
    std::unique_ptr<T> result_; //our condition is: "have result" or "jobsFinished == jobsTotal"
    std::condition_variable conditionJobDone;
};



template <class T> inline
GetFirstResult<T>::GetFirstResult() : asyncResult_(std::make_shared<AsyncResult>()), jobsTotal_(0) {}


template <class T>
template <class Fun> inline
void GetFirstResult<T>::addJob(Fun f) //f must return a std::unique_ptr<T> containing a value on success
{
    std::thread t([asyncResult = this->asyncResult_, f = std::move(f)] { asyncResult->reportFinished(f()); });
    ++jobsTotal_;
    t.detach(); //we have to be explicit since C++11: [thread.thread.destr] ~thread() calls std::terminate() if joinable()!!!
}


template <class T>
template <class Duration> inline
bool GetFirstResult<T>::timedWait(const Duration& duration) const { return asyncResult_->waitForResult(jobsTotal_, duration); }


template <class T> inline
std::unique_ptr<T> GetFirstResult<T>::get() const { return asyncResult_->getResult(jobsTotal_); }

//------------------------------------------------------------------------------------------

//thread_local with non-POD seems to cause memory leaks on VS 14 => pointer only is fine:
#ifdef _MSC_VER
    #define ZEN_THREAD_LOCAL_SPECIFIER __declspec(thread)
#elif defined __GNUC__ || defined __clang__
    #define ZEN_THREAD_LOCAL_SPECIFIER __thread
#else
    #error "game over"
#endif


class ThreadInterruption {};


class InterruptionStatus
{
public:
    //context of InterruptibleThread instance:
    void interrupt()
    {
        interrupted = true;

        {
            std::lock_guard<std::mutex> dummy(lockSleep); //needed! makes sure the following signal is not lost!
            //usually we'd make "interrupted" non-atomic, but this is already given due to interruptibleWait() handling
        }
        conditionSleepInterruption.notify_all();

        std::lock_guard<std::mutex> dummy(lockConditionPtr);
        if (activeCondition)
            activeCondition->notify_all(); //signal may get lost!
        //alternative design locking the cv's mutex here could be dangerous: potential for dead lock!
    }

    //context of worker thread:
    void checkInterruption() //throw ThreadInterruption
    {
        if (interrupted)
            throw ThreadInterruption();
    }

    //context of worker thread:
    template<class Predicate>
    void interruptibleWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lock, Predicate pred) //throw ThreadInterruption
    {
        setConditionVar(&cv);
        ZEN_ON_SCOPE_EXIT(setConditionVar(nullptr));

        //"interrupted" is not protected by cv's mutex => signal may get lost!!! => add artifical time out to mitigate! CPU: 0.25% vs 0% for longer time out!
        while (!cv.wait_for(lock, std::chrono::milliseconds(1), [&] { return this->interrupted || pred(); }))
            ;

        checkInterruption(); //throw ThreadInterruption
    }

    //context of worker thread:
    template <class Rep, class Period>
    void interruptibleSleep(const std::chrono::duration<Rep, Period>& relTime) //throw ThreadInterruption
    {
        std::unique_lock<std::mutex> lock(lockSleep);
        if (conditionSleepInterruption.wait_for(lock, relTime, [&] { return static_cast<bool>(this->interrupted); }))
            throw ThreadInterruption();
    }

private:
    void setConditionVar(std::condition_variable* cv)
    {
        std::lock_guard<std::mutex> dummy(lockConditionPtr);
        activeCondition = cv;
    }

    std::atomic<bool> interrupted{ false }; //std:atomic is uninitialized by default!

    std::condition_variable* activeCondition = nullptr;
    std::mutex lockConditionPtr; //serialize pointer access (only!)

    std::condition_variable conditionSleepInterruption;
    std::mutex lockSleep;
};


namespace impl
{
inline
InterruptionStatus*& refThreadLocalInterruptionStatus()
{
    static ZEN_THREAD_LOCAL_SPECIFIER InterruptionStatus* threadLocalInterruptionStatus = nullptr;
    return threadLocalInterruptionStatus;
}
}

//context of worker thread:
inline
void interruptionPoint() //throw ThreadInterruption
{
    assert(impl::refThreadLocalInterruptionStatus());
    if (impl::refThreadLocalInterruptionStatus())
        impl::refThreadLocalInterruptionStatus()->checkInterruption(); //throw ThreadInterruption
}


//context of worker thread:
template<class Predicate> inline
void interruptibleWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lock, Predicate pred) //throw ThreadInterruption
{
    assert(impl::refThreadLocalInterruptionStatus());
    if (impl::refThreadLocalInterruptionStatus())
        impl::refThreadLocalInterruptionStatus()->interruptibleWait(cv, lock, pred);
    else
        cv.wait(lock, pred);
}

//context of worker thread:
template <class Rep, class Period> inline
void interruptibleSleep(const std::chrono::duration<Rep, Period>& relTime) //throw ThreadInterruption
{
    assert(impl::refThreadLocalInterruptionStatus());
    if (impl::refThreadLocalInterruptionStatus())
        impl::refThreadLocalInterruptionStatus()->interruptibleSleep(relTime);
    else
        std::this_thread::sleep_for(relTime);
}


template <class Function> inline
InterruptibleThread::InterruptibleThread(Function f) : intStatus_(std::make_shared<InterruptionStatus>())
{
    std::promise<void> pFinished;
    threadCompleted = pFinished.get_future();

    stdThread = std::thread([f = std::move(f),
                             intStatus = this->intStatus_,
                             pFinished = std::move(pFinished)]() mutable
    {
        assert(!impl::refThreadLocalInterruptionStatus());
        impl::refThreadLocalInterruptionStatus() = intStatus.get();
        ZEN_ON_SCOPE_EXIT(impl::refThreadLocalInterruptionStatus() = nullptr);
        ZEN_ON_SCOPE_EXIT(pFinished.set_value());

        try
        {
            f(); //throw ThreadInterruption
        }
        catch (ThreadInterruption&) {}
    });
}


inline
void InterruptibleThread::interrupt() { intStatus_->interrupt(); }
}

#endif //STD_THREAD_WRAP_H_7896323423432
