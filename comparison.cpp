#include "comparison.h"
#include "library/globalFunctions.h"
#include <wx/intl.h>
#include "library/globalFunctions.h"
#include <wx/ffile.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include "library/multithreading.h"
#include "algorithm.h"
#include <wx/thread.h>
#include <memory>

#ifdef FFS_WIN
#include <wx/msw/wrapwin.h> //includes "windows.h"
#endif  //FFS_WIN

using namespace FreeFileSync;


class GetAllFilesFull : public FullDetailFileTraverser
{
public:
    GetAllFilesFull(DirectoryDescrType& output, Zstring dirThatIsSearched, StatusHandler* handler) :
            m_output(output),
            directory(dirThatIsSearched),
            statusHandler(handler)
    {
        prefixLength = directory.length();
        textScanning = Zstring(_("Scanning:")) + wxT(" \n"); //performance optimization
    }


    inline
    void writeText(const wxChar* text, const int length, wxChar*& currentPos)
    {
        memcpy(currentPos, text, length * sizeof(wxChar));
        currentPos+=length;
    }


    wxDirTraverseResult OnFile(const Zstring& fullFileName, const FileInfo& details) //virtual impl.
    {
        FileDescrLine fileDescr;
        fileDescr.fullName         = fullFileName;
        fileDescr.directory        = directory;
        fileDescr.relativeName     = fullFileName.zsubstr(prefixLength);
        fileDescr.lastWriteTimeRaw = details.lastWriteTimeRaw;
        fileDescr.fileSize         = details.fileSize;
        fileDescr.objType          = FileDescrLine::TYPE_FILE;
        m_output.push_back(fileDescr);

        //assemble status message (performance optimized)  = textScanning + wxT("\"") + fullFileName + wxT("\"")
        const unsigned int statusTextMaxLen = 2000;
        wxChar statusText[statusTextMaxLen];
        wxChar* position = statusText;
        if (textScanning.length() + fullFileName.length() + 2 < statusTextMaxLen) //leave room for 0 terminating char!
        {
            writeText(textScanning.c_str(), textScanning.length(), position);
            writeText(wxT("\""), 1, position);
            writeText(fullFileName.c_str(), fullFileName.length(), position);
            writeText(wxT("\""), 1, position);
        }
        *position = 0;

        //update UI/commandline status information
        statusHandler->updateStatusText(statusText);
        //add 1 element to the progress indicator
        statusHandler->updateProcessedData(1, 0); //NO performance issue at all
        //trigger display refresh
        statusHandler->requestUiRefresh();

        return wxDIR_CONTINUE;
    }


    wxDirTraverseResult OnDir(const Zstring& fullDirName) //virtual impl.
    {
#ifdef FFS_WIN
        if (    fullDirName.EndsWith(wxT("\\RECYCLER")) ||
                fullDirName.EndsWith(wxT("\\System Volume Information")))
            return wxDIR_IGNORE;
#endif  // FFS_WIN

        FileDescrLine fileDescr;
        fileDescr.fullName         = fullDirName;
        fileDescr.directory        = directory;
        fileDescr.relativeName     = fullDirName.zsubstr(prefixLength);
        fileDescr.lastWriteTimeRaw = 0;  //irrelevant for directories
        fileDescr.fileSize         = wxULongLong(0);  //currently used by getBytesToTransfer
        fileDescr.objType          = FileDescrLine::TYPE_DIRECTORY;
        m_output.push_back(fileDescr);

        //assemble status message (performance optimized)  = textScanning + wxT("\"") + fullDirName + wxT("\"")
        const unsigned int statusTextMaxLen = 2000;
        wxChar statusText[statusTextMaxLen];
        wxChar* position = statusText;
        if (textScanning.length() + fullDirName.length() + 2 < statusTextMaxLen) //leave room for 0 terminating char!
        {
            writeText(textScanning.c_str(), textScanning.length(), position);
            writeText(wxT("\""), 1, position);
            writeText(fullDirName.c_str(), fullDirName.length(), position);
            writeText(wxT("\""), 1, position);
        }
        *position = 0;

        //update UI/commandline status information
        statusHandler->updateStatusText(statusText);
        //add 1 element to the progress indicator
        statusHandler->updateProcessedData(1, 0);     //NO performance issue at all
        //trigger display refresh
        statusHandler->requestUiRefresh();

        return wxDIR_CONTINUE;
    }


