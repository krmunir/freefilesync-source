#include "fileHandling.h"
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include "../algorithm.h"
#include <wx/filename.h>

#ifdef FFS_WIN
#include <wx/msw/wrapwin.h> //includes "windows.h"
#endif  // FFS_WIN


class RecycleBin
{
public:
    RecycleBin() :
            recycleBinAvailable(false)
    {
#ifdef FFS_WIN
        recycleBinAvailable = true;
#endif  // FFS_WIN
    }

    ~RecycleBin() {}

    bool recycleBinExists()
    {
        return recycleBinAvailable;
    }

    bool moveToRecycleBin(const Zstring& filename)
    {
        if (!recycleBinAvailable)   //this method should ONLY be called if recycle bin is available
            throw RuntimeException(_("Initialization of Recycle Bin failed!"));

#ifdef FFS_WIN
        Zstring filenameDoubleNull = filename + wxChar(0);

        SHFILEOPSTRUCT fileOp;
        fileOp.hwnd   = NULL;
        fileOp.wFunc  = FO_DELETE;
        fileOp.pFrom  = filenameDoubleNull.c_str();
        fileOp.pTo    = NULL;
        fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
        fileOp.fAnyOperationsAborted = false;
        fileOp.hNameMappings         = NULL;
        fileOp.lpszProgressTitle     = NULL;

        if (SHFileOperation(&fileOp   //pointer to an SHFILEOPSTRUCT structure that contains information the function needs to carry out
                           ) != 0 || fileOp.fAnyOperationsAborted) return false;
#else
        assert(false);
#endif

        return true;
    }

private:
    bool recycleBinAvailable;
};

//global instance of recycle bin
RecycleBin recyclerInstance;


bool FreeFileSync::recycleBinExists()
{
    return recyclerInstance.recycleBinExists();
}


inline
bool moveToRecycleBin(const Zstring& filename) throw(RuntimeException)
{
    return recyclerInstance.moveToRecycleBin(filename);
}


inline
void FreeFileSync::removeFile(const Zstring& filename, const bool useRecycleBin)
{
    if (!wxFileExists(filename.c_str())) return; //this is NOT an error situation: the manual deletion relies on it!

    if (useRecycleBin)
    {
        if (!moveToRecycleBin(filename))
            throw FileError(Zstring(_("Error moving to Recycle Bin:")) + wxT("\n\"") + filename + wxT("\""));
        return;
    }

#ifdef FFS_WIN
    //initialize file attributes
    if (!SetFileAttributes(
                filename.c_str(),       //address of filename
                FILE_ATTRIBUTE_NORMAL)) //attributes to set
    {
        Zstring errorMessage = Zstring(_("Error deleting file:")) + wxT("\n\"") + filename + wxT("\"");
        throw FileError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted());
    }

    //remove file, support for \\?\-prefix
    if (DeleteFile(filename.c_str()) == 0)
    {
        Zstring errorMessage = Zstring(_("Error deleting file:")) + wxT("\n\"") + filename + wxT("\"");
        throw FileError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted());
    }
#else
    if (!wxRemoveFile(filename.c_str()))
        throw FileError(Zstring(_("Error deleting file:")) + wxT("\n\"") + filename + wxT("\""));
#endif
}


void FreeFileSync::removeDirectory(const Zstring& directory, const bool useRecycleBin)
{
    if (!wxDirExists(directory)) return; //this is NOT an error situation: the manual deletion relies on it!

    if (useRecycleBin)
    {
        if (!moveToRecycleBin(directory))
            throw FileError(Zstring(_("Error moving to Recycle Bin:")) + wxT("\n\"") + directory + wxT("\""));
        return;
    }

    std::vector<Zstring> fileList;
    std::vector<Zstring> dirList;

    getAllFilesAndDirs(directory, fileList, dirList);

    for (unsigned int j = 0; j < fileList.size(); ++j)
        removeFile(fileList[j], false);

    dirList.insert(dirList.begin(), directory);   //parent directory will be deleted last

    for (int j = int(dirList.size()) - 1; j >= 0 ; --j)
    {
#ifdef FFS_WIN
        //initialize file attributes
        if (!SetFileAttributes(
                    dirList[j].c_str(),     // address of directory name
                    FILE_ATTRIBUTE_NORMAL)) // attributes to set
        {
            Zstring errorMessage = Zstring(_("Error deleting directory:")) + wxT("\n\"") + dirList[j] + wxT("\"");
            throw FileError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted());
        }

        //remove directory, support for \\?\-prefix
        if (RemoveDirectory(dirList[j].c_str()) == 0)
        {
            Zstring errorMessage = Zstring(_("Error deleting directory:")) + wxT("\n\"") + dirList[j] + wxT("\"");
            throw FileError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted());
        }
