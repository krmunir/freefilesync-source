// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) 2008-2011 ZenJu (zhnmju123 AT gmx.de)                    *
// **************************************************************************

#include "file_handling.h"
#include <map>
#include <algorithm>
#include <stdexcept>
#include "file_traverser.h"
#include "scope_guard.h"
#include "symlink_target.h"
#include "file_io.h"
#include "assert_static.h"
#include <boost/thread/tss.hpp>
#include <boost/thread/once.hpp>
#include "file_id_def.h"

#ifdef FFS_WIN
#include "privilege.h"
#include "dll.h"
#include "win.h" //includes "windows.h"
#include "long_path_prefix.h"
#include <Aclapi.h>
#include "dst_hack.h"
#include "file_update_handle.h"
#include "win_ver.h"

#elif defined FFS_LINUX
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include <cerrno>
#include <sys/time.h>

#ifdef HAVE_SELINUX
#include <selinux/selinux.h>
#endif
#endif

using namespace zen;


bool zen::fileExists(const Zstring& filename)
{
    //symbolic links (broken or not) are also treated as existing files!
#ifdef FFS_WIN
    const DWORD ret = ::GetFileAttributes(applyLongPathPrefix(filename).c_str());
    return ret != INVALID_FILE_ATTRIBUTES && (ret & FILE_ATTRIBUTE_DIRECTORY) == 0; //returns true for (file-)symlinks also

#elif defined FFS_LINUX
    struct stat fileInfo = {};
    return ::lstat(filename.c_str(), &fileInfo) == 0 &&
           (S_ISLNK(fileInfo.st_mode) || S_ISREG(fileInfo.st_mode)); //in Linux a symbolic link is neither file nor directory
#endif
}


bool zen::dirExists(const Zstring& dirname)
{
    //symbolic links (broken or not) are also treated as existing directories!
#ifdef FFS_WIN
    const DWORD ret = ::GetFileAttributes(applyLongPathPrefix(dirname).c_str());
    return ret != INVALID_FILE_ATTRIBUTES && (ret & FILE_ATTRIBUTE_DIRECTORY) != 0; //returns true for (dir-)symlinks also

#elif defined FFS_LINUX
    struct stat dirInfo = {};
    return ::lstat(dirname.c_str(), &dirInfo) == 0 &&
           (S_ISLNK(dirInfo.st_mode) || S_ISDIR(dirInfo.st_mode)); //in Linux a symbolic link is neither file nor directory
#endif
}


bool zen::symlinkExists(const Zstring& objname)
{
#ifdef FFS_WIN
    const DWORD ret = ::GetFileAttributes(applyLongPathPrefix(objname).c_str());
    return ret != INVALID_FILE_ATTRIBUTES && (ret & FILE_ATTRIBUTE_REPARSE_POINT) != 0;

#elif defined FFS_LINUX
    struct stat fileInfo = {};
    return ::lstat(objname.c_str(), &fileInfo) == 0 &&
           S_ISLNK(fileInfo.st_mode); //symbolic link
#endif
}


bool zen::somethingExists(const Zstring& objname) //throw()       check whether any object with this name exists
{
#ifdef FFS_WIN
    return ::GetFileAttributes(applyLongPathPrefix(objname).c_str()) != INVALID_FILE_ATTRIBUTES;

#elif defined FFS_LINUX
    struct stat fileInfo = {};
    return ::lstat(objname.c_str(), &fileInfo) == 0;
#endif
}