    wxDirTraverseResult OnError(const Zstring& errorText) //virtual impl.
    {
        while (true)
        {
            ErrorHandler::Response rv = statusHandler->reportError(errorText);
            if (rv == ErrorHandler::IGNORE_ERROR)
                return wxDIR_CONTINUE;
            else if (rv == ErrorHandler::RETRY)
                ;   //I have to admit "retry" is a bit of a fake here... at least the user has opportunity to abort!
            else
            {
                assert (false);
                return wxDIR_CONTINUE;
            }
        }

        return wxDIR_CONTINUE;
    }

private:
    DirectoryDescrType& m_output;
    Zstring directory;
    int prefixLength;
    Zstring textScanning;
    StatusHandler* statusHandler;
};


struct DescrBufferLine
{
    Zstring directoryName;
    DirectoryDescrType* directoryDesc;

    bool operator < (const DescrBufferLine& b) const
    {
#ifdef FFS_WIN //Windows does NOT distinguish between upper/lower-case
        return (directoryName.CmpNoCase(b.directoryName) < 0);
#elif defined FFS_LINUX //Linux DOES distinguish between upper/lower-case
        return (directoryName.Cmp(b.directoryName) < 0);
#endif
    }
};


class FreeFileSync::DirectoryDescrBuffer  //buffer multiple scans of the same directories
{
public:
    DirectoryDescrBuffer(const bool traverseDirectorySymlinks, StatusHandler* statusUpdater) :
            m_traverseDirectorySymlinks(traverseDirectorySymlinks),
            m_statusUpdater(statusUpdater) {}

    ~DirectoryDescrBuffer()
    {
        //clean up
        for (std::set<DescrBufferLine>::iterator i = buffer.begin(); i != buffer.end(); ++i)
            delete i->directoryDesc;
    }

    DirectoryDescrType* getDirectoryDescription(const Zstring& directoryFormatted)
    {
        DescrBufferLine bufferEntry;
        bufferEntry.directoryName = directoryFormatted;

        std::set<DescrBufferLine>::iterator entryFound = buffer.find(bufferEntry);
        if (entryFound != buffer.end())
        {
            //entry found in buffer; return
            return entryFound->directoryDesc;
        }
        else
        {
            //entry not found; create new one
            bufferEntry.directoryDesc = new DirectoryDescrType;
            buffer.insert(bufferEntry); //exception safety: insert into buffer right after creation!

            //get all files and folders from directoryFormatted (and subdirectories)
            GetAllFilesFull traverser(*bufferEntry.directoryDesc, directoryFormatted, m_statusUpdater); //exceptions may be thrown!
            traverseInDetail(directoryFormatted, m_traverseDirectorySymlinks, &traverser);

            return bufferEntry.directoryDesc;
        }
    }

private:
    std::set<DescrBufferLine> buffer;

    const bool m_traverseDirectorySymlinks;
    StatusHandler* m_statusUpdater;
};


bool foldersAreValidForComparison(const std::vector<FolderPair>& folderPairs, wxString& errorMessage)
{
    errorMessage.Clear();

    for (std::vector<FolderPair>::const_iterator i = folderPairs.begin(); i != folderPairs.end(); ++i)
    {
        const Zstring leftFolderName  = getFormattedDirectoryName(i->leftDirectory);
        const Zstring rightFolderName = getFormattedDirectoryName(i->rightDirectory);

        //check if folder name is empty
        if (leftFolderName.empty() || rightFolderName.empty())
        {
            errorMessage = _("Please fill all empty directory fields.");
            return false;
        }

        //check if folder exists
        if (!wxDirExists(leftFolderName))
        {
            errorMessage = wxString(_("Directory does not exist:")) + wxT(" \"") + leftFolderName + wxT("\"");
            return false;
        }
        if (!wxDirExists(rightFolderName))
        {
            errorMessage = wxString(_("Directory does not exist:")) + wxT(" \"") + rightFolderName + wxT("\"");
            return false;
        }
    }
    return true;
}