#else
        if (!wxRmdir(dirList[j].c_str()))
            throw FileError(Zstring(_("Error deleting directory:")) + wxT("\n\"") + dirList[j] + wxT("\""));
#endif
    }
}


void FreeFileSync::createDirectory(const Zstring& directory, const int level)
{
    if (wxDirExists(directory.c_str()))
        return;

    if (level == 50) //catch endless recursion
        return;

    //try to create directory, support for \\?\-prefix
#ifdef FFS_WIN
    if (CreateDirectory(
                directory.c_str(),	// pointer to a directory path string
                NULL               	// pointer to a security descriptor
            ) != 0)
        return;
#else
    if (wxMkdir(directory.c_str())) //wxMkDir has different signature under Linux!
        return;
#endif

    //if not successfull try to create parent folders first
    Zstring parentDir;
    if (endsWithPathSeparator(directory)) //may be valid on first level of recursion at most! Usually never true!
    {
        parentDir = directory.BeforeLast(GlobalResources::FILE_NAME_SEPARATOR);
        parentDir = parentDir.BeforeLast(GlobalResources::FILE_NAME_SEPARATOR);
    }
    else
        parentDir = directory.BeforeLast(GlobalResources::FILE_NAME_SEPARATOR);

    if (parentDir.empty()) return;

    //call function recursively
    createDirectory(parentDir, level + 1);

    //now creation should be possible
#ifdef FFS_WIN
    if (CreateDirectory(
                directory.c_str(), // pointer to a directory path string
                NULL 	           // pointer to a security descriptor
            ) == 0)
    {
        if (level == 0)
        {
            Zstring errorMessage = Zstring(_("Error creating directory:")) + wxT("\n\"") + directory + wxT("\"");
            throw FileError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted());
        }
    }
#else
    if (!wxMkdir(directory.c_str())) //wxMkDir has different signature under Linux!
    {
        if (level == 0)
            throw FileError(Zstring(_("Error creating directory:")) + wxT("\n\"") + directory + wxT("\""));
    }
#endif
}


void FreeFileSync::copyFolderAttributes(const Zstring& source, const Zstring& target)
{
#ifdef FFS_WIN
    DWORD attr = GetFileAttributes(source.c_str()); // address of the name of a file or directory
    if (attr ==  0xFFFFFFFF)
    {
        Zstring errorMessage = Zstring(_("Error reading folder attributes:")) + wxT("\n\"") + source + wxT("\"");
        throw FileError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted());
    }

    if (!SetFileAttributes(
                target.c_str(), // address of filename
                attr))          // address of attributes to set
    {
        Zstring errorMessage = Zstring(_("Error writing folder attributes:")) + wxT("\n\"") + target + wxT("\"");
        throw FileError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted());
    }
#elif defined FFS_LINUX
//Linux doesn't use attributes for files or folders
#endif
}


class FilesDirsOnlyTraverser : public FreeFileSync::FullDetailFileTraverser
{
public:
    FilesDirsOnlyTraverser(std::vector<Zstring>& files, std::vector<Zstring>& dirs) :
            m_files(files),
            m_dirs(dirs) {}

