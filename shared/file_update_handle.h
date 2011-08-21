#ifndef FILE_UPDATE_HANDLE_H_INCLUDED
#define FILE_UPDATE_HANDLE_H_INCLUDED

#include <wx/msw/wrapwin.h> //includes "windows.h"
#include "long_path_prefix.h"

namespace
{
//manage file handle to update existing files (temporarily resetting read-only if necessary)
//CreateFileCmd: lambda directly returning non-owned file handle from ::CreateFile()
class FileUpdateHandle
{
public:
    template <class CreateFileCmd>
    FileUpdateHandle(const Zstring& filename, CreateFileCmd cmd) :
        filenameFmt(zen::applyLongPathPrefix(filename)),
        hFile(INVALID_HANDLE_VALUE),
        attr(INVALID_FILE_ATTRIBUTES)
    {
        hFile = cmd();
        if (hFile != INVALID_HANDLE_VALUE)
            return;

        const DWORD lastError = ::GetLastError();
        if (lastError == ERROR_ACCESS_DENIED) //function fails if file is read-only
        {
            Loki::ScopeGuard guardErrorCode = Loki::MakeGuard(::SetLastError, lastError); //transactional behavior: ensure cleanup (e.g. network drop) -> cref [!]

            //read-only file attribute may cause trouble: temporarily reset it
            const DWORD tmpAttr = ::GetFileAttributes(filenameFmt.c_str());
            if (tmpAttr != INVALID_FILE_ATTRIBUTES && (tmpAttr & FILE_ATTRIBUTE_READONLY))
            {
                if (::SetFileAttributes(filenameFmt.c_str(), FILE_ATTRIBUTE_NORMAL))
                {
                    guardErrorCode.Dismiss();
                    attr = tmpAttr; //"create" guard on read-only attribute

                    //now try again
                    hFile = cmd();
                }
            }
        }
    }

    ~FileUpdateHandle()
    {
        if (hFile != INVALID_HANDLE_VALUE)
            ::CloseHandle(hFile);

        if (attr != INVALID_FILE_ATTRIBUTES)
            ::SetFileAttributes(filenameFmt.c_str(), attr);
    }

    //may return INVALID_FILE_ATTRIBUTES, in which case ::GetLastError() may be called directly after FileUpdateHandle()
    HANDLE get() const { return hFile; }

private:
    FileUpdateHandle(const FileUpdateHandle&);
    FileUpdateHandle& operator=(const FileUpdateHandle&);

    Zstring filenameFmt;
    HANDLE hFile;
    DWORD attr;
};
}

#endif // FILE_UPDATE_HANDLE_H_INCLUDED