bool dependencyExists(const std::vector<Zstring>& folders, const Zstring& newFolder, wxString& warningMessage)
{
    warningMessage.Clear();

    for (std::vector<Zstring>::const_iterator i = folders.begin(); i != folders.end(); ++i)
        if (newFolder.StartsWith(*i) || i->StartsWith(newFolder))
        {
            warningMessage = wxString(_("Directories are dependent! Be careful when setting up synchronization rules:")) + wxT("\n") +
                             wxT("\"") + *i + wxT("\",\n") +
                             wxT("\"") + newFolder + wxT("\"");
            return true;
        }
    return false;
}


bool foldersHaveDependencies(const std::vector<FolderPair>& folderPairs, wxString& warningMessage)
{
    warningMessage.Clear();

    std::vector<Zstring> folders;
    for (std::vector<FolderPair>::const_iterator i = folderPairs.begin(); i != folderPairs.end(); ++i)
    {
        const Zstring leftFolderName  = getFormattedDirectoryName(i->leftDirectory);
        const Zstring rightFolderName = getFormattedDirectoryName(i->rightDirectory);

        if (dependencyExists(folders, leftFolderName, warningMessage))
            return true;
        folders.push_back(leftFolderName);

        if (dependencyExists(folders, rightFolderName, warningMessage))
            return true;
        folders.push_back(rightFolderName);
    }

    return false;
}


CompareProcess::CompareProcess(const bool traverseSymLinks,
                               const unsigned fileTimeTol,
                               bool& warningDependentFolders,
                               StatusHandler* handler) :
        traverseDirectorySymlinks(traverseSymLinks),
        fileTimeTolerance(fileTimeTol),
        m_warningDependentFolders(warningDependentFolders),
        statusUpdater(handler),
        txtComparingContentOfFiles(_("Comparing content of files %x"))
{
    descriptionBuffer = new DirectoryDescrBuffer(traverseSymLinks, handler);
    txtComparingContentOfFiles.Replace(wxT("%x"), wxT("\n\"%x\""), false);
}


CompareProcess::~CompareProcess()
{
    delete descriptionBuffer;
}


struct MemoryAllocator
{
    MemoryAllocator()
    {
        buffer1 = new unsigned char[bufferSize];
        buffer2 = new unsigned char[bufferSize];
    }

    ~MemoryAllocator()
    {
        delete [] buffer1;
        delete [] buffer2;
    }

    static const unsigned int bufferSize = 512 * 1024; //512 kb seems to be the perfect buffer size
    unsigned char* buffer1;
    unsigned char* buffer2;
};


//callback functionality
struct CallBackData
{
    StatusHandler* handler;
    wxULongLong bytesComparedLast;
};

//callback function for status updates whily comparing
typedef void (*CompareCallback)(const wxULongLong&, void*);


void compareContentCallback(const wxULongLong& totalBytesTransferred, void* data)
{   //called every 512 kB
    CallBackData* sharedData = static_cast<CallBackData*>(data);

    //inform about the (differential) processed amount of data
    sharedData->handler->updateProcessedData(0, (totalBytesTransferred - sharedData->bytesComparedLast).ToDouble());
    sharedData->bytesComparedLast = totalBytesTransferred;

    sharedData->handler->requestUiRefresh(); //exceptions may be thrown here!
}


bool filesHaveSameContent(const Zstring& filename1, const Zstring& filename2, CompareCallback callback, void* data)
{
    static MemoryAllocator memory;

    wxFFile file1(filename1.c_str(), wxT("rb"));
    if (!file1.IsOpened())
        throw FileError(Zstring(_("Error reading file:")) + wxT(" \"") + filename1 + wxT("\""));

    wxFFile file2(filename2.c_str(), wxT("rb"));
    if (!file2.IsOpened()) //NO cleanup necessary for (wxFFile) file1
        throw FileError(Zstring(_("Error reading file:")) + wxT(" \"") + filename2 + wxT("\""));

    wxULongLong bytesCompared;
    do
    {
        const size_t length1 = file1.Read(memory.buffer1, memory.bufferSize);
        if (file1.Error()) throw FileError(Zstring(_("Error reading file:")) + wxT(" \"") + filename1 + wxT("\""));

        const size_t length2 = file2.Read(memory.buffer2, memory.bufferSize);
        if (file2.Error()) throw FileError(Zstring(_("Error reading file:")) + wxT(" \"") + filename2 + wxT("\""));

        if (length1 != length2 || memcmp(memory.buffer1, memory.buffer2, length1) != 0)
            return false;

        bytesCompared += length1 * 2;

        //send progress updates
        callback(bytesCompared, data);
    }
    while (!file1.Eof());

    if (!file2.Eof())
        return false;

    return true;
}


