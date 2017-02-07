// *****************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under    *
// * GNU General Public License: http://www.gnu.org/licenses/gpl-3.0           *
// * Copyright (C) Zenju (zenju AT freefilesync DOT org) - All Rights Reserved *
// *****************************************************************************

#include "file_io.h"
#include "file_access.h"

    #include <sys/stat.h>
    #include <fcntl.h>  //open, close
    #include <unistd.h> //read, write

using namespace zen;


namespace
{
//- "filePath" could be a named pipe which *blocks* forever for open()!
//- open() with O_NONBLOCK avoids the block, but opens successfully
//- create sample pipe: "sudo mkfifo named_pipe"
void checkForUnsupportedType(const Zstring& filePath) //throw FileError
{
    struct ::stat fileInfo = {};
    if (::stat(filePath.c_str(), &fileInfo) != 0) //follows symlinks
        return; //let the caller handle errors like "not existing"

    if (!S_ISREG(fileInfo.st_mode) &&
        !S_ISLNK(fileInfo.st_mode) &&
        !S_ISDIR(fileInfo.st_mode))
    {
        auto getTypeName = [](mode_t m) -> std::wstring
        {
            const wchar_t* name =
            S_ISCHR (m) ? L"character device":
            S_ISBLK (m) ? L"block device" :
            S_ISFIFO(m) ? L"FIFO, named pipe" :
            S_ISSOCK(m) ? L"socket" : nullptr;
            const std::wstring numFmt = printNumber<std::wstring>(L"0%06o", m & S_IFMT);
            return name ? numFmt + L", " + name : numFmt;
        };
        throw FileError(replaceCpy(_("Type of item %x is not supported:"), L"%x", fmtPath(filePath)) + L" " + getTypeName(fileInfo.st_mode));
    }
}
}


    const FileBase::FileHandle FileBase::invalidHandleValue = -1;


FileBase::~FileBase()
{
    if (fileHandle_ != invalidHandleValue)
        try
        {
            close(); //throw FileError
        }
        catch (FileError&) { assert(false); }
}


void FileBase::close() //throw FileError
{
    if (fileHandle_ == invalidHandleValue)
        throw FileError(replaceCpy(_("Cannot write file %x."), L"%x", fmtPath(getFilePath())), L"Contract error: close() called more than once.");
    ZEN_ON_SCOPE_EXIT(fileHandle_ = invalidHandleValue);

    //no need to clean-up on failure here (just like there is no clean on FileOutput::write failure!) => FileOutput is not transactional!

    if (::close(fileHandle_) != 0)
        THROW_LAST_FILE_ERROR(replaceCpy(_("Cannot write file %x."), L"%x", fmtPath(getFilePath())), L"close");
}

//----------------------------------------------------------------------------------------------------

namespace
{
FileBase::FileHandle openHandleForRead(const Zstring& filePath) //throw FileError, ErrorFileLocked
{
    checkForUnsupportedType(filePath); //throw FileError; opening a named pipe would block forever!

    //don't use O_DIRECT: http://yarchive.net/comp/linux/o_direct.html
    const FileBase::FileHandle fileHandle = ::open(filePath.c_str(), O_RDONLY);
    if (fileHandle == -1) //don't check "< 0" -> docu seems to allow "-2" to be a valid file handle
        THROW_LAST_FILE_ERROR(replaceCpy(_("Cannot open file %x."), L"%x", fmtPath(filePath)), L"open");
    return fileHandle; //pass ownership
}
}


FileInput::FileInput(FileHandle handle, const Zstring& filePath, const IOCallback& notifyUnbufferedIO) :
    FileBase(handle, filePath), notifyUnbufferedIO_(notifyUnbufferedIO) {}


FileInput::FileInput(const Zstring& filePath, const IOCallback& notifyUnbufferedIO) :
    FileBase(openHandleForRead(filePath), filePath), //throw FileError, ErrorFileLocked
    notifyUnbufferedIO_(notifyUnbufferedIO)
{
    //optimize read-ahead on input file:
    if (::posix_fadvise(getHandle(), 0, 0, POSIX_FADV_SEQUENTIAL) != 0)
        THROW_LAST_FILE_ERROR(replaceCpy(_("Cannot read file %x."), L"%x", fmtPath(filePath)), L"posix_fadvise");

}


