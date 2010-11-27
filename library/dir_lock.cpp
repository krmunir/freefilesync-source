#include "dir_lock.h"
#include <wx/intl.h>
#include "../shared/string_conv.h"
#include "../shared/system_func.h"
#include <wx/utils.h>
#include <wx/timer.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "../shared/boost_thread_wrap.h" //include <boost/thread.hpp>
#include "../shared/loki/ScopeGuard.h"
#include <wx/msgdlg.h>
#include "../shared/system_constants.h"
#include "../shared/guid.h"
#include "../shared/file_io.h"
#include "../shared/assert_static.h"
#include <utility>
#include "../shared/serialize.h"
#include <boost/cstdint.hpp>
#include "../shared/build_info.h"

#ifdef FFS_WIN
#include <tlhelp32.h>
#include <wx/msw/wrapwin.h> //includes "windows.h"
#include "../shared/long_path_prefix.h"

#elif defined FFS_LINUX
#include "../shared/file_handling.h"
#include <sys/stat.h>
#include <cerrno>
#include <unistd.h>
#endif

using namespace ffs3;
using namespace std::rel_ops;


namespace
{
const size_t EMIT_LIFE_SIGN_INTERVAL =  5000; //show life sign;        unit [ms]
const size_t POLL_LIFE_SIGN_INTERVAL =  6000; //poll for life sign;    unit [ms]
const size_t DETECT_EXITUS_INTERVAL  = 30000; //assume abandoned lock; unit [ms]

typedef Zbase<Zchar, StorageDeepCopy> BasicString; //thread safe string class
}

class LifeSigns
{
public:
    LifeSigns(const BasicString& lockfilename) : //throw()!!! siehe SharedDirLock()
        lockfilename_(lockfilename) //thread safety: make deep copy!
    {
        threadObj = boost::thread(boost::cref(*this)); //localize all thread logic to this class!
    }

    ~LifeSigns()
    {
        threadObj.interrupt(); //thread lifetime is subset of this instances's life
        threadObj.join();
    }

    void operator()() const //thread entry
    {
        try
        {
            while (true)
            {
                boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(EMIT_LIFE_SIGN_INTERVAL)); //interruption point!

                //actual work
                emitLifeSign(); //throw ()
            }
        }
        catch (const std::exception& e) //exceptions must be catched per thread
        {
            wxMessageBox(wxString::FromAscii(e.what()), wxString(_("An exception occurred!")) + wxT("(Dirlock)"), wxOK | wxICON_ERROR);
        }
    }

    void emitLifeSign() const //try to append one byte...; throw()
    {
        const char buffer[1] = {' '};

#ifdef FFS_WIN
        const HANDLE fileHandle = ::CreateFile(applyLongPathPrefix(lockfilename_.c_str()).c_str(),
                                               FILE_APPEND_DATA,
                                               FILE_SHARE_READ,
                                               NULL,
                                               OPEN_EXISTING,
                                               FILE_ATTRIBUTE_NORMAL,
                                               NULL);
        if (fileHandle == INVALID_HANDLE_VALUE)
            return;

        Loki::ScopeGuard dummy = Loki::MakeGuard(::CloseHandle, fileHandle);
        (void)dummy; //silence warning "unused variable"

        const DWORD fpLow = ::SetFilePointer(fileHandle, 0, NULL, FILE_END);
        if (fpLow == INVALID_SET_FILE_POINTER)
            return;

        DWORD bytesWritten = 0;
        ::WriteFile(
            fileHandle,    //__in         HANDLE hFile,
            buffer,        //__out        LPVOID lpBuffer,
            1,             //__in         DWORD nNumberOfBytesToRead,
            &bytesWritten, //__out_opt    LPDWORD lpNumberOfBytesWritten,
            NULL);         //__inout_opt  LPOVERLAPPED lpOverlapped

#elif defined FFS_LINUX
        const int fileHandle = ::open(lockfilename_.c_str(), O_WRONLY | O_APPEND); //O_EXCL contains a race condition on NFS file systems: http://linux.die.net/man/2/open
        if (fileHandle == -1)
            return;

        Loki::ScopeGuard dummy = Loki::MakeGuard(::close, fileHandle);
        (void)dummy; //silence warning "unused variable"

        const ssize_t bytesWritten = ::write(fileHandle, buffer, 1);
        (void)bytesWritten;
#endif
    }