bool filesHaveSameContentUpdating(const Zstring& filename1, const Zstring& filename2, const wxULongLong& totalBytesToCmp, StatusHandler* handler)
{
    CallBackData sharedData;
    sharedData.handler = handler;
//data.bytesTransferredLast = //amount of bytes that have been compared and communicated to status handler

    bool sameContent = true;
    try
    {
        sameContent = filesHaveSameContent(filename1, filename2, compareContentCallback, &sharedData);
    }
    catch (...)
    {
        //error situation: undo communication of processed amount of data
        handler->updateProcessedData(0, sharedData.bytesComparedLast.ToDouble() * -1 );

        throw;
    }

    //inform about the (remaining) processed amount of data
    handler->updateProcessedData(0, (totalBytesToCmp - sharedData.bytesComparedLast).ToDouble());

    return sameContent;
}


/* OLD IMPLEMENTATION USING A WORKER THREAD

//handle execution of a method while updating the UI
class UpdateWhileComparing : public UpdateWhileExecuting
{
public:
    UpdateWhileComparing() {}
    ~UpdateWhileComparing() {}

    Zstring file1;
    Zstring file2;
    bool success;
    Zstring errorMessage;
    bool sameContent;

private:
    void longRunner() //virtual method implementation
    {
        try
        {
            sameContent = filesHaveSameContent(file1, file2);
            success = true;
        }
        catch (FileError& error)
        {
            success = false;
            errorMessage = error.show();
        }
    }
};


bool filesHaveSameContentUpdating(const Zstring& filename1, const Zstring& filename2, const wxULongLong& totalBytesToCmp, StatusHandler* updateClass)
{
    static UpdateWhileComparing cmpAndUpdate; //single instantiation: thread enters wait phase after each execution

    cmpAndUpdate.waitUntilReady();

    //longRunner is called from thread, but no mutex needed here, since thread is in waiting state!
    cmpAndUpdate.file1 = filename1;
    cmpAndUpdate.file2 = filename2;

    cmpAndUpdate.execute(updateClass);

    //no mutex needed here since longRunner is finished
    if (!cmpAndUpdate.success)
        throw FileError(cmpAndUpdate.errorMessage);

    //inform about the processed amount of data
    updateClass->updateProcessedData(0, totalBytesToCmp.ToDouble());

    return cmpAndUpdate.sameContent;
}*/


void getBytesToCompare(int& objectsTotal, double& dataTotal, const FileCompareResult& grid, const std::set<int>& rowsToCompare)
{
    dataTotal = 0;

    for (std::set<int>::iterator i = rowsToCompare.begin(); i != rowsToCompare.end(); ++i)
    {
        const FileCompareLine& gridline = grid[*i];

        dataTotal+= gridline.fileDescrLeft.fileSize.ToDouble();
        dataTotal+= gridline.fileDescrRight.fileSize.ToDouble();
    }

    objectsTotal = rowsToCompare.size() * 2;
}


inline
bool sameFileTime(const wxLongLong& a, const wxLongLong& b, const unsigned tolerance)
{
    if (a < b)
        return b - a <= tolerance;
    else
        return a - b <= tolerance;
}