    virtual wxDirTraverseResult OnFile(const Zstring& filename, const FreeFileSync::FileInfo& details)
    {
        m_files.push_back(filename);
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnDir(const Zstring& dirname)
    {
        m_dirs.push_back(dirname);
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnError(const Zstring& errorText)
    {
        throw FileError(errorText);
    }

private:
    std::vector<Zstring>& m_files;
    std::vector<Zstring>& m_dirs;
};


void FreeFileSync::getAllFilesAndDirs(const Zstring& sourceDir, std::vector<Zstring>& files, std::vector<Zstring>& directories) throw(FileError)
{
    files.clear();
    directories.clear();

    //get all files and directories from current directory (and subdirectories)
    FilesDirsOnlyTraverser traverser(files, directories);
    traverseInDetail(sourceDir, false, &traverser);
}


#ifdef FFS_WIN
class CloseOnExit
{
public:
    CloseOnExit(HANDLE searchHandle) : m_searchHandle(searchHandle) {}

    ~CloseOnExit()
    {
        FindClose(m_searchHandle);
    }

private:
    HANDLE m_searchHandle;
};


inline
void getWin32FileInformation(const WIN32_FIND_DATA& input, FreeFileSync::FileInfo& output)
{
    //convert UTC FILETIME to ANSI C format (number of seconds since Jan. 1st 1970 UTC)
    wxULongLong writeTimeLong(input.ftLastWriteTime.dwHighDateTime, input.ftLastWriteTime.dwLowDateTime);
    writeTimeLong /= 10000000;                     //reduce precision to 1 second (FILETIME has unit 10^-7 s)
    writeTimeLong -= wxULongLong(2, 3054539008UL); //timeshift between ansi C time and FILETIME in seconds == 11644473600s
    assert(writeTimeLong.GetHi() == 0);            //it should fit into a 32bit variable now
    output.lastWriteTimeRaw = writeTimeLong.GetLo();

    output.fileSize = wxULongLong(input.nFileSizeHigh, input.nFileSizeLow);
}

#elif defined FFS_LINUX
class EnhancedFileTraverser : public wxDirTraverser
{
public:
    EnhancedFileTraverser(FreeFileSync::FullDetailFileTraverser* sink) : m_sink(sink) {}

    wxDirTraverseResult OnFile(const wxString& filename) //virtual impl.
    {
        struct stat fileInfo;
        if (stat(filename.c_str(), &fileInfo) != 0)
            return m_sink->OnError(Zstring(_("Could not retrieve file info for:")) + wxT("\n\"") + filename.c_str() + wxT("\""));

        FreeFileSync::FileInfo details;
        details.lastWriteTimeRaw = fileInfo.st_mtime; //UTC time(ANSI C format); unit: 1 second
        details.fileSize         = fileInfo.st_size;

        return m_sink->OnFile(filename.c_str(), details);
    }

    wxDirTraverseResult OnDir(const wxString& dirname) //virtual impl.
    {
        return m_sink->OnDir(dirname.c_str());
    }

    wxDirTraverseResult OnOpenError(const wxString& errorText) //virtual impl.
    {
        return m_sink->OnError(errorText.c_str());
    }

private:
    FreeFileSync::FullDetailFileTraverser* m_sink;
};
#endif


class TraverseRecursively
{
public:
    TraverseRecursively(const bool traverseSymbolicLinks, FreeFileSync::FullDetailFileTraverser* sink) :
            m_traverseSymbolicLinks(traverseSymbolicLinks),
            m_sink(sink) {}

    bool traverse(const Zstring& directory, const int level)
    {
#ifdef FFS_WIN
        if (level == 50) //catch endless recursion
        {
            if (m_sink->OnError(Zstring(_("Error traversing directory:")) + wxT("\n\"") + directory + wxT("\"")) == wxDIR_STOP)
                return false;
            else
                return true;
        }

        Zstring directoryFormatted = directory;
        if (!FreeFileSync::endsWithPathSeparator(directoryFormatted))
            directoryFormatted += GlobalResources::FILE_NAME_SEPARATOR;

        const Zstring filespec = directoryFormatted + DefaultChar('*');

        WIN32_FIND_DATA fileMetaData;
        HANDLE searchHandle = FindFirstFile(filespec.c_str(), //pointer to name of file to search for
                                            &fileMetaData);   //pointer to returned information

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            const DWORD lastError = GetLastError();
            if (lastError == ERROR_FILE_NOT_FOUND)
                return true;

            //else: we have a problem... report it:
            Zstring errorMessage = Zstring(_("Error traversing directory:")) + wxT("\n\"") + directory + wxT("\" ") ;
            if (m_sink->OnError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted(lastError)) == wxDIR_STOP)
                return false;
            else
                return true;
        }
        CloseOnExit dummy(searchHandle);

        do
        {   //don't return "." and ".."
            const wxChar* name = fileMetaData.cFileName;
            if (    name[0] == wxChar('.') &&
                    ((name[1] == wxChar('.') && name[2] == wxChar('\0')) ||
                     name[1] == wxChar('\0')))
                continue;

            const Zstring fullName = directoryFormatted + name;

            if (fileMetaData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //a directory...
            {
                switch (m_sink->OnDir(fullName))
                {
                case wxDIR_IGNORE:
                    break;
                case wxDIR_CONTINUE:
                    //traverse into symbolic links, junctions, etc. if requested only:
                    if (m_traverseSymbolicLinks || (~fileMetaData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
                        if (!this->traverse(fullName, level + 1))
                            return false;
                    break;
                case wxDIR_STOP:
                    return false;
                default:
                    assert(false);
                    break;
                }
            }
            else //a file...
            {
                FreeFileSync::FileInfo details;
                getWin32FileInformation(fileMetaData, details);

                switch (m_sink->OnFile(fullName, details))
                {
                case wxDIR_IGNORE:
                case wxDIR_CONTINUE:
                    break;
                case wxDIR_STOP:
                    return false;
                default:
                    assert(false);
                    break;
                }
            }
        }
        while (FindNextFile(searchHandle,	 // handle to search
                            &fileMetaData)); // pointer to structure for data on found file

        const DWORD lastError = GetLastError();
        if (lastError == ERROR_NO_MORE_FILES)
            return true; //everything okay

        //else: we have a problem... report it:
        Zstring errorMessage = Zstring(_("Error traversing directory:")) + wxT("\n\"") + directory + wxT("\" ") ;
        if (m_sink->OnError(errorMessage + wxT("\n\n") + FreeFileSync::getLastErrorFormatted(lastError)) == wxDIR_STOP)
            return false;
        else
            return true;
#elif defined FFS_LINUX

        //use standard file traverser and enrich output with additional file information
        //could be improved with custom traversing algorithm for optimized performance
        EnhancedFileTraverser traverser(m_sink);

        wxDir dir(directory.c_str());
        if (dir.IsOpened())
            dir.Traverse(traverser);

        return true;
#else
        adapt this
#endif
    }

private:
    const bool m_traverseSymbolicLinks;
    FreeFileSync::FullDetailFileTraverser* m_sink;
};


void FreeFileSync::traverseInDetail(const Zstring& directory,
                                    const bool traverseSymbolicLinks,
                                    FullDetailFileTraverser* sink)
{
    TraverseRecursively filewalker(traverseSymbolicLinks, sink);
    filewalker.traverse(directory, 0);
}


#ifdef FFS_WIN
inline
Zstring getDriveName(const Zstring& directoryName) //GetVolume() doesn't work under Linux!
{
    const Zstring volumeName = wxFileName(directoryName.c_str()).GetVolume().c_str();
    if (volumeName.empty())
        return Zstring();

    return volumeName + wxFileName::GetVolumeSeparator().c_str() + GlobalResources::FILE_NAME_SEPARATOR;
}


bool FreeFileSync::isFatDrive(const Zstring& directoryName)
{
    const Zstring driveName = getDriveName(directoryName);
    if (driveName.empty())
        return false;

    wxChar fileSystem[32];
    if (!GetVolumeInformation(driveName.c_str(), NULL, 0, NULL, NULL, NULL, fileSystem, 32))
        return false;

    return Zstring(fileSystem).StartsWith(wxT("FAT"));
}
#endif  //FFS_WIN