size_t FileInput::tryRead(void* buffer, size_t bytesToRead) //throw FileError; may return short, only 0 means EOF!
{
    if (bytesToRead == 0) //"read() with a count of 0 returns zero" => indistinguishable from end of file! => check!
        throw std::logic_error("Contract violation! " + std::string(__FILE__) + ":" + numberTo<std::string>(__LINE__));
    assert(bytesToRead == getBlockSize());

    ssize_t bytesRead = 0;
    do
    {
        bytesRead = ::read(getHandle(), buffer, bytesToRead);
    }
    while (bytesRead < 0 && errno == EINTR); //Compare copy_reg() in copy.c: ftp://ftp.gnu.org/gnu/coreutils/coreutils-8.23.tar.xz

    if (bytesRead < 0)
        THROW_LAST_FILE_ERROR(replaceCpy(_("Cannot read file %x."), L"%x", fmtPath(getFilePath())), L"read");
    if (static_cast<size_t>(bytesRead) > bytesToRead) //better safe than sorry
        throw FileError(replaceCpy(_("Cannot read file %x."), L"%x", fmtPath(getFilePath())), L"ReadFile: buffer overflow."); //user should never see this

    //if ::read is interrupted (EINTR) right in the middle, it will return successfully with "bytesRead < bytesToRead"

    return bytesRead; //"zero indicates end of file"
}


size_t FileInput::read(void* buffer, size_t bytesToRead) //throw FileError, X; return "bytesToRead" bytes unless end of stream!
{
    const size_t blockSize = getBlockSize();

    while (memBuf_.size() < bytesToRead)
    {
        memBuf_.resize(memBuf_.size() + blockSize);
        const size_t bytesRead = tryRead(&*(memBuf_.end() - blockSize), blockSize); //throw FileError; may return short, only 0 means EOF! => CONTRACT: bytesToRead > 0
        memBuf_.resize(memBuf_.size() - blockSize + bytesRead); //caveat: unsigned arithmetics

        if (notifyUnbufferedIO_) notifyUnbufferedIO_(bytesRead); //throw X

        if (bytesRead == 0) //end of file
            bytesToRead = memBuf_.size();
    }

    std::copy(memBuf_.begin(), memBuf_.begin() + bytesToRead, static_cast<char*>(buffer));
    memBuf_.erase(memBuf_.begin(), memBuf_.begin() + bytesToRead);
    return bytesToRead;
}

//----------------------------------------------------------------------------------------------------

namespace
{
FileBase::FileHandle openHandleForWrite(const Zstring& filePath, FileOutput::AccessFlag access) //throw FileError, ErrorTargetExisting
{
    //checkForUnsupportedType(filePath); -> not needed, open() + O_WRONLY should fail fast

    const FileBase::FileHandle fileHandle = ::open(filePath.c_str(), O_WRONLY | O_CREAT | (access == FileOutput::ACC_CREATE_NEW ? O_EXCL : O_TRUNC),
                                                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); //0666
    if (fileHandle == -1)
    {
        const int ec = errno; //copy before making other system calls!
        const std::wstring errorMsg = replaceCpy(_("Cannot write file %x."), L"%x", fmtPath(filePath));
        const std::wstring errorDescr = formatSystemError(L"open", ec);

        if (ec == EEXIST)
            throw ErrorTargetExisting(errorMsg, errorDescr);
        //if (ec == ENOENT) throw ErrorTargetPathMissing(errorMsg, errorDescr);

        throw FileError(errorMsg, errorDescr);
    }
    return fileHandle; //pass ownership
}
}


FileOutput::FileOutput(FileHandle handle, const Zstring& filePath, const IOCallback& notifyUnbufferedIO) :
    FileBase(handle, filePath), notifyUnbufferedIO_(notifyUnbufferedIO) {}