void CompareProcess::startCompareProcess(const std::vector<FolderPair>& directoryPairs,
        const CompareVariant cmpVar,
        FileCompareResult& output) throw(AbortThisProcess)
{
#ifndef __WXDEBUG__
    wxLogNull noWxLogs; //hide wxWidgets log messages in release build
#endif

    //PERF_START;

    //format directory pairs
    std::vector<FolderPair> directoryPairsFormatted;

    for (std::vector<FolderPair>::const_iterator i = directoryPairs.begin(); i != directoryPairs.end(); ++i)
    {
        FolderPair newEntry;
        newEntry.leftDirectory  = FreeFileSync::getFormattedDirectoryName(i->leftDirectory);
        newEntry.rightDirectory = FreeFileSync::getFormattedDirectoryName(i->rightDirectory);
        directoryPairsFormatted.push_back(newEntry);
    }

    //some basic checks:

    //check if folders are valid
    wxString errorMessage;
    if (!foldersAreValidForComparison(directoryPairsFormatted, errorMessage))
    {
        statusUpdater->reportFatalError(errorMessage.c_str());
        return; //should be obsolete!
    }

    //check if folders have dependencies
    if (m_warningDependentFolders) //test if check should be executed
    {
        wxString warningMessage;
        if (foldersHaveDependencies(directoryPairsFormatted, warningMessage))
        {
            bool dontShowAgain = false;
            statusUpdater->reportWarning(warningMessage.c_str(),
                                         dontShowAgain);
            m_warningDependentFolders = !dontShowAgain;
        }
    }

    try
    {
        FileCompareResult output_tmp; //write to output not before END of process!
        if (cmpVar == CMP_BY_TIME_SIZE)
            compareByTimeSize(directoryPairsFormatted, output_tmp);
        else if (cmpVar == CMP_BY_CONTENT)
            compareByContent(directoryPairsFormatted, output_tmp);
        else assert(false);

        //only if everything was processed correctly output is written to! output mustn't change to be in sync with GUI grid view!!!
        output_tmp.swap(output);
    }
    catch (const RuntimeException& theException)
    {
        statusUpdater->reportFatalError(theException.show().c_str());
        return; //should be obsolete!
    }
    catch (std::bad_alloc& e)
    {
        statusUpdater->reportFatalError((wxString(_("System out of memory!")) + wxT(" ") + wxString::From8BitData(e.what())).c_str());
        return; //should be obsolete!
    }
}


void CompareProcess::compareByTimeSize(const std::vector<FolderPair>& directoryPairsFormatted, FileCompareResult& output)
{
    //inform about the total amount of data that will be processed from now on
    statusUpdater->initNewProcess(-1, 0, StatusHandler::PROCESS_SCANNING); //it's not known how many files will be scanned => -1 objects

    //process one folder pair after each other
    unsigned tableSizeOld = 0;
    for (std::vector<FolderPair>::const_iterator pair = directoryPairsFormatted.begin(); pair != directoryPairsFormatted.end(); ++pair)
    {
        //do basis scan: only result lines of type FILE_UNDEFINED (files that exist on both sides) need to be determined after this call
        this->performBaseComparison(*pair, output);

        //categorize files that exist on both sides
        for (FileCompareResult::iterator i = output.begin() + tableSizeOld; i != output.end(); ++i)
        {
            if (i->cmpResult == FILE_UNDEFINED)
            {
                //last write time may differ by up to 2 seconds (NTFS vs FAT32)
                if (sameFileTime(i->fileDescrLeft.lastWriteTimeRaw, i->fileDescrRight.lastWriteTimeRaw, fileTimeTolerance))
                {
                    if (i->fileDescrLeft.fileSize == i->fileDescrRight.fileSize)
                        i->cmpResult = FILE_EQUAL;
                    else
                        i->cmpResult = FILE_DIFFERENT;
                }
                else
                {
                    if (i->fileDescrLeft.lastWriteTimeRaw < i->fileDescrRight.lastWriteTimeRaw)
                        i->cmpResult = FILE_RIGHT_NEWER;
                    else
                        i->cmpResult = FILE_LEFT_NEWER;
                }
            }
        }

        tableSizeOld = output.size();
    }
}