namespace
{
void getFileAttrib(const Zstring& filename, FileAttrib& attr, ProcSymlink procSl) //throw FileError
{
#ifdef FFS_WIN
    WIN32_FIND_DATA fileInfo = {};
    {
        const HANDLE searchHandle = ::FindFirstFile(applyLongPathPrefix(filename).c_str(), &fileInfo);
        if (searchHandle == INVALID_HANDLE_VALUE)
            throw FileError(_("Error reading file attributes:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());
        ::FindClose(searchHandle);
    }
    //        WIN32_FILE_ATTRIBUTE_DATA sourceAttr = {};
    //        if (!::GetFileAttributesEx(applyLongPathPrefix(sourceObj).c_str(), //__in   LPCTSTR lpFileName,
    //                                   GetFileExInfoStandard,                  //__in   GET_FILEEX_INFO_LEVELS fInfoLevelId,
    //                                   &sourceAttr))                           //__out  LPVOID lpFileInformation

    const bool isSymbolicLink = (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
    if (!isSymbolicLink || procSl == SYMLINK_DIRECT)
    {
        //####################################### DST hack ###########################################
        const bool isDirectory = (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if (!isDirectory && dst::isFatDrive(filename)) //throw()
        {
            const dst::RawTime rawTime(fileInfo.ftCreationTime, fileInfo.ftLastWriteTime);
            if (dst::fatHasUtcEncoded(rawTime)) //throw (std::runtime_error)
            {
                fileInfo.ftLastWriteTime = dst::fatDecodeUtcTime(rawTime); //return last write time in real UTC, throw (std::runtime_error)
                ::GetSystemTimeAsFileTime(&fileInfo.ftCreationTime); //real creation time information is not available...
            }
        }
        //####################################### DST hack ###########################################

        attr.fileSize         = UInt64(fileInfo.nFileSizeLow, fileInfo.nFileSizeHigh);
        attr.modificationTime = toTimeT(fileInfo.ftLastWriteTime);
    }
    else
    {
        const HANDLE hFile = ::CreateFile(applyLongPathPrefix(filename).c_str(), //open handle to target of symbolic link
                                          0,
                                          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                          0,
                                          OPEN_EXISTING,
                                          FILE_FLAG_BACKUP_SEMANTICS,
                                          NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            throw FileError(_("Error reading file attributes:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());
        ZEN_ON_BLOCK_EXIT(::CloseHandle(hFile));

        BY_HANDLE_FILE_INFORMATION fileInfoHnd = {};
        if (!::GetFileInformationByHandle(hFile, &fileInfoHnd))
            throw FileError(_("Error reading file attributes:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());

        attr.fileSize         = UInt64(fileInfoHnd.nFileSizeLow, fileInfoHnd.nFileSizeHigh);
        attr.modificationTime = toTimeT(fileInfoHnd.ftLastWriteTime);
    }

#elif defined FFS_LINUX
    struct stat fileInfo = {};

    const int rv = procSl == SYMLINK_FOLLOW ?
                   :: stat(filename.c_str(), &fileInfo) :
                   ::lstat(filename.c_str(), &fileInfo);
    if (rv != 0) //follow symbolic links
        throw FileError(_("Error reading file attributes:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());

    attr.fileSize         = UInt64(fileInfo.st_size);
    attr.modificationTime = fileInfo.st_mtime;
#endif
}
}


UInt64 zen::getFilesize(const Zstring& filename) //throw FileError
{
    FileAttrib attr;
    getFileAttrib(filename, attr, SYMLINK_FOLLOW); //throw FileError
    return attr.fileSize;
}


Int64 zen::getFileTime(const Zstring& filename, ProcSymlink procSl) //throw FileError
{
    FileAttrib attr;
    getFileAttrib(filename, attr, procSl); //throw FileError
    return attr.modificationTime;
}


namespace
{


#ifdef FFS_WIN
DWORD retrieveVolumeSerial(const Zstring& pathName) //return 0 on error!
{
    //note: this even works for network shares: \\share\dirname

    const DWORD BUFFER_SIZE = 10000;
    std::vector<wchar_t> buffer(BUFFER_SIZE);

    //full pathName need not yet exist!
    if (!::GetVolumePathName(pathName.c_str(), //__in   LPCTSTR lpszFileName,
                             &buffer[0],       //__out  LPTSTR lpszVolumePathName,
                             BUFFER_SIZE))     //__in   DWORD cchBufferLength
        return 0;

    Zstring volumePath = &buffer[0];
    if (!endsWith(volumePath, FILE_NAME_SEPARATOR))
        volumePath += FILE_NAME_SEPARATOR;

    DWORD volumeSerial = 0;
    if (!::GetVolumeInformation(volumePath.c_str(), //__in_opt   LPCTSTR lpRootPathName,
                                NULL,               //__out      LPTSTR lpVolumeNameBuffer,
                                0,                  //__in       DWORD nVolumeNameSize,
                                &volumeSerial,      //__out_opt  LPDWORD lpVolumeSerialNumber,
                                NULL,               //__out_opt  LPDWORD lpMaximumComponentLength,
                                NULL,               //__out_opt  LPDWORD lpFileSystemFlags,
                                NULL,               //__out      LPTSTR lpFileSystemNameBuffer,
                                0))                 //__in       DWORD nFileSystemNameSize
        return 0;

    return volumeSerial;
}
#elif defined FFS_LINUX

dev_t retrieveVolumeSerial(const Zstring& pathName) //return 0 on error!
{
    Zstring volumePathName = pathName;

    //remove trailing slash
    if (volumePathName.size() > 1 && endsWith(volumePathName, FILE_NAME_SEPARATOR))  //exception: allow '/'
        volumePathName = beforeLast(volumePathName, FILE_NAME_SEPARATOR);

    struct stat fileInfo = {};
    while (::lstat(volumePathName.c_str(), &fileInfo) != 0) //go up in folder hierarchy until existing folder is found
    {
        volumePathName = beforeLast(volumePathName, FILE_NAME_SEPARATOR); //returns empty string if ch not found
        if (volumePathName.empty())
            return 0;  //this includes path "/" also!
    }

    return fileInfo.st_dev;
}
#endif
}


zen::ResponseSame zen::onSameVolume(const Zstring& folderLeft, const Zstring& folderRight) //throw()
{
    const auto serialLeft  = retrieveVolumeSerial(folderLeft);  //returns 0 on error!
    const auto serialRight = retrieveVolumeSerial(folderRight); //returns 0 on error!
    if (serialLeft == 0 || serialRight == 0)
        return IS_SAME_CANT_SAY;

    return serialLeft == serialRight ? IS_SAME_YES : IS_SAME_NO;
}


bool zen::removeFile(const Zstring& filename) //throw FileError;
{
#ifdef FFS_WIN
    //remove file, support for \\?\-prefix
    const Zstring filenameFmt = applyLongPathPrefix(filename);
    if (!::DeleteFile(filenameFmt.c_str()))
#elif defined FFS_LINUX
    if (::unlink(filename.c_str()) != 0)
#endif
    {
#ifdef FFS_WIN
        //perf: apply ONLY when necessary!
        if (::GetLastError() == ERROR_ACCESS_DENIED) //function fails if file is read-only
        {
            //(try to) normalize file attributes
            ::SetFileAttributes(filenameFmt.c_str(), FILE_ATTRIBUTE_NORMAL);

            //now try again...
            if (::DeleteFile(filenameFmt.c_str()))
                return true;
        }
        //eval error code before next call
        DWORD lastError = ::GetLastError();
#elif defined FFS_LINUX
        int lastError = errno;
#endif

        //no error situation if file is not existing! manual deletion relies on it!
        //perf: check is placed in error handling block
        //warning: this call changes error code!!
        if (!somethingExists(filename))
            return false; //neither file nor any other object (e.g. broken symlink) with that name existing

        throw FileError(_("Error deleting file:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted(lastError));
    }
    return true;
}


namespace
{
DEFINE_NEW_FILE_ERROR(ErrorDifferentVolume);

/* Usage overview: (avoid circular pattern!)

  renameFile()  -->  renameFile_sub()
      |               /|\
     \|/               |
      Fix8Dot3NameClash()
*/
//wrapper for file system rename function:
void renameFile_sub(const Zstring& oldName, const Zstring& newName) //throw FileError, ErrorDifferentVolume, ErrorTargetExisting
{
#ifdef FFS_WIN
    const Zstring oldNameFmt = applyLongPathPrefix(oldName);
    const Zstring newNameFmt = applyLongPathPrefix(newName);

    if (!::MoveFileEx(oldNameFmt.c_str(), //__in      LPCTSTR lpExistingFileName,
                      newNameFmt.c_str(), //__in_opt  LPCTSTR lpNewFileName,
                      0))                 //__in      DWORD dwFlags
    {
        DWORD lastError = ::GetLastError();
        if (lastError == ERROR_ACCESS_DENIED) //MoveFileEx may fail to rename a read-only file on a SAMBA-share -> (try to) handle this
        {
            const DWORD oldAttr = ::GetFileAttributes(oldNameFmt.c_str());
            if (oldAttr != INVALID_FILE_ATTRIBUTES && (oldAttr & FILE_ATTRIBUTE_READONLY))
            {
                if (::SetFileAttributes(oldNameFmt.c_str(), FILE_ATTRIBUTE_NORMAL)) //remove readonly-attribute
                {
                    //try again...
                    if (::MoveFileEx(oldNameFmt.c_str(), //__in      LPCTSTR lpExistingFileName,
                                     newNameFmt.c_str(), //__in_opt  LPCTSTR lpNewFileName,
                                     0))                 //__in      DWORD dwFlags
                    {
                        //(try to) restore file attributes
                        ::SetFileAttributes(newNameFmt.c_str(), oldAttr); //don't handle error
                        return;
                    }
                    else
                    {
                        lastError = ::GetLastError(); //use error code from second call to ::MoveFileEx()

                        //cleanup: (try to) restore file attributes: assume oldName is still existing
                        ::SetFileAttributes(oldNameFmt.c_str(), oldAttr);
                    }
                }
            }
        }

        std::wstring errorMessage = _("Error moving file:") + L"\n\"" + oldName +  L"\" ->\n\"" + newName + L"\"" + L"\n\n" + getLastErrorFormatted(lastError);

        if (lastError == ERROR_NOT_SAME_DEVICE)
            throw ErrorDifferentVolume(errorMessage);
        else if (lastError == ERROR_FILE_EXISTS)
            throw ErrorTargetExisting(errorMessage);
        else
            throw FileError(errorMessage);
    }

#elif defined FFS_LINUX
    if (::rename(oldName.c_str(), newName.c_str()) != 0)
    {
        const int lastError = errno;

        std::wstring errorMessage = _("Error moving file:") + L"\n\"" + oldName +  L"\" ->\n\"" + newName + L"\"" + L"\n\n" + getLastErrorFormatted(lastError);

        if (lastError == EXDEV)
            throw ErrorDifferentVolume(errorMessage);
        else if (lastError == EEXIST)
            throw ErrorTargetExisting(errorMessage);
        else
            throw FileError(errorMessage);
    }
#endif
}


#ifdef FFS_WIN
/*small wrapper around
::GetShortPathName()
::GetLongPathName() */
template <typename Function>
Zstring getFilenameFmt(const Zstring& filename, Function fun) //throw(); returns empty string on error
{
    const Zstring filenameFmt = applyLongPathPrefix(filename);

    const DWORD bufferSize = fun(filenameFmt.c_str(), NULL, 0);
    if (bufferSize == 0)
        return Zstring();

    std::vector<wchar_t> buffer(bufferSize);

    const DWORD rv = fun(filenameFmt.c_str(), //__in   LPCTSTR lpszShortPath,
                         &buffer[0],          //__out  LPTSTR  lpszLongPath,
                         static_cast<DWORD>(buffer.size()));      //__in   DWORD   cchBuffer
    if (rv == 0 || rv >= buffer.size())
        return Zstring();

    return &buffer[0];
}


Zstring findUnused8Dot3Name(const Zstring& filename) //find a unique 8.3 short name
{
    const Zstring pathPrefix = filename.find(FILE_NAME_SEPARATOR) != Zstring::npos ?
                               (beforeLast(filename, FILE_NAME_SEPARATOR) + FILE_NAME_SEPARATOR) : Zstring();

    Zstring extension = afterLast(afterLast(filename, FILE_NAME_SEPARATOR), Zchar('.')); //extension needn't contain reasonable data
    if (extension.empty())
        extension = Zstr("FFS");
    truncate(extension, 3);

    for (int index = 0; index < 100000000; ++index) //filename must be representable by <= 8 characters
    {
        const Zstring output = pathPrefix + toString<Zstring>(index) + Zchar('.') + extension;
        if (!somethingExists(output)) //ensure uniqueness
            return output;
    }

    throw std::runtime_error(std::string("100000000 files, one for each number, exist in this directory? You're kidding...\n") + utf8CvrtTo<std::string>(pathPrefix));
}


bool have8dot3NameClash(const Zstring& filename)
{
    if (filename.find(FILE_NAME_SEPARATOR) == Zstring::npos)
        return false;

    if (somethingExists(filename)) //name OR directory!
    {
        const Zstring origName  = afterLast(filename, FILE_NAME_SEPARATOR); //returns the whole string if ch not found
        const Zstring shortName = afterLast(getFilenameFmt(filename, ::GetShortPathName), FILE_NAME_SEPARATOR); //throw() returns empty string on error
        const Zstring longName  = afterLast(getFilenameFmt(filename, ::GetLongPathName) , FILE_NAME_SEPARATOR); //

        if (!shortName.empty() &&
            !longName.empty()  &&
            EqualFilename()(origName,  shortName) &&
            !EqualFilename()(shortName, longName))
        {
            //for filename short and long file name are equal and another unrelated file happens to have the same short name
            //e.g. filename == "TESTWE~1", but another file is existing named "TestWeb" with short name ""TESTWE~1"
            return true;
        }
    }
    return false;
}

class Fix8Dot3NameClash
{
public:
    Fix8Dot3NameClash(const Zstring& filename)
    {
        const Zstring longName = afterLast(getFilenameFmt(filename, ::GetLongPathName), FILE_NAME_SEPARATOR); //throw() returns empty string on error

        unrelatedFile = beforeLast(filename, FILE_NAME_SEPARATOR) + FILE_NAME_SEPARATOR + longName;

        //find another name in short format: this ensures the actual short name WILL be renamed as well!
        unrelatedFileParked = findUnused8Dot3Name(filename);

        //move already existing short name out of the way for now
        renameFile_sub(unrelatedFile, unrelatedFileParked); //throw FileError, ErrorDifferentVolume
        //DON'T call renameFile() to avoid reentrance!
    }

    ~Fix8Dot3NameClash()
    {
        //the file system should assign this unrelated file a new (unique) short name
        try
        {
            renameFile_sub(unrelatedFileParked, unrelatedFile); //throw FileError, ErrorDifferentVolume
        }
        catch (...) {}
    }
private:
    Zstring unrelatedFile;
    Zstring unrelatedFileParked;
};
#endif
}


//rename file: no copying!!!
void zen::renameFile(const Zstring& oldName, const Zstring& newName) //throw FileError, ErrorDifferentVolume, ErrorTargetExisting
{
    try
    {
        renameFile_sub(oldName, newName); //throw FileError, ErrorDifferentVolume, ErrorTargetExisting
    }
    catch (const FileError&)
    {
#ifdef FFS_WIN
        //try to handle issues with already existing short 8.3 file names on Windows
        if (have8dot3NameClash(newName))
        {
            Fix8Dot3NameClash dummy(newName); //move clashing filename to the side
            //now try again...
            renameFile_sub(oldName, newName); //throw FileError
            return;
        }
#endif
        throw;
    }
}


class CopyCallbackImpl : public zen::CallbackCopyFile //callback functionality
{
public:
    CopyCallbackImpl(const Zstring& sourceFile, CallbackMoveFile& callback) : sourceFile_(sourceFile), moveCallback(callback) {}

    virtual void deleteTargetFile(const Zstring& targetFile) { assert(!fileExists(targetFile)); }

    virtual void updateCopyStatus(UInt64 totalBytesTransferred)
    {
        moveCallback.requestUiRefresh(sourceFile_);
    }

private:
    CopyCallbackImpl(const CopyCallbackImpl&);
    CopyCallbackImpl& operator=(const CopyCallbackImpl&);

    const Zstring sourceFile_;
    CallbackMoveFile& moveCallback;
};


void zen::moveFile(const Zstring& sourceFile, const Zstring& targetFile, bool ignoreExisting, CallbackMoveFile* callback)   //throw FileError;
{
    //call back once per file (moveFile() is called by moveDirectory())
    if (callback)
        callback->requestUiRefresh(sourceFile);

    const bool targetExisting = fileExists(targetFile);

    if (targetExisting && !ignoreExisting) //test file existence: e.g. Linux might silently overwrite existing symlinks
        throw FileError(_("Error moving file:") + L"\n\"" + sourceFile +  L"\" ->\n\"" + targetFile + L"\"" +
                        L"\n\n" + _("Target file already existing!"));

    if (!targetExisting)
    {
        //try to move the file directly without copying
        try
        {
            renameFile(sourceFile, targetFile); //throw FileError, ErrorDifferentVolume
            return; //great, we get away cheaply!
        }
        //if moving failed treat as error (except when it tried to move to a different volume: in this case we will copy the file)
        catch (const ErrorDifferentVolume&) {}

        //file is on a different volume: let's copy it
        std::unique_ptr<CopyCallbackImpl> copyCallback(callback != NULL ? new CopyCallbackImpl(sourceFile, *callback) : NULL);

        if (symlinkExists(sourceFile))
            copySymlink(sourceFile, targetFile, false); //throw FileError; don't copy filesystem permissions
        else
            copyFile(sourceFile, targetFile, false, true, copyCallback.get()); //throw FileError;

        //attention: if copy-operation was cancelled an exception is thrown => sourcefile is not deleted, as we wish!
    }

    removeFile(sourceFile); //throw FileError
    //note: copying file is NOT undone in case of exception: currently this function is called in context of user-defined deletion dir, where this behavior is fine
}

namespace
{
class TraverseOneLevel : public zen::TraverseCallback
{
public:
    typedef std::pair<Zstring, Zstring> NamePair;
    typedef std::vector<NamePair> NameList;

    TraverseOneLevel(NameList& files, NameList& dirs) :
        files_(files),
        dirs_(dirs) {}

    virtual void onFile(const Zchar* shortName, const Zstring& fullName, const FileInfo& details)
    {
        files_.push_back(NamePair(shortName, fullName));
    }

    virtual void onSymlink(const Zchar* shortName, const Zstring& fullName, const SymlinkInfo& details)
    {
        if (details.dirLink)
            dirs_.push_back(NamePair(shortName, fullName));
        else
            files_.push_back(NamePair(shortName, fullName));
    }

    virtual std::shared_ptr<TraverseCallback> onDir(const Zchar* shortName, const Zstring& fullName)
    {
        dirs_.push_back(NamePair(shortName, fullName));
        return nullptr; //DON'T traverse into subdirs; moveDirectory works recursively!
    }

    virtual HandleError onError(const std::wstring& errorText) { throw FileError(errorText); }

private:
    TraverseOneLevel(const TraverseOneLevel&);
    TraverseOneLevel& operator=(const TraverseOneLevel&);

    NameList& files_;
    NameList& dirs_;
};


struct RemoveCallbackImpl : public CallbackRemoveDir
{
    RemoveCallbackImpl(const Zstring& sourceDir,
                       CallbackMoveFile& moveCallback) :
        sourceDir_(sourceDir),
        moveCallback_(moveCallback) {}

    virtual void notifyFileDeletion(const Zstring& filename) { moveCallback_.requestUiRefresh(sourceDir_); }
    virtual void notifyDirDeletion (const Zstring& dirname ) { moveCallback_.requestUiRefresh(sourceDir_); }

private:
    RemoveCallbackImpl(const RemoveCallbackImpl&);
    RemoveCallbackImpl& operator=(const RemoveCallbackImpl&);

    const Zstring sourceDir_;
    CallbackMoveFile& moveCallback_;
};
}


void moveDirectoryImpl(const Zstring& sourceDir, const Zstring& targetDir, bool ignoreExisting, CallbackMoveFile* callback) //throw FileError
{
    //call back once per folder
    if (callback)
        callback->requestUiRefresh(sourceDir);

    const bool targetExisting = dirExists(targetDir);

    if (targetExisting && !ignoreExisting) //directory or symlink exists (or even a file... this error will be caught later)
        throw FileError(_("Error moving directory:") + L"\n\"" + sourceDir +  L"\" ->\n\"" + targetDir + L"\"" +
                        L"\n\n" + _("Target directory already existing!"));

    const bool isSymlink = symlinkExists(sourceDir);

    if (!targetExisting)
    {
        //first try to move the directory directly without copying
        try
        {
            renameFile(sourceDir, targetDir); //throw FileError, ErrorDifferentVolume, ErrorTargetExisting
            return; //great, we get away cheaply!
        }
        //if moving failed treat as error (except when it tried to move to a different volume: in this case we will copy the directory)
        catch (const ErrorDifferentVolume&) {}

        //create target
        if (isSymlink)
            copySymlink(sourceDir, targetDir, false); //throw FileError -> don't copy permissions
        else
            createDirectory(targetDir, sourceDir, false); //throw FileError
    }

    if (!isSymlink) //handle symbolic links
    {
        //move files/folders recursively
        TraverseOneLevel::NameList fileList; //list of names: 1. short 2.long
        TraverseOneLevel::NameList dirList;  //

        //traverse source directory one level
        TraverseOneLevel traverseCallback(fileList, dirList);
        traverseFolder(sourceDir, false, traverseCallback); //traverse one level, don't follow symlinks

        const Zstring targetDirFormatted = endsWith(targetDir, FILE_NAME_SEPARATOR) ? //ends with path separator
                                           targetDir :
                                           targetDir + FILE_NAME_SEPARATOR;

        //move files
        for (TraverseOneLevel::NameList::const_iterator i = fileList.begin(); i != fileList.end(); ++i)
            moveFile(i->second, targetDirFormatted + i->first, ignoreExisting, callback); //throw FileError, ErrorTargetExisting

        //move directories
        for (TraverseOneLevel::NameList::const_iterator i = dirList.begin(); i != dirList.end(); ++i)
            ::moveDirectoryImpl(i->second, targetDirFormatted + i->first, ignoreExisting, callback);

        //attention: if move-operation was cancelled an exception is thrown => sourceDir is not deleted, as we wish!
    }

    //delete source
    std::unique_ptr<RemoveCallbackImpl> removeCallback(callback != NULL ? new RemoveCallbackImpl(sourceDir, *callback) : NULL);
    removeDirectory(sourceDir, removeCallback.get()); //throw FileError;
}


void zen::moveDirectory(const Zstring& sourceDir, const Zstring& targetDir, bool ignoreExisting, CallbackMoveFile* callback) //throw FileError
{
#ifdef FFS_WIN
    const Zstring& sourceDirFormatted = sourceDir;
    const Zstring& targetDirFormatted = targetDir;

#elif defined FFS_LINUX
    const Zstring sourceDirFormatted = //remove trailing slash
        sourceDir.size() > 1 && endsWith(sourceDir, FILE_NAME_SEPARATOR) ?  //exception: allow '/'
        beforeLast(sourceDir, FILE_NAME_SEPARATOR) :
        sourceDir;
    const Zstring targetDirFormatted = //remove trailing slash
        targetDir.size() > 1 && endsWith(targetDir, FILE_NAME_SEPARATOR) ?  //exception: allow '/'
        beforeLast(targetDir, FILE_NAME_SEPARATOR) :
        targetDir;
#endif

    ::moveDirectoryImpl(sourceDirFormatted, targetDirFormatted, ignoreExisting, callback);
}


class FilesDirsOnlyTraverser : public zen::TraverseCallback
{
public:
    FilesDirsOnlyTraverser(std::vector<Zstring>& files, std::vector<Zstring>& dirs) :
        m_files(files),
        m_dirs(dirs) {}

    virtual void onFile(const Zchar* shortName, const Zstring& fullName, const FileInfo& details)
    {
        m_files.push_back(fullName);
    }
    virtual void onSymlink(const Zchar* shortName, const Zstring& fullName, const SymlinkInfo& details)
    {
        if (details.dirLink)
            m_dirs.push_back(fullName);
        else
            m_files.push_back(fullName);
    }
    virtual std::shared_ptr<TraverseCallback> onDir(const Zchar* shortName, const Zstring& fullName)
    {
        m_dirs.push_back(fullName);
        return nullptr; //DON'T traverse into subdirs; removeDirectory works recursively!
    }
    virtual HandleError onError(const std::wstring& errorText) { throw FileError(errorText); }

private:
    FilesDirsOnlyTraverser(const FilesDirsOnlyTraverser&);
    FilesDirsOnlyTraverser& operator=(const FilesDirsOnlyTraverser&);

    std::vector<Zstring>& m_files;
    std::vector<Zstring>& m_dirs;
};


void zen::removeDirectory(const Zstring& directory, CallbackRemoveDir* callback)
{
    //no error situation if directory is not existing! manual deletion relies on it!
    if (!somethingExists(directory))
        return; //neither directory nor any other object (e.g. broken symlink) with that name existing

#ifdef FFS_WIN
    const Zstring directoryFmt = applyLongPathPrefix(directory); //support for \\?\-prefix

    //(try to) normalize file attributes: actually NEEDED for symbolic links also!
    ::SetFileAttributes(directoryFmt.c_str(), FILE_ATTRIBUTE_NORMAL);
#endif

    //attention: check if directory is a symlink! Do NOT traverse into it deleting contained files!!!
    if (symlinkExists(directory)) //remove symlink directly
    {
#ifdef FFS_WIN
        if (!::RemoveDirectory(directoryFmt.c_str()))
#elif defined FFS_LINUX
        if (::unlink(directory.c_str()) != 0)
#endif
            throw FileError(_("Error deleting directory:") + L"\n\"" + directory + L"\"" + L"\n\n" + getLastErrorFormatted());

        if (callback)
            callback->notifyDirDeletion(directory); //once per symlink
        return;
    }

    std::vector<Zstring> fileList;
    std::vector<Zstring> dirList;

    //get all files and directories from current directory (WITHOUT subdirectories!)
    FilesDirsOnlyTraverser traverser(fileList, dirList);
    traverseFolder(directory, false, traverser); //don't follow symlinks

    //delete files
    for (std::vector<Zstring>::const_iterator i = fileList.begin(); i != fileList.end(); ++i)
    {
        const bool workDone = removeFile(*i);
        if (callback && workDone)
            callback->notifyFileDeletion(*i); //call once per file
    }

    //delete directories recursively
    for (std::vector<Zstring>::const_iterator i = dirList.begin(); i != dirList.end(); ++i)
        removeDirectory(*i, callback); //call recursively to correctly handle symbolic links

    //parent directory is deleted last
#ifdef FFS_WIN
    if (!::RemoveDirectory(directoryFmt.c_str())) //remove directory, support for \\?\-prefix
#else
    if (::rmdir(directory.c_str()) != 0)
#endif
    {
        throw FileError(_("Error deleting directory:") + L"\n\"" + directory + L"\"" + L"\n\n" + getLastErrorFormatted());
    }
    if (callback)
        callback->notifyDirDeletion(directory); //and once per folder
}


void zen::setFileTime(const Zstring& filename, const Int64& modificationTime, ProcSymlink procSl) //throw FileError
{
#ifdef FFS_WIN
    FILETIME creationTime  = {};
    FILETIME lastWriteTime = tofiletime(modificationTime);

    //####################################### DST hack ###########################################
    if (dst::isFatDrive(filename)) //throw()
    {
        const dst::RawTime encodedTime = dst::fatEncodeUtcTime(lastWriteTime); //throw (std::runtime_error)
        creationTime  = encodedTime.createTimeRaw;
        lastWriteTime = encodedTime.writeTimeRaw;
    }
    //####################################### DST hack ###########################################

    //privilege SE_BACKUP_NAME doesn't seem to be required here for symbolic links
    //note: setting privileges requires admin rights!

    //opening newly created target file may fail due to some AV-software scanning it: no error, we will wait!
    //http://support.microsoft.com/?scid=kb%3Ben-us%3B316609&x=17&y=20
    //-> enable as soon it turns out it is required!

    /*const int retryInterval = 50;
    const int maxRetries = 2000 / retryInterval;
    for (int i = 0; i < maxRetries; ++i)
    {
    */

    //may need to remove the readonly-attribute (e.g. FAT usb drives)
    FileUpdateHandle targetHandle(filename, [ = ]()
    {
        return ::CreateFile(applyLongPathPrefix(filename).c_str(),
                            GENERIC_READ | GENERIC_WRITE, //use both when writing over network, see comment in file_io.cpp
                            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                            0,
                            OPEN_EXISTING,
                            FILE_FLAG_BACKUP_SEMANTICS | //needed to open a directory
                            (procSl == SYMLINK_DIRECT ? FILE_FLAG_OPEN_REPARSE_POINT : 0), //process symlinks
                            NULL);
    });

    if (targetHandle.get() == INVALID_HANDLE_VALUE)
        throw FileError(_("Error changing modification time:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());

    /*
    if (hTarget == INVALID_HANDLE_VALUE && ::GetLastError() == ERROR_SHARING_VIOLATION)
        ::Sleep(retryInterval); //wait then retry
    else //success or unknown failure
        break;
    }
    */

    auto isNullTime = [](const FILETIME & ft) { return ft.dwLowDateTime == 0 && ft.dwHighDateTime == 0; };

    if (!::SetFileTime(targetHandle.get(),
                       isNullTime(creationTime) ? NULL : &creationTime,
                       NULL,
                       &lastWriteTime))
        throw FileError(_("Error changing modification time:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());

#ifndef NDEBUG //dst hack: verify data written
    if (dst::isFatDrive(filename) && !dirExists(filename)) //throw()
    {
        WIN32_FILE_ATTRIBUTE_DATA debugeAttr = {};
        assert(::GetFileAttributesEx(applyLongPathPrefix(filename).c_str(), //__in   LPCTSTR lpFileName,
                                     GetFileExInfoStandard,                 //__in   GET_FILEEX_INFO_LEVELS fInfoLevelId,
                                     &debugeAttr));                         //__out  LPVOID lpFileInformation

        assert(::CompareFileTime(&debugeAttr.ftCreationTime,  &creationTime)  == 0);
        assert(::CompareFileTime(&debugeAttr.ftLastWriteTime, &lastWriteTime) == 0);
    }
#endif

#elif defined FFS_LINUX
    if (procSl == SYMLINK_FOLLOW)
    {
        struct utimbuf newTimes = {};
        newTimes.actime  = ::time(NULL);
        newTimes.modtime = to<time_t>(modificationTime);

        // set new "last write time"
        if (::utime(filename.c_str(), &newTimes) != 0)
            throw FileError(_("Error changing modification time:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());
    }
    else
    {
        struct timeval newTimes[2] = {};
        newTimes[0].tv_sec  = ::time(NULL);	/* seconds */
        newTimes[0].tv_usec = 0;	        /* microseconds */

        newTimes[1].tv_sec  = to<time_t>(modificationTime);
        newTimes[1].tv_usec = 0;

        if (::lutimes(filename.c_str(), newTimes) != 0)
            throw FileError(_("Error changing modification time:") + L"\n\"" + filename + L"\"" + L"\n\n" + getLastErrorFormatted());
    }
#endif
}


namespace
{
#ifdef FFS_WIN
Zstring getSymlinkTargetPath(const Zstring& symlink) //throw FileError
{
    //open handle to target of symbolic link
    const HANDLE hDir = ::CreateFile(applyLongPathPrefix(symlink).c_str(),
                                     0,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                     0,
                                     OPEN_EXISTING,
                                     FILE_FLAG_BACKUP_SEMANTICS,  //needed to open a directory
                                     NULL);
    if (hDir == INVALID_HANDLE_VALUE)
        throw FileError(_("Error resolving symbolic link:") + L"\n\"" + symlink + L"\"" + L"\n\n" + getLastErrorFormatted());
    ZEN_ON_BLOCK_EXIT(::CloseHandle(hDir));

    //dynamically load windows API function
    typedef DWORD (WINAPI *GetFinalPathNameByHandleWFunc)(HANDLE hFile,
                                                          LPTSTR lpszFilePath,
                                                          DWORD cchFilePath,
                                                          DWORD dwFlags);
    const SysDllFun<GetFinalPathNameByHandleWFunc> getFinalPathNameByHandle(L"kernel32.dll", "GetFinalPathNameByHandleW");
    if (!getFinalPathNameByHandle)
        throw FileError(_("Error loading library function:") + L"\n\"" + L"GetFinalPathNameByHandleW" + L"\"");

    const DWORD BUFFER_SIZE = 10000;
    std::vector<wchar_t> targetPath(BUFFER_SIZE);
    const DWORD charsWritten = getFinalPathNameByHandle(hDir,                  //__in   HANDLE hFile,
                                                        &targetPath[0],        //__out  LPTSTR lpszFilePath,
                                                        BUFFER_SIZE,           //__in   DWORD cchFilePath,
                                                        FILE_NAME_NORMALIZED); //__in   DWORD dwFlags
    if (charsWritten >= BUFFER_SIZE || charsWritten == 0)
    {
        std::wstring errorMessage = _("Error resolving symbolic link:") + L"\n\"" + symlink + L"\"";
        if (charsWritten == 0)
            errorMessage += L"\n\n" + getLastErrorFormatted();
        throw FileError(errorMessage);
    }

    return Zstring(&targetPath[0], charsWritten);
}
#endif


#ifdef HAVE_SELINUX
//copy SELinux security context
void copySecurityContext(const Zstring& source, const Zstring& target, ProcSymlink procSl) //throw FileError
{
    security_context_t contextSource = NULL;
    const int rv = procSl == SYMLINK_FOLLOW ?
                   ::getfilecon(source.c_str(), &contextSource) :
                   ::lgetfilecon(source.c_str(), &contextSource);
    if (rv < 0)
    {
        if (errno == ENODATA ||  //no security context (allegedly) is not an error condition on SELinux
            errno == EOPNOTSUPP) //extended attributes are not supported by the filesystem
            return;

        throw FileError(_("Error reading security context:") + L"\n\"" + source + L"\"" + L"\n\n" + getLastErrorFormatted());
    }
    ZEN_ON_BLOCK_EXIT(::freecon(contextSource));

    {
        security_context_t contextTarget = NULL;
        const int rv2 = procSl == SYMLINK_FOLLOW ?
                        ::getfilecon(target.c_str(), &contextTarget) :
                        ::lgetfilecon(target.c_str(), &contextTarget);
        if (rv2 < 0)
        {
            if (errno == EOPNOTSUPP)
                return;
            //else: still try to set security context
        }
        else
        {
            ZEN_ON_BLOCK_EXIT(::freecon(contextTarget));

            if (::strcmp(contextSource, contextTarget) == 0) //nothing to do
                return;
        }
    }

    const int rv3 = procSl == SYMLINK_FOLLOW ?
                    ::setfilecon(target.c_str(), contextSource) :
                    ::lsetfilecon(target.c_str(), contextSource);
    if (rv3 < 0)
        throw FileError(_("Error writing security context:") + L"\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted());
}
#endif //HAVE_SELINUX


//copy permissions for files, directories or symbolic links: requires admin rights
void copyObjectPermissions(const Zstring& source, const Zstring& target, ProcSymlink procSl) //throw FileError;
{
#ifdef FFS_WIN
    //setting privileges requires admin rights!
    try
    {
        //enable privilege: required to read/write SACL information
        activatePrivilege(SE_SECURITY_NAME); //polling allowed...

        //enable privilege: required to copy owner information
        activatePrivilege(SE_RESTORE_NAME);

        //the following privilege may be required according to http://msdn.microsoft.com/en-us/library/aa364399(VS.85).aspx (although not needed nor active in my tests)
        activatePrivilege(SE_BACKUP_NAME);
    }
    catch (const FileError& e)
    {
        throw FileError(_("Error copying file permissions:") + L"\n\"" + source +  L"\" ->\n\"" + target + L"\"" + L"\n\n" + e.toString());
    }

    //in contrast to ::SetSecurityInfo(), ::SetFileSecurity() seems to honor the "inherit DACL/SACL" flags

    //NOTE: ::GetFileSecurity()/::SetFileSecurity() do NOT follow Symlinks!
    const Zstring sourceResolved = procSl == SYMLINK_FOLLOW && symlinkExists(source) ? getSymlinkTargetPath(source) : source;
    const Zstring targetResolved = procSl == SYMLINK_FOLLOW && symlinkExists(target) ? getSymlinkTargetPath(target) : target;

    std::vector<char> buffer(10000); //example of actually required buffer size: 192 bytes
    for (;;)
    {
        DWORD bytesNeeded = 0;
        if (::GetFileSecurity(applyLongPathPrefix(sourceResolved).c_str(), //__in LPCTSTR lpFileName, -> long path prefix IS needed, although it is NOT mentioned on MSDN!!!
                              OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                              DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,  //__in       SECURITY_INFORMATION RequestedInformation,
                              reinterpret_cast<PSECURITY_DESCRIPTOR>(&buffer[0]),     //__out_opt  PSECURITY_DESCRIPTOR pSecurityDescriptor,
                              static_cast<DWORD>(buffer.size()), //__in       DWORD nLength,
                              &bytesNeeded))                     //__out      LPDWORD lpnLengthNeeded
            break;
        //failure: ...
        if (bytesNeeded > buffer.size())
            buffer.resize(bytesNeeded);
        else
            throw FileError(_("Error copying file permissions:") + L"\n\"" + sourceResolved + L"\" ->\n\"" + targetResolved + L"\"" + L"\n\n" + getLastErrorFormatted() + L" (R)");
    }
    SECURITY_DESCRIPTOR& secDescr = reinterpret_cast<SECURITY_DESCRIPTOR&>(buffer[0]);

    /*
        SECURITY_DESCRIPTOR_CONTROL secCtrl = 0;
        {
            DWORD ctrlRev = 0;
            if (!::GetSecurityDescriptorControl(&secDescr, // __in   PSECURITY_DESCRIPTOR pSecurityDescriptor,
                                                &secCtrl,  // __out  PSECURITY_DESCRIPTOR_CONTROL pControl,
                                                &ctrlRev)) //__out  LPDWORD lpdwRevision
                throw FileError(_("Error copying file permissions:") + L"\n\"" + sourceResolved + L"\" ->\n\"" + targetResolved + L"\"" + L"\n\n" + getLastErrorFormatted() + L" (C)");
       }
    //interesting flags:
    //#define SE_DACL_PRESENT                  (0x0004)
    //#define SE_SACL_PRESENT                  (0x0010)
    //#define SE_DACL_PROTECTED                (0x1000)
    //#define SE_SACL_PROTECTED                (0x2000)
    */

    if (!::SetFileSecurity(applyLongPathPrefix(targetResolved).c_str(), //__in  LPCTSTR lpFileName, -> long path prefix IS needed, although it is NOT mentioned on MSDN!!!
                           OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                           DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION, //__in  SECURITY_INFORMATION SecurityInformation,
                           &secDescr)) //__in  PSECURITY_DESCRIPTOR pSecurityDescriptor
        throw FileError(_("Error copying file permissions:") + L"\n\"" + sourceResolved + L"\" ->\n\"" + targetResolved + L"\"" + L"\n\n" + getLastErrorFormatted() + L" (W)");

    /*
    PSECURITY_DESCRIPTOR buffer = NULL;
    PSID owner                  = NULL;
    PSID group                  = NULL;
    PACL dacl                   = NULL;
    PACL sacl                   = NULL;

    //File Security and Access Rights:    http://msdn.microsoft.com/en-us/library/aa364399(v=VS.85).aspx
    //SECURITY_INFORMATION Access Rights: http://msdn.microsoft.com/en-us/library/windows/desktop/aa379573(v=vs.85).aspx
    const HANDLE hSource = ::CreateFile(applyLongPathPrefix(source).c_str(),
                                        READ_CONTROL | ACCESS_SYSTEM_SECURITY, //ACCESS_SYSTEM_SECURITY required for SACL access
                                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                        0,
                                        OPEN_EXISTING,
                                        FILE_FLAG_BACKUP_SEMANTICS | (procSl == SYMLINK_DIRECT ? FILE_FLAG_OPEN_REPARSE_POINT : 0), //FILE_FLAG_BACKUP_SEMANTICS needed to open a directory
                                        NULL);
    if (hSource == INVALID_HANDLE_VALUE)
        throw FileError(_("Error copying file permissions:") + L"\n\"" + source + L"\" ->\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted() + L" (OR)");
    ZEN_ON_BLOCK_EXIT(::CloseHandle(hSource));

    //  DWORD rc = ::GetNamedSecurityInfo(const_cast<WCHAR*>(applyLongPathPrefix(source).c_str()), -> does NOT dereference symlinks!
    DWORD rc = ::GetSecurityInfo(hSource,        //__in       LPTSTR pObjectName,
                                 SE_FILE_OBJECT, //__in       SE_OBJECT_TYPE ObjectType,
                                 OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                                 DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,  //__in       SECURITY_INFORMATION SecurityInfo,
                                 &owner,   //__out_opt  PSID *ppsidOwner,
                                 &group,   //__out_opt  PSID *ppsidGroup,
                                 &dacl,    //__out_opt  PACL *ppDacl,
                                 &sacl,    //__out_opt  PACL *ppSacl,
                                 &buffer); //__out_opt  PSECURITY_DESCRIPTOR *ppSecurityDescriptor
    if (rc != ERROR_SUCCESS)
        throw FileError(_("Error copying file permissions:") + L"\n\"" + source + L"\" ->\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted(rc) + L" (R)");
    ZEN_ON_BLOCK_EXIT(::LocalFree(buffer));

    SECURITY_DESCRIPTOR_CONTROL secCtrl = 0;
    {
    DWORD ctrlRev = 0;
    if (!::GetSecurityDescriptorControl(buffer, // __in   PSECURITY_DESCRIPTOR pSecurityDescriptor,
    &secCtrl, // __out  PSECURITY_DESCRIPTOR_CONTROL pControl,
    &ctrlRev))//__out  LPDWORD lpdwRevision
        throw FileError(_("Error copying file permissions:") + L"\n\"" + source + L"\" ->\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted(rc) + L" (C)");
    }

    //may need to remove the readonly-attribute
    FileUpdateHandle targetHandle(target, [ = ]()
    {
        return ::CreateFile(applyLongPathPrefix(target).c_str(),                              // lpFileName
                            GENERIC_WRITE | WRITE_OWNER | WRITE_DAC | ACCESS_SYSTEM_SECURITY, // dwDesiredAccess: all four seem to be required!!!
                            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,           // dwShareMode
                            0,                       // lpSecurityAttributes
                            OPEN_EXISTING,              // dwCreationDisposition
                            FILE_FLAG_BACKUP_SEMANTICS | (procSl == SYMLINK_DIRECT ? FILE_FLAG_OPEN_REPARSE_POINT : 0), // dwFlagsAndAttributes
                            NULL);                        // hTemplateFile
    });

    if (targetHandle.get() == INVALID_HANDLE_VALUE)
        throw FileError(_("Error copying file permissions:") + L"\n\"" + source + L"\" ->\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted() + L" (OW)");

    	SECURITY_INFORMATION secFlags = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION;

    	//SACL/DACL inheritence flag is NOT copied by default: we have to tell ::SetSecurityInfo(() to enable/disable it manually!
    	//if (secCtrl & SE_DACL_PRESENT)
    	secFlags |= (secCtrl & SE_DACL_PROTECTED) ? PROTECTED_DACL_SECURITY_INFORMATION : UNPROTECTED_DACL_SECURITY_INFORMATION;
    	//if (secCtrl & SE_SACL_PRESENT)
    	secFlags |= (secCtrl & SE_SACL_PROTECTED) ? PROTECTED_SACL_SECURITY_INFORMATION : UNPROTECTED_SACL_SECURITY_INFORMATION;


    //  rc = ::SetNamedSecurityInfo(const_cast<WCHAR*>(applyLongPathPrefix(target).c_str()), //__in      LPTSTR pObjectName, -> does NOT dereference symlinks!
    rc = ::SetSecurityInfo(targetHandle.get(), //__in      LPTSTR pObjectName,
                           SE_FILE_OBJECT,     //__in      SE_OBJECT_TYPE ObjectType,
                           secFlags, //__in      SECURITY_INFORMATION SecurityInfo,
                           owner, //__in_opt  PSID psidOwner,
                           group, //__in_opt  PSID psidGroup,
                           dacl,  //__in_opt  PACL pDacl,
                           sacl); //__in_opt  PACL pSacl

    if (rc != ERROR_SUCCESS)
        throw FileError(_("Error copying file permissions:") + L"\n\"" + source + L"\" ->\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted(rc) + L" (W)");
    		*/

#elif defined FFS_LINUX

#ifdef HAVE_SELINUX  //copy SELinux security context
    copySecurityContext(source, target, procSl); //throw FileError
#endif

    struct stat fileInfo = {};
    if (procSl == SYMLINK_FOLLOW)
    {
        if (::stat(source.c_str(), &fileInfo) != 0                        ||
            ::chown(target.c_str(), fileInfo.st_uid, fileInfo.st_gid) != 0 || // may require admin rights!
            ::chmod(target.c_str(), fileInfo.st_mode) != 0)
            throw FileError(_("Error copying file permissions:") + L"\n\"" + source + L"\" ->\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted() + L" (R)");
    }
    else
    {
        if (::lstat(source.c_str(), &fileInfo) != 0                        ||
            ::lchown(target.c_str(), fileInfo.st_uid, fileInfo.st_gid) != 0 || // may require admin rights!
            (!symlinkExists(target) && ::chmod(target.c_str(), fileInfo.st_mode) != 0)) //setting access permissions doesn't make sense for symlinks on Linux: there is no lchmod()
            throw FileError(_("Error copying file permissions:") + L"\n\"" + source + L"\" ->\n\"" + target + L"\"" + L"\n\n" + getLastErrorFormatted() + L" (W)");
    }
#endif
}


void createDirectory_straight(const Zstring& directory, const Zstring& templateDir, bool copyFilePermissions, int level)
{
    //default directory creation
#ifdef FFS_WIN
    //don't use ::CreateDirectoryEx:
    //- it may fail with "wrong parameter (error code 87)" when source is on mapped online storage
    //- automatically copies symbolic links if encountered: unfortunately it doesn't copy symlinks over network shares but silently creates empty folders instead (on XP)!
    //- it isn't able to copy most junctions because of missing permissions (although target path can be retrieved alternatively!)
    if (!::CreateDirectory(applyLongPathPrefixCreateDir(directory).c_str(), NULL))
#elif defined FFS_LINUX
    if (::mkdir(directory.c_str(), 0755) != 0)
#endif
    {
        if (level != 0) return;
        throw FileError(_("Error creating directory:") + L"\n\"" + directory + L"\"" + L"\n\n" + getLastErrorFormatted());
    }

    if (!templateDir.empty())
    {
#ifdef FFS_WIN
        //try to copy file attributes
        Zstring sourcePath;

        if (symlinkExists(templateDir)) //dereference symlink!
        {
            try
            {
                //get target directory of symbolic link
                sourcePath = getSymlinkTargetPath(templateDir); //throw FileError
            }
            catch (FileError&) {} //dereferencing a symbolic link usually fails if it is located on network drive or client is XP: NOT really an error...
        }
        else     //no symbolic link
            sourcePath = templateDir;

        //try to copy file attributes
        if (!sourcePath.empty())
        {
            const DWORD sourceAttr = ::GetFileAttributes(applyLongPathPrefix(sourcePath).c_str());
            if (sourceAttr != INVALID_FILE_ATTRIBUTES)
            {
                const bool isCompressed = (sourceAttr & FILE_ATTRIBUTE_COMPRESSED)  != 0;
                const bool isEncrypted  = (sourceAttr & FILE_ATTRIBUTE_ENCRYPTED)   != 0;

                ::SetFileAttributes(applyLongPathPrefix(directory).c_str(), sourceAttr);

                if (isEncrypted)
                    ::EncryptFile(directory.c_str()); //seems no long path is required (check passed!)

                if (isCompressed)
                {
                    HANDLE hDir = ::CreateFile(applyLongPathPrefix(directory).c_str(),
                                               GENERIC_READ | GENERIC_WRITE, //read access required for FSCTL_SET_COMPRESSION
                                               FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                               0,
                                               OPEN_EXISTING,
                                               FILE_FLAG_BACKUP_SEMANTICS,
                                               NULL);
                    if (hDir != INVALID_HANDLE_VALUE)
                    {
                        ZEN_ON_BLOCK_EXIT(::CloseHandle(hDir));

                        USHORT cmpState = COMPRESSION_FORMAT_DEFAULT;

                        DWORD bytesReturned = 0;
                        ::DeviceIoControl(hDir,                  //handle to file or directory
                                          FSCTL_SET_COMPRESSION, //dwIoControlCode
                                          &cmpState,             //input buffer
                                          sizeof(cmpState),      //size of input buffer
                                          NULL,                  //lpOutBuffer
                                          0,                     //OutBufferSize
                                          &bytesReturned,        //number of bytes returned
                                          NULL);                 //OVERLAPPED structure
                    }
                }
            }
        }
#endif
        zen::ScopeGuard guardNewDir = zen::makeGuard([&]() { removeDirectory(directory); }); //ensure cleanup:

        //enforce copying file permissions: it's advertized on GUI...
        if (copyFilePermissions)
            copyObjectPermissions(templateDir, directory, SYMLINK_FOLLOW); //throw FileError

        guardNewDir.dismiss(); //target has been created successfully!
    }
}


void createDirectoryRecursively(const Zstring& directory, const Zstring& templateDir, bool copyFilePermissions, int level)
{
    if (level == 100) //catch endless recursion
        return;

#ifdef FFS_WIN
    std::unique_ptr<Fix8Dot3NameClash> fnc;
    if (somethingExists(directory))
    {
        //handle issues with already existing short 8.3 file names on Windows
        if (have8dot3NameClash(directory))
            fnc.reset(new Fix8Dot3NameClash(directory)); //move clashing object to the side
        else if (dirExists(directory))
            return;
    }
#elif defined FFS_LINUX
    if (dirExists(directory))
        return;
#endif
    else //if "somethingExists" we needn't create the parent directory
    {
        //try to create parent folders first
        const Zstring dirParent = beforeLast(directory, FILE_NAME_SEPARATOR);
        if (!dirParent.empty() && !dirExists(dirParent))
        {
            //call function recursively
            const Zstring templateParent = beforeLast(templateDir, FILE_NAME_SEPARATOR); //returns empty string if ch not found
            createDirectoryRecursively(dirParent, templateParent, copyFilePermissions, level + 1);
        }
    }

    //now creation should be possible
    createDirectory_straight(directory, templateDir, copyFilePermissions, level); //throw FileError
}
}


void zen::createDirectory(const Zstring& directory, const Zstring& templateDir, bool copyFilePermissions)
{
    //remove trailing separator
    const Zstring dirFormatted = endsWith(directory, FILE_NAME_SEPARATOR) ?
                                 beforeLast(directory, FILE_NAME_SEPARATOR) :
                                 directory;

    const Zstring templateFormatted = endsWith(templateDir, FILE_NAME_SEPARATOR) ?
                                      beforeLast(templateDir, FILE_NAME_SEPARATOR) :
                                      templateDir;

    createDirectoryRecursively(dirFormatted, templateFormatted, copyFilePermissions, 0);
}


void zen::createDirectory(const Zstring& directory)
{
    zen::createDirectory(directory, Zstring(), false);
}


void zen::copySymlink(const Zstring& sourceLink, const Zstring& targetLink, bool copyFilePermissions) //throw FileError
{
    const Zstring linkPath = getSymlinkRawTargetString(sourceLink); //accept broken symlinks; throw FileError

#ifdef FFS_WIN
    const bool isDirLink = [&]() -> bool
    {
        const DWORD ret = ::GetFileAttributes(applyLongPathPrefix(sourceLink).c_str());
        return ret != INVALID_FILE_ATTRIBUTES && (ret& FILE_ATTRIBUTE_DIRECTORY);
    }();

    //dynamically load windows API function
    typedef BOOLEAN (WINAPI *CreateSymbolicLinkFunc)(LPCTSTR lpSymlinkFileName, LPCTSTR lpTargetFileName, DWORD dwFlags);

    const SysDllFun<CreateSymbolicLinkFunc> createSymbolicLink(L"kernel32.dll", "CreateSymbolicLinkW");
    if (!createSymbolicLink)
        throw FileError(_("Error loading library function:") + L"\n\"" + L"CreateSymbolicLinkW" + L"\"");

    if (!createSymbolicLink(targetLink.c_str(), //__in  LPTSTR lpSymlinkFileName, - seems no long path prefix is required...
                            linkPath.c_str(),   //__in  LPTSTR lpTargetFileName,
                            (isDirLink ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0))) //__in  DWORD dwFlags
#elif defined FFS_LINUX
    if (::symlink(linkPath.c_str(), targetLink.c_str()) != 0)
#endif
        throw FileError(_("Error copying symbolic link:") + L"\n\"" + sourceLink + L"\" ->\n\"" + targetLink + L"\"" + L"\n\n" + getLastErrorFormatted());

    //allow only consistent objects to be created -> don't place before ::symlink, targetLink may already exist
    zen::ScopeGuard guardNewDir = zen::makeGuard([&]()
    {
#ifdef FFS_WIN
        if (isDirLink)
            removeDirectory(targetLink);
        else
#endif
            removeFile(targetLink);
    });

    //file times: essential for a symlink: enforce this! (don't just try!)
    {
        const Int64 modTime = getFileTime(sourceLink, SYMLINK_DIRECT); //throw FileError
        setFileTime(targetLink, modTime, SYMLINK_DIRECT); //throw FileError
    }

    if (copyFilePermissions)
        copyObjectPermissions(sourceLink, targetLink, SYMLINK_DIRECT); //throw FileError

    guardNewDir.dismiss(); //target has been created successfully!
}


namespace
{
Zstring createTempName(const Zstring& filename)
{
    Zstring output = filename + zen::TEMP_FILE_ENDING;

    //ensure uniqueness
    for (int i = 1; somethingExists(output); ++i)
        output = filename + Zchar('_') + toString<Zstring>(i) + zen::TEMP_FILE_ENDING;

    return output;
}

#ifdef FFS_WIN
class CallbackData
{
public:
    CallbackData(CallbackCopyFile* cb, //may be NULL
                 const Zstring& sourceFile,
                 const Zstring& targetFile,
                 bool osIsvistaOrLater) :
        userCallback(cb),
        sourceFile_(sourceFile),
        targetFile_(targetFile),
        osIsvistaOrLater_(osIsvistaOrLater),
        exceptionInUserCallback(false) {}

    CallbackCopyFile* userCallback; //optional!
    const Zstring& sourceFile_;
    const Zstring& targetFile_;
    const bool osIsvistaOrLater_;

    //there is some mixed responsibility in this class, pure read-only data and abstraction for error reporting
    //however we need to keep it at one place as ::CopyFileEx() requires!

    void reportUserException(const UInt64& bytesTransferred)
    {
        exceptionInUserCallback = true;
        bytesTransferredOnException = bytesTransferred;
    }

    void reportError(const std::wstring& message) { errorMsg = message; }

    void evaluateErrors() //throw
    {
        if (exceptionInUserCallback)
        {
            assert(userCallback);
            if (userCallback)
                userCallback->updateCopyStatus(bytesTransferredOnException); //rethrow (hopefully!)
        }
        if (!errorMsg.empty())
            throw FileError(errorMsg);
    }

    void setNewAttr(const FileAttrib& attr) { newAttrib = attr; }

    FileAttrib getSrcAttr() const
    {
        assert(newAttrib.modificationTime != 0);
        return newAttrib;
    }

private:
    CallbackData(const CallbackData&);
    CallbackData& operator=(const CallbackData&);

    FileAttrib newAttrib;
    std::wstring errorMsg;        //
    bool exceptionInUserCallback; //these two are exclusive!
    UInt64 bytesTransferredOnException;
};


DWORD CALLBACK copyCallbackInternal(
    LARGE_INTEGER totalFileSize,
    LARGE_INTEGER totalBytesTransferred,
    LARGE_INTEGER streamSize,
    LARGE_INTEGER streamBytesTransferred,
    DWORD dwStreamNumber,
    DWORD dwCallbackReason,
    HANDLE hSourceFile,
    HANDLE hDestinationFile,
    LPVOID lpData)
{
    //this callback is invoked for block sizes managed by Windows, these may vary from e.g. 64 kB up to 1MB. It seems this is dependent from file size amongst others.

    //symlink handling:
    //if source is a symlink and COPY_FILE_COPY_SYMLINK is     specified, this callback is NOT invoked!
    //if source is a symlink and COPY_FILE_COPY_SYMLINK is NOT specified, this callback is called and hSourceFile is a handle to the *target* of the link!

    //file time handling:
    //::CopyFileEx() will copy file modification time (only) over from source file AFTER the last inocation of this callback
    //=> it is possible to adapt file creation time of target in here, but NOT file modification time!

    CallbackData& cbd = *static_cast<CallbackData*>(lpData);

    //#################### return source file attributes ################################
    if (dwCallbackReason == CALLBACK_STREAM_SWITCH) //called up front for every file (even if 0-sized)
    {
        BY_HANDLE_FILE_INFORMATION fileInfoSrc = {};
        if (!::GetFileInformationByHandle(hSourceFile, &fileInfoSrc))
        {
            cbd.reportError(_("Error reading file attributes:") + L"\n\"" + cbd.sourceFile_ + L"\"" + L"\n\n" + getLastErrorFormatted());
            return PROGRESS_CANCEL;
        }

        BY_HANDLE_FILE_INFORMATION fileInfoTrg = {};
        if (!::GetFileInformationByHandle(hDestinationFile, &fileInfoTrg))
        {
            cbd.reportError(_("Error reading file attributes:") + L"\n\"" + cbd.targetFile_ + L"\"" + L"\n\n" + getLastErrorFormatted());
            return PROGRESS_CANCEL;
        }

        FileAttrib attr;
        attr.fileSize         = UInt64(fileInfoSrc.nFileSizeLow, fileInfoSrc.nFileSizeHigh);
        attr.modificationTime = toTimeT(fileInfoSrc.ftLastWriteTime);
        attr.sourceFileId     = extractFileID(fileInfoSrc);
        attr.targetFileId     = extractFileID(fileInfoTrg);

        cbd.setNewAttr(attr);

        //#################### copy file creation time ################################
        FILETIME creationTime = {};

        if (!::GetFileTime(hSourceFile,   //__in       HANDLE hFile,
                           &creationTime, //__out_opt  LPFILETIME lpCreationTime,
                           NULL,          //__out_opt  LPFILETIME lpLastAccessTime,
                           NULL))         //__out_opt  LPFILETIME lpLastWriteTime
        {
            cbd.reportError(_("Error reading file attributes:") + L"\n\"" + cbd.sourceFile_ + L"\"" + L"\n\n" + getLastErrorFormatted());
            return PROGRESS_CANCEL;
        }

        if (!::SetFileTime(hDestinationFile,
                           &creationTime,
                           NULL,
                           NULL))
        {
            cbd.reportError(_("Error changing modification time:") + L"\n\"" + cbd.targetFile_ + L"\"" + L"\n\n" + getLastErrorFormatted());
            return PROGRESS_CANCEL;
        }
        //##############################################################################
    }

    //if (totalFileSize.QuadPart == totalBytesTransferred.QuadPart) {} //called after copy operation is finished - note: for 0-sized files this callback is invoked just ONCE!

    if (cbd.userCallback)
    {
        //some odd check for some possible(?) error condition
        if (totalBytesTransferred.QuadPart < 0) //let's see if someone answers the call...
            ::MessageBox(NULL, L"You've just discovered a bug in WIN32 API function \"CopyFileEx\"! \n\n\
            Please write a mail to the author of FreeFileSync at zhnmju123@gmx.de and simply state that\n\
            \"totalBytesTransferred.HighPart can be below zero\"!\n\n\
            This will then be handled in future versions of FreeFileSync.\n\nThanks -ZenJu",
                         NULL, 0);
        try
        {
            cbd.userCallback->updateCopyStatus(UInt64(totalBytesTransferred.QuadPart));
        }
        catch (...)
        {
            //#warning migrate to std::exception_ptr when available

            cbd.reportUserException(UInt64(totalBytesTransferred.QuadPart));
            return PROGRESS_CANCEL;
        }
    }
    return PROGRESS_CONTINUE;
}


#ifndef COPY_FILE_ALLOW_DECRYPTED_DESTINATION
#define COPY_FILE_ALLOW_DECRYPTED_DESTINATION 0x00000008
#endif

void rawCopyWinApi_sub(const Zstring& sourceFile,
                       const Zstring& targetFile,
                       CallbackCopyFile* callback,
                       FileAttrib* newAttrib) //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked
{
    zen::ScopeGuard guardTarget = zen::makeGuard([&]() { removeFile(targetFile); }); //transactional behavior: guard just before starting copy, we don't trust ::CopyFileEx(), do we ;)

    DWORD copyFlags = COPY_FILE_FAIL_IF_EXISTS;

    //allow copying from encrypted to non-encrytped location
    static bool nonEncSupported = false;
    {
        static boost::once_flag initNonEncOnce = BOOST_ONCE_INIT; //caveat: function scope static initialization is not thread-safe in VS 2010!
        boost::call_once(initNonEncOnce, []() { nonEncSupported = winXpOrLater(); }); //encrypted destination is not supported with Windows 2000
    }
    if (nonEncSupported)
        copyFlags |= COPY_FILE_ALLOW_DECRYPTED_DESTINATION;

    static bool osIsvistaOrLater = false;
    {
        static boost::once_flag initVistaLaterOnce = BOOST_ONCE_INIT; //caveat: function scope static initialization is not thread-safe in VS 2010!
        boost::call_once(initVistaLaterOnce, []() { osIsvistaOrLater = vistaOrLater(); });
    }

    CallbackData cbd(callback, sourceFile, targetFile, osIsvistaOrLater);

    const bool success = ::CopyFileEx( //same performance like CopyFile()
                             applyLongPathPrefix(sourceFile).c_str(),
                             applyLongPathPrefix(targetFile).c_str(),
                             copyCallbackInternal,
                             &cbd,
                             NULL,
                             copyFlags) == TRUE; //silence x64 perf warning

    cbd.evaluateErrors(); //throw ?, process errors in callback first!
    if (!success)
    {
        const DWORD lastError = ::GetLastError();

        //don't suppress "lastError == ERROR_REQUEST_ABORTED": a user aborted operation IS an error condition!

        //assemble error message...
        std::wstring errorMessage = _("Error copying file:") + L"\n\"" + sourceFile + L"\" ->\n\"" + targetFile + L"\"" +
                                    L"\n\n" + getLastErrorFormatted(lastError);

        //if file is locked (try to) use Windows Volume Shadow Copy Service
        if (lastError == ERROR_SHARING_VIOLATION ||
            lastError == ERROR_LOCK_VIOLATION)
            throw ErrorFileLocked(errorMessage);

        if (lastError == ERROR_FILE_EXISTS) //if target is existing this functions is expected to throw ErrorTargetExisting!!!
        {
            guardTarget.dismiss(); //don't delete file that existed previously!
            throw ErrorTargetExisting(errorMessage);
        }

        if (lastError == ERROR_PATH_NOT_FOUND)
        {
            guardTarget.dismiss(); //not relevant
            throw ErrorTargetPathMissing(errorMessage);
        }

        try //add more meaningful message
        {
            //trying to copy > 4GB file to FAT/FAT32 volume gives obscure ERROR_INVALID_PARAMETER (FAT can indeed handle files up to 4 Gig, tested!)
            if (lastError == ERROR_INVALID_PARAMETER &&
                dst::isFatDrive(targetFile) &&
                getFilesize(sourceFile) >= 4U * UInt64(1024U * 1024 * 1024)) //throw FileError
                errorMessage += L"\nFAT volume cannot store files larger than 4 gigabyte!";

            //note: ERROR_INVALID_PARAMETER can also occur when copying to a SharePoint server or MS SkyDrive and the target filename is of a restricted type.
        }
        catch (...) {}

        throw FileError(errorMessage);
    }

    if (newAttrib)
        *newAttrib = cbd.getSrcAttr();

    {
        const Int64 modTime = getFileTime(sourceFile, SYMLINK_FOLLOW); //throw FileError
        setFileTime(targetFile, modTime, SYMLINK_FOLLOW); //throw FileError
        //note: this sequence leads to a loss of precision of up to 1 sec!
        //perf-loss on USB sticks with many small files of about 30%! damn!
    }

    guardTarget.dismiss(); //target has been created successfully!
}


inline
void rawCopyWinApi(const Zstring& sourceFile,
                   const Zstring& targetFile,
                   CallbackCopyFile* callback,
                   FileAttrib* sourceAttr) //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked
{
    try
    {
        rawCopyWinApi_sub(sourceFile, targetFile, callback, sourceAttr); // throw ...
    }
    catch (ErrorTargetExisting&)
    {
        //try to handle issues with already existing short 8.3 file names on Windows
        if (have8dot3NameClash(targetFile))
        {
            Fix8Dot3NameClash dummy(targetFile); //move clashing filename to the side
            rawCopyWinApi_sub(sourceFile, targetFile, callback, sourceAttr); //throw FileError; the short filename name clash is solved, this should work now
            return;
        }
        throw;
    }
}

//void rawCopyWinOptimized(const Zstring& sourceFile,
//                         const Zstring& targetFile,
//                         CallbackCopyFile* callback) //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked
//{
//    /*
//                BackupRead()	FileRead()		CopyFileEx()
//    			--------------------------------------------
//    Attributes  NO			    NO              YES
//    create time NO              NO              NO
//    ADS			YES				NO				YES
//    Encrypted	NO(silent fail)	NO				YES
//    Compressed	NO				NO				NO
//    Sparse		YES				NO				NO
//    PERF		    6% faster		            -
//
//    Mark stream as compressed: FSCTL_SET_COMPRESSION
//        compatible with: BackupRead() FileRead()
//    */
//
//    //open sourceFile for reading
//    HANDLE hFileIn = ::CreateFile(applyLongPathPrefix(sourceFile).c_str(),
//                                  GENERIC_READ,
//                                  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, //all shared modes are required to read files that are open in other applications
//                                  0,
//                                  OPEN_EXISTING,
//                                  FILE_FLAG_SEQUENTIAL_SCAN,
//                                  NULL);
//    if (hFileIn == INVALID_HANDLE_VALUE)
//    {
//        const DWORD lastError = ::GetLastError();
//        const std::wstring& errorMessage = _("Error opening file:") + "\n\"" + sourceFile + "\"" + "\n\n" + getLastErrorFormatted(lastError);
//
//        //if file is locked (try to) use Windows Volume Shadow Copy Service
//        if (lastError == ERROR_SHARING_VIOLATION ||
//            lastError == ERROR_LOCK_VIOLATION)
//            throw ErrorFileLocked(errorMessage);
//
//        throw FileError(errorMessage);
//    }
//    ZEN_ON_BLOCK_EXIT(::CloseHandle, hFileIn);
//
//
//    BY_HANDLE_FILE_INFORMATION infoFileIn = {};
//    if (!::GetFileInformationByHandle(hFileIn, &infoFileIn))
//        throw FileError(_("Error reading file attributes:") + "\n\"" + sourceFile + "\"" + "\n\n" + getLastErrorFormatted());
//
//    //####################################### DST hack ###########################################
//    if (dst::isFatDrive(sourceFile)) //throw()
//    {
//        const dst::RawTime rawTime(infoFileIn.ftCreationTime, infoFileIn.ftLastWriteTime);
//        if (dst::fatHasUtcEncoded(rawTime)) //throw (std::runtime_error)
//        {
//            infoFileIn.ftLastWriteTime = dst::fatDecodeUtcTime(rawTime); //return last write time in real UTC, throw (std::runtime_error)
//            ::GetSystemTimeAsFileTime(&infoFileIn.ftCreationTime);       //real creation time information is not available...
//        }
//    }
//
//    if (dst::isFatDrive(targetFile)) //throw()
//    {
//        const dst::RawTime encodedTime = dst::fatEncodeUtcTime(infoFileIn.ftLastWriteTime); //throw (std::runtime_error)
//        infoFileIn.ftCreationTime  = encodedTime.createTimeRaw;
//        infoFileIn.ftLastWriteTime = encodedTime.writeTimeRaw;
//    }
//    //####################################### DST hack ###########################################
//
//    const DWORD validAttribs = FILE_ATTRIBUTE_READONLY |
//                               FILE_ATTRIBUTE_HIDDEN   |
//                               FILE_ATTRIBUTE_SYSTEM   |
//                               FILE_ATTRIBUTE_ARCHIVE  |            //those two are not set properly (not worse than ::CopyFileEx())
//                               FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | //
//                               FILE_ATTRIBUTE_ENCRYPTED;
//
//    //create targetFile and open it for writing
//    HANDLE hFileOut = ::CreateFile(applyLongPathPrefix(targetFile).c_str(),
//                                   GENERIC_READ | GENERIC_WRITE, //read access required for FSCTL_SET_COMPRESSION
//                                   FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
//                                   0,
//                                   CREATE_NEW,
//                                   (infoFileIn.dwFileAttributes & validAttribs) | FILE_FLAG_SEQUENTIAL_SCAN,
//                                   NULL);
//    if (hFileOut == INVALID_HANDLE_VALUE)
//    {
//        const DWORD lastError = ::GetLastError();
//        const std::wstring& errorMessage = _("Error writing file:") + "\n\"" + targetFile + "\"" +
//                                       "\n\n" + getLastErrorFormatted(lastError);
//
//        if (lastError == ERROR_FILE_EXISTS)
//            throw ErrorTargetExisting(errorMessage);
//
//        if (lastError == ERROR_PATH_NOT_FOUND)
//            throw ErrorTargetPathMissing(errorMessage);
//
//        throw FileError(errorMessage);
//    }
//    Loki::ScopeGuard guardTarget = Loki::MakeGuard(&removeFile, targetFile); //transactional behavior: guard just after opening target and before managing hFileOut
//
//    ZEN_ON_BLOCK_EXIT(::CloseHandle, hFileOut);
//
//
//#ifndef _MSC_VER
//#warning teste perf von GetVolumeInformationByHandleW
//#endif
//    DWORD fsFlags = 0;
//    if (!GetVolumeInformationByHandleW(hFileOut, //__in       HANDLE hFile,
//            NULL,     //__out_opt  LPTSTR lpVolumeNameBuffer,
//            0,        //__in       DWORD nVolumeNameSize,
//            NULL,     //__out_opt  LPDWORD lpVolumeSerialNumber,
//            NULL,     //__out_opt  LPDWORD lpMaximumComponentLength,
//            &fsFlags, //__out_opt  LPDWORD lpFileSystemFlags,
//            NULL,     //__out      LPTSTR lpFileSystemNameBuffer,
//            0))       //__in       DWORD nFileSystemNameSize
//        throw FileError(_("Error reading file attributes:") + "\n\"" + sourceFile + "\"" + "\n\n" + getLastErrorFormatted());
//
//	const bool sourceIsEncrypted  = (infoFileIn.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)   != 0;
//	const bool sourceIsCompressed = (infoFileIn.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)  != 0;
//    const bool sourceIsSparse     = (infoFileIn.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE) != 0;
//
//    bool targetSupportSparse     = (fsFlags & FILE_SUPPORTS_SPARSE_FILES) != 0;
//    bool targetSupportCompressed = (fsFlags & FILE_FILE_COMPRESSION     ) != 0;
//	bool targetSupportStreams    = (fsFlags & FILE_NAMED_STREAMS        ) != 0;
//
//
//    const bool useBackupFun = !sourceIsEncrypted; //http://msdn.microsoft.com/en-us/library/aa362509(v=VS.85).aspx
//
//    if (sourceIsCompressed && targetSupportCompressed)
//    {
//        USHORT cmpState = COMPRESSION_FORMAT_DEFAULT;
//
//        DWORD bytesReturned = 0;
//        if (!DeviceIoControl(hFileOut,              //handle to file or directory
//                             FSCTL_SET_COMPRESSION, //dwIoControlCode
//                             &cmpState,             //input buffer
//                             sizeof(cmpState),      //size of input buffer
//                             NULL,                  //lpOutBuffer
//                             0,                     //OutBufferSize
//                             &bytesReturned,        //number of bytes returned
//                             NULL))                 //OVERLAPPED structure
//            throw FileError(_("Error writing file:") + "\n\"" + targetFile + "\"" +
//                            "\n\n" + getLastErrorFormatted() +
//                            "\nFailed to write NTFS compressed attribute!");
//    }
//
//    //although it seems the sparse attribute is set automatically by BackupWrite, we are required to do this manually: http://support.microsoft.com/kb/271398/en-us
//    if (sourceIsSparse && targetSupportSparse)
//    {
//        if (useBackupFun)
//        {
//            DWORD bytesReturned = 0;
//            if (!DeviceIoControl(hFileOut,         //handle to file
//                                 FSCTL_SET_SPARSE, //dwIoControlCode
//                                 NULL,             //input buffer
//                                 0,                //size of input buffer
//                                 NULL,             //lpOutBuffer
//                                 0,                //OutBufferSize
//                                 &bytesReturned,   //number of bytes returned
//                                 NULL))            //OVERLAPPED structure
//                throw FileError(_("Error writing file:") + "\n\"" + targetFile + "\"" +
//                                "\n\n" + getLastErrorFormatted() +
//                                "\nFailed to write NTFS sparse attribute!");
//        }
//    }
//
//
//    const DWORD BUFFER_SIZE = 512 * 1024; //512 kb seems to be a reasonable buffer size
//        static boost::thread_specific_ptr<std::vector<char>> cpyBuf;
//        if (!cpyBuf.get())
//            cpyBuf.reset(new std::vector<char>(BUFFER_SIZE)); //512 kb seems to be a reasonable buffer size
//            std::vector<char>& buffer = *cpyBuf;
//
//    struct ManageCtxt //manage context for BackupRead()/BackupWrite()
//    {
//        ManageCtxt() : read(NULL), write(NULL) {}
//        ~ManageCtxt()
//        {
//            if (read != NULL)
//                ::BackupRead (0, NULL, 0, NULL, true, false, &read);
//            if (write != NULL)
//                ::BackupWrite(0, NULL, 0, NULL, true, false, &write);
//        }
//
//        LPVOID read;
//        LPVOID write;
//    } context;
//
//    //copy contents of sourceFile to targetFile
//    UInt64 totalBytesTransferred;
//
//    bool eof = false;
//    do
//    {
//        DWORD bytesRead = 0;
//
//        if (useBackupFun)
//        {
//            if (!::BackupRead(hFileIn,        //__in   HANDLE hFile,
//                              &buffer[0],   //__out  LPBYTE lpBuffer,
//                              BUFFER_SIZE,    //__in   DWORD nNumberOfBytesToRead,
//                              &bytesRead,     //__out  LPDWORD lpNumberOfBytesRead,
//                              false,          //__in   BOOL bAbort,
//                              false,          //__in   BOOL bProcessSecurity,
//                              &context.read)) //__out  LPVOID *lpContext
//                throw FileError(_("Error reading file:") + "\n\"" + sourceFile + "\"" +
//                                "\n\n" + getLastErrorFormatted());
//        }
//        else if (!::ReadFile(hFileIn,      //__in         HANDLE hFile,
//                             &buffer[0], //__out        LPVOID lpBuffer,
//                             BUFFER_SIZE,  //__in         DWORD nNumberOfBytesToRead,
//                             &bytesRead,   //__out_opt    LPDWORD lpNumberOfBytesRead,
//                             NULL))        //__inout_opt  LPOVERLAPPED lpOverlapped
//            throw FileError(_("Error reading file:") + "\n\"" + sourceFile + "\"" +
//                            "\n\n" + getLastErrorFormatted());
//
//        if (bytesRead > BUFFER_SIZE)
//            throw FileError(_("Error reading file:") + "\n\"" + sourceFile + "\"" +
//                            "\n\n" + "buffer overflow");
//
//        if (bytesRead < BUFFER_SIZE)
//            eof = true;
//
//        DWORD bytesWritten = 0;
//
//        if (useBackupFun)
//        {
//            if (!::BackupWrite(hFileOut,        //__in   HANDLE hFile,
//                               &buffer[0],    //__in   LPBYTE lpBuffer,
//                               bytesRead,       //__in   DWORD nNumberOfBytesToWrite,
//                               &bytesWritten,   //__out  LPDWORD lpNumberOfBytesWritten,
//                               false,           //__in   BOOL bAbort,
//                               false,           //__in   BOOL bProcessSecurity,
//                               &context.write)) //__out  LPVOID *lpContext
//                throw FileError(_("Error writing file:") + "\n\"" + targetFile + "\"" +
//                                "\n\n" + getLastErrorFormatted() + " (w)"); //w -> distinguish from fopen error message!
//        }
//        else if (!::WriteFile(hFileOut,      //__in         HANDLE hFile,
//                              &buffer[0],  //__out        LPVOID lpBuffer,
//                              bytesRead,     //__in         DWORD nNumberOfBytesToWrite,
//                              &bytesWritten, //__out_opt    LPDWORD lpNumberOfBytesWritten,
//                              NULL))         //__inout_opt  LPOVERLAPPED lpOverlapped
//            throw FileError(_("Error writing file:") + "\n\"" + targetFile + "\"" +
//                            "\n\n" + getLastErrorFormatted() + " (w)"); //w -> distinguish from fopen error message!
//
//        if (bytesWritten != bytesRead)
//            throw FileError(_("Error writing file:") + "\n\"" + targetFile + "\"" + "\n\n" + "incomplete write");
//
//        totalBytesTransferred += bytesRead;
//
//#ifndef _MSC_VER
//#warning totalBytesTransferred  kann größer als filesize sein!!
//#endif
//
//        //invoke callback method to update progress indicators
//        if (callback != NULL)
//            switch (callback->updateCopyStatus(totalBytesTransferred))
//            {
//                case CallbackCopyFile::CONTINUE:
//                    break;
//
//                case CallbackCopyFile::CANCEL: //a user aborted operation IS an error condition!
//                    throw FileError(_("Error copying file:") + "\n\"" + sourceFile + "\" ->\n\"" +
//                                    targetFile + "\"\n\n" + _("Operation aborted!"));
//            }
//    }
//    while (!eof);
//
//
//    if (totalBytesTransferred == 0) //BackupRead silently fails reading encrypted files -> double check!
//    {
//        LARGE_INTEGER inputSize = {};
//        if (!::GetFileSizeEx(hFileIn, &inputSize))
//            throw FileError(_("Error reading file attributes:") + "\n\"" + sourceFile + "\"" + "\n\n" + getLastErrorFormatted());
//
//        if (inputSize.QuadPart != 0)
//            throw FileError(_("Error reading file:") + "\n\"" + sourceFile + "\"" + "\n\n" + "unknown error");
//    }
//
//    //time needs to be set at the end: BackupWrite() changes file time
//    if (!::SetFileTime(hFileOut,
//                       &infoFileIn.ftCreationTime,
//                       NULL,
//                       &infoFileIn.ftLastWriteTime))
//        throw FileError(_("Error changing modification time:") + "\n\"" + targetFile + "\"" + "\n\n" + getLastErrorFormatted());
//
//
//#ifndef NDEBUG //dst hack: verify data written
//    if (dst::isFatDrive(targetFile)) //throw()
//    {
//        WIN32_FILE_ATTRIBUTE_DATA debugeAttr = {};
//        assert(::GetFileAttributesEx(applyLongPathPrefix(targetFile).c_str(), //__in   LPCTSTR lpFileName,
//                                     GetFileExInfoStandard,                   //__in   GET_FILEEX_INFO_LEVELS fInfoLevelId,
//                                     &debugeAttr));                           //__out  LPVOID lpFileInformation
//
//        assert(::CompareFileTime(&debugeAttr.ftCreationTime,  &infoFileIn.ftCreationTime)  == 0);
//        assert(::CompareFileTime(&debugeAttr.ftLastWriteTime, &infoFileIn.ftLastWriteTime) == 0);
//    }
//#endif
//
//    guardTarget.Dismiss();
//
//    /*
//        //create test sparse file
//        HANDLE hSparse = ::CreateFile(L"C:\\sparse.file",
//                                      GENERIC_READ | GENERIC_WRITE, //read access required for FSCTL_SET_COMPRESSION
//                                      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
//                                      0,
//                                      CREATE_NEW,
//                                      FILE_FLAG_SEQUENTIAL_SCAN,
//                                      NULL);
//        DWORD br = 0;
//        if (!::DeviceIoControl(hSparse, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &br,NULL))
//            throw 1;
//
//        LARGE_INTEGER liDistanceToMove =  {};
//        liDistanceToMove.QuadPart = 1024 * 1024 * 1024; //create 5 TB sparse file
//        liDistanceToMove.QuadPart *= 5 * 1024;          //
//        if (!::SetFilePointerEx(hSparse, liDistanceToMove, NULL, FILE_BEGIN))
//            throw 1;
//
//        if (!SetEndOfFile(hSparse))
//            throw 1;
//
//        FILE_ZERO_DATA_INFORMATION zeroInfo = {};
//        zeroInfo.BeyondFinalZero.QuadPart = liDistanceToMove.QuadPart;
//        if (!::DeviceIoControl(hSparse, FSCTL_SET_ZERO_DATA, &zeroInfo, sizeof(zeroInfo), NULL, 0, &br, NULL))
//            throw 1;
//
//        ::CloseHandle(hSparse);
//        */
//}
#endif

#ifdef FFS_LINUX
void rawCopyStream(const Zstring& sourceFile,
                   const Zstring& targetFile,
                   CallbackCopyFile* callback,
                   FileAttrib* newAttrib) //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting
{
    zen::ScopeGuard guardTarget = zen::makeGuard([&]() { removeFile(targetFile); }); //transactional behavior: place guard before lifetime of FileOutput
    try
    {
        //open sourceFile for reading
        FileInput fileIn(sourceFile); //throw FileError

        //create targetFile and open it for writing
        FileOutput fileOut(targetFile, FileOutput::ACC_CREATE_NEW); //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting

        static boost::thread_specific_ptr<std::vector<char>> cpyBuf;
        if (!cpyBuf.get())
            cpyBuf.reset(new std::vector<char>(512 * 1024)); //512 kb seems to be a reasonable buffer size
        std::vector<char>& buffer = *cpyBuf;

        //copy contents of sourceFile to targetFile
        UInt64 totalBytesTransferred;
        do
        {
            const size_t bytesRead = fileIn.read(&buffer[0], buffer.size()); //throw FileError

            fileOut.write(&buffer[0], bytesRead); //throw FileError

            totalBytesTransferred += bytesRead;

            //invoke callback method to update progress indicators
            if (callback)
                callback->updateCopyStatus(totalBytesTransferred);
        }
        while (!fileIn.eof());
    }
    catch (ErrorTargetExisting&)
    {
        guardTarget.dismiss(); //don't delete file that existed previously!
        throw;
    }

    //adapt file modification time:
    {
        struct ::stat srcInfo = {};
        if (::stat(sourceFile.c_str(), &srcInfo) != 0) //read file attributes from source directory
            throw FileError(_("Error reading file attributes:") + L"\n\"" + sourceFile + L"\"" + L"\n\n" + getLastErrorFormatted());

        struct ::utimbuf newTimes = {};
        newTimes.actime  = srcInfo.st_atime;
        newTimes.modtime = srcInfo.st_mtime;

        //set new "last write time"
        if (::utime(targetFile.c_str(), &newTimes) != 0)
            throw FileError(_("Error changing modification time:") + L"\n\"" + targetFile + L"\"" + L"\n\n" + getLastErrorFormatted());

        if (newAttrib)
        {
            struct ::stat trgInfo = {};
            if (::stat(targetFile.c_str(), &trgInfo) != 0) //read file attributes from source directory
                throw FileError(_("Error reading file attributes:") + L"\n\"" + targetFile + L"\"" + L"\n\n" + getLastErrorFormatted());

            newAttrib->fileSize         = UInt64(srcInfo.st_size);
            newAttrib->modificationTime = srcInfo.st_mtime;
            newAttrib->sourceFileId     = extractFileID(srcInfo);
            newAttrib->targetFileId     = extractFileID(trgInfo);
        }
    }

    guardTarget.dismiss(); //target has been created successfully!
}
#endif


inline
void copyFileImpl(const Zstring& sourceFile,
                  const Zstring& targetFile,
                  CallbackCopyFile* callback,
                  FileAttrib* sourceAttr) //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked
{
#ifdef FFS_WIN
    /*
                    rawCopyWinApi()	rawCopyWinOptimized()
        			-------------------------------------
        Attributes  YES			    YES
        Filetimes   YES			    YES
        ADS			YES			    YES
        Encrypted	YES			    YES
        Compressed	NO              YES
        Sparse		NO              YES
        PERF		-               6% faster
        SAMBA, ect. YES            UNKNOWN! -> issues writing ADS to Samba, issues reading from NAS, error copying files having "blocked" state... ect. damn!
    */

    rawCopyWinApi(sourceFile, targetFile, callback, sourceAttr);     //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked
    //rawCopyWinOptimized(sourceFile, targetFile, callback); //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked ->about 8% faster

#elif defined FFS_LINUX
    rawCopyStream(sourceFile, targetFile, callback, sourceAttr); //throw FileError, ErrorTargetPathMissing, ErrorTargetExisting
#endif
}
}


void zen::copyFile(const Zstring& sourceFile, //throw FileError, ErrorTargetPathMissing, ErrorFileLocked
                   const Zstring& targetFile,
                   bool copyFilePermissions,
                   bool transactionalCopy,
                   CallbackCopyFile* callback,
                   FileAttrib* sourceAttr)
{
    if (transactionalCopy)
    {
        Zstring temporary = targetFile + zen::TEMP_FILE_ENDING; //use temporary file until a correct date has been set
        zen::ScopeGuard guardTempFile = zen::makeGuard([&]() { removeFile(temporary); }); //transactional behavior: ensure cleanup (e.g. network drop) -> ref to temporary[!]

        //raw file copy
        try
        {
            copyFileImpl(sourceFile, temporary, callback, sourceAttr); //throw FileError: ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked
        }
        catch (ErrorTargetExisting&)
        {
            //determine non-used temp file name "first":
            //using optimistic strategy: assume everything goes well, but recover on error -> minimize file accesses
            temporary = createTempName(targetFile);

            //retry
            copyFileImpl(sourceFile, temporary, callback, sourceAttr); //throw FileError
        }

        //have target file deleted (after read access on source and target has been confirmed) => allow for almost transactional overwrite
        if (callback)
            callback->deleteTargetFile(targetFile);

        //rename temporary file:
        //perf: this call is REALLY expensive on unbuffered volumes! ~40% performance decrease on FAT USB stick!
        renameFile(temporary, targetFile); //throw FileError

        guardTempFile.dismiss();
    }
    else
    {
        //have target file deleted
        if (callback) callback->deleteTargetFile(targetFile);

        copyFileImpl(sourceFile, targetFile, callback, sourceAttr); //throw FileError: ErrorTargetPathMissing, ErrorTargetExisting, ErrorFileLocked
    }
    /*
       Note: non-transactional file copy solves at least four problems:
        	-> skydrive - doesn't allow for .ffs_tmp extension and returns ERROR_INVALID_PARAMETER
        	-> network renaming issues
        	-> allow for true delete before copy to handle low disk space problems
        	-> higher performance on non-buffered drives (e.g. usb sticks)
    */

    //set file permissions
    if (copyFilePermissions)
    {
        zen::ScopeGuard guardTargetFile = zen::makeGuard([&]() { removeFile(targetFile);});
        copyObjectPermissions(sourceFile, targetFile, SYMLINK_FOLLOW); //throw FileError


        /*
                warn_static("fix!!")
                try
                {
                    copyObjectPermissions(targetFile, L"kfsdaj", SYMLINK_FOLLOW); //throw FileError
                }
                catch (...) {}
        */

        guardTargetFile.dismiss(); //target has been created successfully!
    }
}