private:
    LifeSigns(const LifeSigns&);            //just be sure this ref-counting Zstring doesn't bite
    LifeSigns& operator=(const LifeSigns&); //

    boost::thread threadObj;
    const BasicString lockfilename_; //used by worker thread only! Not ref-counted!
};


namespace
{
bool somethingExists(const Zstring& objname) //throw()       check whether any object with this name exists
{
#ifdef FFS_WIN
    return ::GetFileAttributes(applyLongPathPrefix(objname).c_str()) != INVALID_FILE_ATTRIBUTES;

#elif defined FFS_LINUX
    struct stat fileInfo;
    return ::lstat(objname.c_str(), &fileInfo) == 0;
#endif
}


void deleteLockFile(const Zstring& filename) //throw (FileError)
{
#ifdef FFS_WIN
    if (!::DeleteFile(applyLongPathPrefix(filename).c_str()))
#elif defined FFS_LINUX
    if (::unlink(filename.c_str()) != 0)
#endif
    {
        wxString errorMessage = wxString(_("Error deleting file:")) + wxT("\n\"") + zToWx(filename) + wxT("\"");
        throw FileError(errorMessage + wxT("\n\n") + getLastErrorFormatted());
    }
}


wxULongLong getLockFileSize(const Zstring& filename) //throw (FileError, ErrorNotExisting)
{
#ifdef FFS_WIN
    WIN32_FIND_DATA fileMetaData;
    const HANDLE searchHandle = ::FindFirstFile(applyLongPathPrefix(filename).c_str(), &fileMetaData);
    if (searchHandle == INVALID_HANDLE_VALUE)
    {
        const DWORD lastError = ::GetLastError();
        const wxString errorMessage = wxString(_("Error reading file attributes:")) + wxT("\n\"") + zToWx(filename) + wxT("\"")  +
                                      wxT("\n\n") + getLastErrorFormatted(lastError);
        if (    lastError == ERROR_FILE_NOT_FOUND ||
                lastError == ERROR_PATH_NOT_FOUND)
            throw ErrorNotExisting(errorMessage);
        else
            throw FileError(errorMessage);
    }

    ::FindClose(searchHandle);

    return wxULongLong(fileMetaData.nFileSizeHigh, fileMetaData.nFileSizeLow);

#elif defined FFS_LINUX
    struct stat fileInfo;
    if (::stat(filename.c_str(), &fileInfo) != 0) //follow symbolic links
    {
        const int lastError = errno;
        const wxString errorMessage = wxString(_("Error reading file attributes:")) + wxT("\n\"") + zToWx(filename) + wxT("\"") +
                                      wxT("\n\n") + getLastErrorFormatted(lastError);
        if (lastError == ENOENT)
            throw ErrorNotExisting(errorMessage);
        else
            throw FileError(errorMessage);
    }

    return fileInfo.st_size;
#endif
}


Zstring deleteAbandonedLockName(const Zstring& lockfilename)
{
    const size_t pos = lockfilename.rfind(common::FILE_NAME_SEPARATOR); //search from end
    return pos == Zstring::npos ? Zstr("Del.") + lockfilename :
           Zstring(lockfilename.c_str(), pos + 1) + //include path separator
           Zstr("Del.") +
           lockfilename.AfterLast(common::FILE_NAME_SEPARATOR); //returns the whole string if ch not found
}


namespace
{
inline
std::string readString(wxInputStream& stream)  //read string from file stream
{
    const boost::uint32_t strLength = util::readNumber<boost::uint32_t>(stream);
    std::string output;
	if (strLength > 0)
	{
    output.resize(strLength);
    stream.Read(&output[0], strLength);
	}
    return output;
}


inline
void writeString(wxOutputStream& stream, const std::string& str)  //write string to filestream
{ 
	const boost::uint32_t strLength = static_cast<boost::uint32_t>(str.length());
    util::writeNumber<boost::uint32_t>(stream, strLength);
    stream.Write(str.c_str(), strLength);
}


std::string getComputerId() //returns empty string on error
{
    const wxString fhn = ::wxGetFullHostName();
    if (fhn.empty()) return std::string();
#ifdef FFS_WIN
    return "Windows " + std::string(fhn.ToUTF8());
#elif defined FFS_LINUX
    return "Linux " + std::string(fhn.ToUTF8());
#endif
}


struct LockInformation
{
    LockInformation()
    {
#ifdef FFS_WIN
        procDescr.processId = ::GetCurrentProcessId();
#elif defined FFS_LINUX
        procDescr.processId = ::getpid();
#endif
        procDescr.computerId = getComputerId();
    }