void CompareProcess::compareByContent(const std::vector<FolderPair>& directoryPairsFormatted, FileCompareResult& output)
{
    //PERF_START;

    //inform about the total amount of data that will be processed from now on
    statusUpdater->initNewProcess(-1, 0, StatusHandler::PROCESS_SCANNING); //it's not known how many files will be scanned => -1 objects

    //process one folder pair after each other
    for (std::vector<FolderPair>::const_iterator pair = directoryPairsFormatted.begin(); pair != directoryPairsFormatted.end(); ++pair)
    {
        //do basis scan: only result lines of type FILE_UNDEFINED (files that exist on both sides) need to be determined after this call
        this->performBaseComparison(*pair, output);
    }


    std::set<int> rowsToCompareBytewise; //content comparison of file content happens AFTER finding corresponding files
    //in order to separate into two processes (scanning and comparing)

    //pre-check: files have different content if they have a different filesize
    for (FileCompareResult::iterator i = output.begin(); i != output.end(); ++i)
    {
        if (i->cmpResult == FILE_UNDEFINED)
        {
            if (i->fileDescrLeft.fileSize != i->fileDescrRight.fileSize)
                i->cmpResult = FILE_DIFFERENT;
            else
                rowsToCompareBytewise.insert(i - output.begin());
        }
    }

    int objectsTotal = 0;
    double dataTotal = 0;
    getBytesToCompare(objectsTotal, dataTotal, output, rowsToCompareBytewise);

    statusUpdater->initNewProcess(objectsTotal, dataTotal, StatusHandler::PROCESS_COMPARING_CONTENT);

    std::set<int> rowsToDelete;  //if errors occur during file access and user skips, these rows need to be deleted from result

    //compare files (that have same size) bytewise...
    for (std::set<int>::iterator i = rowsToCompareBytewise.begin(); i != rowsToCompareBytewise.end(); ++i)
    {
        FileCompareLine& gridline = output[*i];

        Zstring statusText = txtComparingContentOfFiles;
        statusText.Replace(wxT("%x"), gridline.fileDescrLeft.relativeName.c_str(), false);
        statusUpdater->updateStatusText(statusText);

        //check files that exist in left and right model but have different content
        while (true)
        {
            //trigger display refresh
            statusUpdater->requestUiRefresh();

            try
            {
                if (filesHaveSameContentUpdating(gridline.fileDescrLeft.fullName, gridline.fileDescrRight.fullName, gridline.fileDescrLeft.fileSize * 2, statusUpdater))
                    gridline.cmpResult = FILE_EQUAL;
                else
                    gridline.cmpResult = FILE_DIFFERENT;

                statusUpdater->updateProcessedData(2, 0); //processed data is communicated in subfunctions!
                break;
            }
            catch (FileError& error)
            {
                ErrorHandler::Response rv = statusUpdater->reportError(error.show());
                if (rv == ErrorHandler::IGNORE_ERROR)
                {
                    rowsToDelete.insert(*i);
                    break;
                }
                else if (rv == ErrorHandler::RETRY)
                    ;   //continue with loop
                else
                    assert (false);
            }
        }
    }

    //delete invalid rows that have no valid cmpResult
    if (rowsToDelete.size() > 0)
        removeRowsFromVector(output, rowsToDelete);
}


class ThreadSorting : public wxThread
{
public:
    ThreadSorting(DirectoryDescrType* directory) :
            wxThread(wxTHREAD_JOINABLE),
            m_directory(directory)
    {
        if (Create() != wxTHREAD_NO_ERROR)
            throw RuntimeException(wxString(wxT("Error creating thread for sorting!")));
    }

    ~ThreadSorting() {}


    ExitCode Entry()
    {
        std::sort(m_directory->begin(), m_directory->end());
        return 0;
    }

private:
    DirectoryDescrType* m_directory;
};