FileOutput::FileOutput(const Zstring& filePath, AccessFlag access, const IOCallback& notifyUnbufferedIO) :
    FileBase(openHandleForWrite(filePath, access), filePath), notifyUnbufferedIO_(notifyUnbufferedIO) {} //throw FileError, ErrorTargetExisting


FileOutput::~FileOutput()
{
    try
    {
        flushBuffers(); //throw FileError, X
    }
    catch (...) { assert(false); }
}


size_t FileOutput::tryWrite(const void* buffer, size_t bytesToWrite) //throw FileError; may return short! CONTRACT: bytesToWrite > 0
{
    if (bytesToWrite == 0)
        throw std::logic_error("Contract violation! " + std::string(__FILE__) + ":" + numberTo<std::string>(__LINE__));
    assert(bytesToWrite <= getBlockSize());

    ssize_t bytesWritten = 0;
    do
    {
        bytesWritten = ::write(getHandle(), buffer, bytesToWrite);
    }
    while (bytesWritten < 0 && errno == EINTR);

    if (bytesWritten <= 0)
    {
        if (bytesWritten == 0) //comment in safe-read.c suggests to treat this as an error due to buggy drivers
            errno = ENOSPC;

        THROW_LAST_FILE_ERROR(replaceCpy(_("Cannot write file %x."), L"%x", fmtPath(getFilePath())), L"write");
    }
    if (bytesWritten > static_cast<ssize_t>(bytesToWrite)) //better safe than sorry
        throw FileError(replaceCpy(_("Cannot write file %x."), L"%x", fmtPath(getFilePath())), L"write: buffer overflow."); //user should never see this

    //if ::write() is interrupted (EINTR) right in the middle, it will return successfully with "bytesWritten < bytesToWrite"!
    return bytesWritten;
}


void FileOutput::write(const void* buffer, size_t bytesToWrite) //throw FileError, X
{
    auto bufFirst = static_cast<const char*>(buffer);
    memBuf_.insert(memBuf_.end(), bufFirst, bufFirst + bytesToWrite);

    const size_t blockSize = getBlockSize();
    size_t bytesRemaining = memBuf_.size();
    ZEN_ON_SCOPE_EXIT(memBuf_.erase(memBuf_.begin(), memBuf_.end() - bytesRemaining));
    while (bytesRemaining >= blockSize)
    {
        const size_t bytesWritten = tryWrite(&*(memBuf_.end() - bytesRemaining), blockSize); //throw FileError; may return short! CONTRACT: bytesToWrite > 0
        bytesRemaining -= bytesWritten;
        if (notifyUnbufferedIO_) notifyUnbufferedIO_(bytesWritten); //throw X!
    }
}


void FileOutput::flushBuffers() //throw FileError, X
{
    size_t bytesRemaining = memBuf_.size();
    ZEN_ON_SCOPE_EXIT(memBuf_.erase(memBuf_.begin(), memBuf_.end() - bytesRemaining));
    while (bytesRemaining > 0)
    {
        const size_t bytesWritten = tryWrite(&*(memBuf_.end() - bytesRemaining), bytesRemaining); //throw FileError; may return short! CONTRACT: bytesToWrite > 0
        bytesRemaining -= bytesWritten;
        if (notifyUnbufferedIO_) notifyUnbufferedIO_(bytesWritten); //throw X!
    }
}


void FileOutput::finalize() //throw FileError, X
{
    flushBuffers(); //throw FileError, X
    //~FileBase() calls this one, too, but we want to propagate errors if any:
    close(); //throw FileError
}


void FileOutput::preAllocateSpaceBestEffort(uint64_t expectedSize) //throw FileError
{
    const FileHandle fh = getHandle();
    //don't use potentially inefficient ::posix_fallocate!
    const int rv = ::fallocate(fh,            //int fd,
                               0,             //int mode,
                               0,             //off_t offset
                               expectedSize); //off_t len
    if (rv != 0)
        return; //may fail with EOPNOTSUPP, unlike posix_fallocate

}