    LockInformation(wxInputStream& stream) : //read
        lockId(util::UniqueId(stream))
    {
        procDescr.processId  = static_cast<ProcessId>(util::readNumber<boost::uint64_t>(stream)); //possible loss of precision (32/64 bit process) covered by buildId
        procDescr.computerId = readString(stream);
    }

    void toStream(wxOutputStream& stream) const //write
    {
        assert_static(sizeof(ProcessId) <= sizeof(boost::uint64_t)); //ensure portability

        lockId.toStream(stream);
        util::writeNumber<boost::uint64_t>(stream, procDescr.processId);
        writeString(stream, procDescr.computerId);
    }

#ifdef FFS_WIN
    typedef DWORD ProcessId; //same size on 32 and 64 bit windows!
#elif defined FFS_LINUX
    typedef pid_t ProcessId;
#endif

    util::UniqueId lockId; //16 byte portable construct

    struct ProcessDescription
    {
        ProcessId processId;
        std::string computerId;
    } procDescr;
};


//true: process not available, false: cannot say anything
enum ProcessStatus
{
    PROC_STATUS_NOT_RUNNING,
    PROC_STATUS_RUNNING,
    PROC_STATUS_NO_IDEA
};
ProcessStatus getProcessStatus(const LockInformation::ProcessDescription& procDescr)
{
    if (    procDescr.computerId != getComputerId() ||
            procDescr.computerId.empty()) //both names are empty
        return PROC_STATUS_NO_IDEA; //lock owned by different computer

#ifdef FFS_WIN
    //note: ::OpenProcess() is no option as it may successfully return for crashed processes!
    HANDLE snapshot = ::CreateToolhelp32Snapshot(
                          TH32CS_SNAPPROCESS, //__in  DWORD dwFlags,
                          0);                 //__in  DWORD th32ProcessID
    if (snapshot == INVALID_HANDLE_VALUE)
        return PROC_STATUS_NO_IDEA;
    boost::shared_ptr<void> dummy(snapshot, ::CloseHandle);

    PROCESSENTRY32 processEntry = {};
    processEntry.dwSize = sizeof(processEntry);

    if (!::Process32First(snapshot,       //__in     HANDLE hSnapshot,
                          &processEntry)) //__inout  LPPROCESSENTRY32 lppe
        return PROC_STATUS_NO_IDEA;

    do
    {
        if (processEntry.th32ProcessID == procDescr.processId)
            return PROC_STATUS_RUNNING; //process still running
    }
    while(::Process32Next(snapshot, &processEntry));

    return PROC_STATUS_NOT_RUNNING;

#elif defined FFS_LINUX
    if (procDescr.processId <= 0 || procDescr.processId >= 65536)
        return PROC_STATUS_NO_IDEA; //invalid process id

    return ffs3::dirExists(Zstr("/proc/") + Zstring::fromNumber(procDescr.processId)) ? PROC_STATUS_RUNNING : PROC_STATUS_NOT_RUNNING;
#endif
}


void writeLockInfo(const Zstring& lockfilename) //throw (FileError)
{
    //write GUID at the beginning of the file: this ID is a universal identifier for this lock (no matter what the path is, considering symlinks ,etc.)
    FileOutputStream lockFile(lockfilename); //throw FileError()
    LockInformation().toStream(lockFile);
}


LockInformation retrieveLockInfo(const Zstring& lockfilename) //throw (FileError, ErrorNotExisting)
{
    //read GUID from beginning of file
    FileInputStream lockFile(lockfilename); //throw (FileError, ErrorNotExisting)
    return LockInformation(lockFile);
}


util::UniqueId retrieveLockId(const Zstring& lockfilename) //throw (FileError, ErrorNotExisting)
{
    return retrieveLockInfo(lockfilename).lockId;
}
}


void waitOnDirLock(const Zstring& lockfilename, DirLockCallback* callback) //throw (FileError)
{
    Zstring infoMsg;
    infoMsg = wxToZ(_("Waiting while directory is locked (%x)..."));
    infoMsg.Replace(Zstr("%x"), Zstr("\"") + lockfilename + Zstr("\""));
    if (callback) callback->reportInfo(infoMsg);
    //---------------------------------------------------------------
    try
    {
        const LockInformation lockInfo = retrieveLockInfo(lockfilename); //throw (FileError, ErrorNotExisting)
        const bool lockOwnderDead = getProcessStatus(lockInfo.procDescr) == PROC_STATUS_NOT_RUNNING; //convenience optimization: if we know the owning process crashed, we needn't wait DETECT_EXITUS_INTERVAL sec

        wxULongLong fileSizeOld;
        wxLongLong lockSilentStart = wxGetLocalTimeMillis();

        while (true)
        {
            const wxULongLong fileSizeNew = ::getLockFileSize(lockfilename); //throw (FileError, ErrorNotExisting)
            const wxLongLong  currentTime  = wxGetLocalTimeMillis();

            if (fileSizeNew != fileSizeOld)
            {
                //received life sign from lock
                fileSizeOld     = fileSizeNew;
                lockSilentStart = currentTime;
            }

            if (    lockOwnderDead || //no need to wait any longer...
                    currentTime - lockSilentStart > DETECT_EXITUS_INTERVAL)
            {
                DirLock dummy(deleteAbandonedLockName(lockfilename), callback); //throw (FileError)

                //now that the lock is in place check existence again: meanwhile another process may have deleted and created a new lock!

                if (retrieveLockId(lockfilename) != lockInfo.lockId) //throw (FileError, ErrorNotExisting)
                    return; //another process has placed a new lock, leave scope: the wait for the old lock is technically over...

                if (getLockFileSize(lockfilename) != fileSizeOld) //throw (FileError, ErrorNotExisting)
                    continue; //belated lifesign

                ::deleteLockFile(lockfilename); //throw (FileError)
                return;
            }

            //wait some time...
            const size_t GUI_CALLBACK_INTERVAL = 100;
            for (size_t i = 0; i < POLL_LIFE_SIGN_INTERVAL / GUI_CALLBACK_INTERVAL; ++i)
            {
                if (callback) callback->requestUiRefresh();
                wxMilliSleep(GUI_CALLBACK_INTERVAL);

                //show some countdown on abandoned locks
                if (callback)
                {
                    if (currentTime - lockSilentStart > EMIT_LIFE_SIGN_INTERVAL) //one signal missed: it's likely this is an abandoned lock:
                    {
                        long remainingSeconds = ((DETECT_EXITUS_INTERVAL - (wxGetLocalTimeMillis() - lockSilentStart)) / 1000).GetLo();
                        remainingSeconds =  std::max(0L, remainingSeconds);

                        Zstring remSecMsg = wxToZ(_("%x sec"));
                        remSecMsg.Replace(Zstr("%x"), Zstring::fromNumber(remainingSeconds));
                        callback->reportInfo(infoMsg + Zstr(" ") + remSecMsg);
                    }
                    else
                        callback->reportInfo(infoMsg); //emit a message in any case (might clear other one)
                }
            }
        }

    }
    catch (const ErrorNotExisting&)
    {
        return; //what we are waiting for...
    }
}


void releaseLock(const Zstring& lockfilename) //throw ()
{
    try
    {
        ::deleteLockFile(lockfilename);
    }
    catch(...) {}
}


bool tryLock(const Zstring& lockfilename) //throw (FileError)
{
#ifdef FFS_WIN
    const HANDLE fileHandle = ::CreateFile(applyLongPathPrefix(lockfilename).c_str(),
                                           GENERIC_WRITE,
                                           FILE_SHARE_READ,
                                           NULL,
                                           CREATE_NEW,
                                           FILE_ATTRIBUTE_NORMAL,
                                           NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        if (::GetLastError() == ERROR_FILE_EXISTS)
            return false;
        else
            throw FileError(wxString(_("Error setting directory lock:")) + wxT("\n\"") + zToWx(lockfilename) + wxT("\"") +
                            wxT("\n\n") + getLastErrorFormatted());
    }
    ::CloseHandle(fileHandle);

#elif defined FFS_LINUX
    //O_EXCL contains a race condition on NFS file systems: http://linux.die.net/man/2/open
    ::umask(0); //important!
    const int fileHandle = ::open(lockfilename.c_str(), O_CREAT | O_WRONLY | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fileHandle == -1)
    {
        if (errno == EEXIST)
            return false;
        else
            throw FileError(wxString(_("Error setting directory lock:")) + wxT("\n\"") + zToWx(lockfilename) + wxT("\"") +
                            wxT("\n\n") + getLastErrorFormatted());
    }
    ::close(fileHandle);
#endif

    Loki::ScopeGuard guardLockFile = Loki::MakeGuard(::releaseLock, lockfilename);

    //write UUID at the beginning of the file: this ID is a universal identifier for this lock (no matter what the path is, considering symlinks ,etc.)
    writeLockInfo(lockfilename); //throw (FileError)

    guardLockFile.Dismiss(); //lockfile created successfully
    return true;
}
}


