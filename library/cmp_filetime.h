#ifndef CMP_FILETIME_H_INCLUDED
#define CMP_FILETIME_H_INCLUDED

#include <wx/longlong.h>
#include <wx/stopwatch.h>


namespace ffs3
{
//---------------------------------------------------------------------------------------------------------------
inline
bool sameFileTime(const wxLongLong& a, const wxLongLong& b, size_t tolerance)
{
    if (a < b)
        return b <= a + tolerance;
    else
        return a <= b + tolerance;
}
//---------------------------------------------------------------------------------------------------------------

class CmpFileTime
{
public:
    CmpFileTime(size_t tolerance) :
        tolerance_(tolerance) {}

    enum Result
    {
        TIME_EQUAL,
        TIME_LEFT_NEWER,
        TIME_RIGHT_NEWER,
        TIME_LEFT_INVALID,
        TIME_RIGHT_INVALID
    };

    Result getResult(const wxLongLong& lhs, const wxLongLong& rhs) const
    {
        if (lhs == rhs)
            return TIME_EQUAL;

        //number of seconds since Jan 1st 1970 + 1 year (needn't be too precise)
        static const long oneYearFromNow = wxGetUTCTime() + 365 * 24 * 3600; //static in header: not a big deal in this case!

        //check for erroneous dates (but only if dates are not (EXACTLY) the same)
        if (lhs < 0 || lhs > oneYearFromNow) //earlier than Jan 1st 1970 or more than one year in future
            return TIME_LEFT_INVALID;

        if (rhs < 0 || rhs > oneYearFromNow)
            return TIME_RIGHT_INVALID;

        if (sameFileTime(lhs, rhs, tolerance_)) //last write time may differ by up to 2 seconds (NTFS vs FAT32)
            return TIME_EQUAL;

        //regular time comparison
        if (lhs < rhs)
            return TIME_RIGHT_NEWER;
        else
            return TIME_LEFT_NEWER;
    }

private:
    const size_t tolerance_;
};
}

#endif // CMP_FILETIME_H_INCLUDED