void CompareProcess::performBaseComparison(const FolderPair& pair, FileCompareResult& output)
{
    //PERF_START;
    //retrieve sets of files (with description data)
    DirectoryDescrType* directoryLeft  = descriptionBuffer->getDirectoryDescription(pair.leftDirectory);
    DirectoryDescrType* directoryRight = descriptionBuffer->getDirectoryDescription(pair.rightDirectory);

    statusUpdater->updateStatusText(_("Generating file list..."));
    statusUpdater->forceUiRefresh(); //keep total number of scanned files up to date
    //PERF_STOP;

    //we use binary search when comparing the directory structures: so sort() first
    if (wxThread::GetCPUCount() >= 2) //do it the multithreaded way:
    {
        //no synchronization (multithreading) needed here: directoryLeft and directoryRight are disjunct
        //reference counting Zstring also shouldn't be an issue, as no strings are deleted during std::sort()
        std::auto_ptr<ThreadSorting> sortLeft(new ThreadSorting(directoryLeft));
        std::auto_ptr<ThreadSorting> sortRight(new ThreadSorting(directoryRight));

        if (sortLeft->Run() != wxTHREAD_NO_ERROR)
            throw RuntimeException(wxString(wxT("Error starting thread for sorting!")));

        if (directoryLeft != directoryRight) //attention: might point to the same vector because of buffer!
        {
            if (sortRight->Run() != wxTHREAD_NO_ERROR)
                throw RuntimeException(wxString(wxT("Error starting thread for sorting!")));

            if (sortRight->Wait() != 0)
                throw RuntimeException(wxString(wxT("Error waiting for thread (sorting)!")));
        }

        if (sortLeft->Wait() != 0)
            throw RuntimeException(wxString(wxT("Error waiting for thread (sorting)!")));
    }
    else //single threaded
    {
        std::sort(directoryLeft->begin(), directoryLeft->end());

        if (directoryLeft != directoryRight) //attention: might point to the same vector because of buffer!
            std::sort(directoryRight->begin(), directoryRight->end());
    }
    //PERF_STOP;

    //reserve some space to avoid too many vector reallocations: doesn't make much sense for multiple folder pairs, but doesn't hurt either
    output.reserve(output.size() + unsigned(std::max(directoryLeft->size(), directoryRight->size()) * 1.2));

    //begin base comparison
    FileCompareLine newline;
    DirectoryDescrType::iterator j;
    for (DirectoryDescrType::const_iterator i = directoryLeft->begin(); i != directoryLeft->end(); ++i)
    {    //find files/folders that exist in left file tree but not in right one
        if ((j = custom_binary_search(directoryRight->begin(), directoryRight->end(), *i)) == directoryRight->end())
        {
            newline.fileDescrLeft            = *i;
            newline.fileDescrRight           = FileDescrLine();
            newline.fileDescrRight.directory = pair.rightDirectory;
            newline.cmpResult                = FILE_LEFT_SIDE_ONLY;
            output.push_back(newline);
        }
        //find files/folders that exist on left and right side
        else
        {
            const FileDescrLine::ObjectType typeLeft  = i->objType;
            const FileDescrLine::ObjectType typeRight = j->objType;

            //files...
            if (typeLeft == FileDescrLine::TYPE_FILE && typeRight == FileDescrLine::TYPE_FILE)
            {
                newline.fileDescrLeft  = *i;
                newline.fileDescrRight = *j;
                newline.cmpResult      = FILE_UNDEFINED; //not yet determined!
                output.push_back(newline);
            }
            //directories...
            else if (typeLeft == FileDescrLine::TYPE_DIRECTORY && typeRight == FileDescrLine::TYPE_DIRECTORY)
            {
                newline.fileDescrLeft  = *i;
                newline.fileDescrRight = *j;
                newline.cmpResult      = FILE_EQUAL;
                output.push_back(newline);
            }
            //if we have a nameclash between a file and a directory: split into two separate rows
            else
            {
                assert (typeLeft != typeRight);

                newline.fileDescrLeft            = *i;
                newline.fileDescrRight           = FileDescrLine();
                newline.fileDescrRight.directory = pair.rightDirectory;
                newline.cmpResult                = FILE_LEFT_SIDE_ONLY;
                output.push_back(newline);

                newline.fileDescrLeft           = FileDescrLine();
                newline.fileDescrLeft.directory = pair.leftDirectory;
                newline.fileDescrRight          = *j;
                newline.cmpResult               = FILE_RIGHT_SIDE_ONLY;
                output.push_back(newline);
            }
        }
    }


    for (DirectoryDescrType::const_iterator j = directoryRight->begin(); j != directoryRight->end(); ++j)
    {
        //find files/folders that exist in right file model but not in left model
        if (custom_binary_search(directoryLeft->begin(), directoryLeft->end(), *j) == directoryLeft->end())
        {
            newline.fileDescrLeft           = FileDescrLine();
            newline.fileDescrLeft.directory = pair.leftDirectory;  //directory info is needed when creating new directories
            newline.fileDescrRight          = *j;
            newline.cmpResult               = FILE_RIGHT_SIDE_ONLY;
            output.push_back(newline);
        }
    }

    //PERF_STOP
}