class DirLock::SharedDirLock
{
public:
    SharedDirLock(const Zstring& lockfilename, DirLockCallback* callback = NULL) : //throw (FileError)
        lockfilename_(lockfilename)
    {
        while (!::tryLock(lockfilename))             //throw (FileError)
            ::waitOnDirLock(lockfilename, callback); //

        emitLifeSigns.reset(new LifeSigns(lockfilename.c_str())); //throw()! ownership of lockfile not yet managed!
    }

    ~SharedDirLock()
    {
        emitLifeSigns.reset();

        ::releaseLock(lockfilename_); //throw ()
    }

private:
    SharedDirLock(const DirLock&);
    SharedDirLock& operator=(const DirLock&);

    const Zstring lockfilename_;

    std::auto_ptr<LifeSigns> emitLifeSigns;
};


class DirLock::LockAdmin //administrate all locks held by this process to avoid deadlock by recursion
{
public:
    static LockAdmin& instance()
    {
        static LockAdmin inst;
        return inst;
    }

    //create or retrieve a SharedDirLock
    boost::shared_ptr<SharedDirLock> retrieve(const Zstring& lockfilename, DirLockCallback* callback) //throw (FileError)
    {
        //optimization: check if there is an active(!) lock for "lockfilename"
        FileToUuidMap::const_iterator iterUuid = fileToUuid.find(lockfilename);
        if (iterUuid != fileToUuid.end())
        {
            const boost::shared_ptr<SharedDirLock>& activeLock = findActive(iterUuid->second); //returns null-lock if not found
            if (activeLock)
                return activeLock; //SharedDirLock is still active -> enlarge circle of shared ownership
        }

        try //actual check based on lock UUID, deadlock prevention: "lockfilename" may be an alternative name for an already active lock
        {
            const util::UniqueId lockId = retrieveLockId(lockfilename); //throw (FileError, ErrorNotExisting)

            const boost::shared_ptr<SharedDirLock>& activeLock = findActive(lockId); //returns null-lock if not found
            if (activeLock)
            {
                fileToUuid[lockfilename] = lockId; //perf-optimization: update relation
                return activeLock;
            }
        }
        catch (const ErrorNotExisting&) {} //let other FileError(s) propagate!

        //not yet in buffer, so create a new directory lock
        boost::shared_ptr<SharedDirLock> newLock(new SharedDirLock(lockfilename, callback)); //throw (FileError)
        const util::UniqueId newLockId = retrieveLockId(lockfilename); //throw (FileError, ErrorNotExisting)

        //update registry
        fileToUuid[lockfilename] = newLockId; //throw()
        uuidToLock[newLockId]    = newLock;   //

        return newLock;
    }

private:
    LockAdmin() {}

    boost::shared_ptr<SharedDirLock> findActive(const util::UniqueId& lockId) //returns null-lock if not found
    {
        UuidToLockMap::const_iterator iterLock = uuidToLock.find(lockId);
        return iterLock != uuidToLock.end() ?
               iterLock->second.lock() : //try to get shared_ptr; throw()
               boost::shared_ptr<SharedDirLock>();
    }

    typedef boost::weak_ptr<SharedDirLock> SharedLock;

    typedef std::map<Zstring, util::UniqueId, LessFilename> FileToUuidMap; //n:1 handle uppper/lower case correctly
    typedef std::map<util::UniqueId, SharedLock>            UuidToLockMap; //1:1

    FileToUuidMap fileToUuid; //lockname |-> UUID; locks can be referenced by a lockfilename or alternatively a UUID
    UuidToLockMap uuidToLock; //UUID |-> "shared lock ownership"
};


DirLock::DirLock(const Zstring& lockfilename, DirLockCallback* callback) : //throw (FileError)
    sharedLock(LockAdmin::instance().retrieve(lockfilename, callback)) {}