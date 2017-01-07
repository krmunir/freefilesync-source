#include "versioning.h"
#include <cstddef> //required by GCC 4.8.1 to find ptrdiff_t

using namespace zen;
using AFS = AbstractFileSystem;

namespace
{
inline
Zstring getDotExtension(const Zstring& relativePath) //including "." if extension is existing, returns empty string otherwise
{
    const Zstring& extension = getFileExtension(relativePath);
    return extension.empty() ? extension : Zstr('.') + extension;
};
}

bool impl::isMatchingVersion(const Zstring& shortname, const Zstring& shortnameVersioned) //e.g. ("Sample.txt", "Sample.txt 2012-05-15 131513.txt")
{
    auto it = shortnameVersioned.begin();
    auto itLast = shortnameVersioned.end();

    auto nextDigit = [&]() -> bool
    {
        if (it == itLast || !isDigit(*it))
            return false;
        ++it;
        return true;
    };
    auto nextDigits = [&](size_t count) -> bool
    {
        while (count-- > 0)
            if (!nextDigit())
                return false;
        return true;
    };
    auto nextChar = [&](Zchar c) -> bool
    {
        if (it == itLast || *it != c)
            return false;
        ++it;
        return true;
    };
    auto nextStringI = [&](const Zstring& str) -> bool //windows: ignore case!
    {
        if (itLast - it < static_cast<ptrdiff_t>(str.size()) || !equalFilePath(str, Zstring(&*it, str.size())))
            return false;
        it += str.size();
        return true;
    };

    return nextStringI(shortname) && //versioned file starts with original name
           nextChar(Zstr(' ')) && //validate timestamp: e.g. "2012-05-15 131513"; Regex: \d{4}-\d{2}-\d{2} \d{6}
           nextDigits(4)       && //YYYY
           nextChar(Zstr('-')) && //
           nextDigits(2)       && //MM
           nextChar(Zstr('-')) && //
           nextDigits(2)       && //DD
           nextChar(Zstr(' ')) && //
           nextDigits(6)       && //HHMMSS
           nextStringI(getDotExtension(shortname)) &&
           it == itLast;
}


/*
create target super directories if missing
*/
void FileVersioner::moveItemToVersioning(const Zstring& relativePath, //throw FileError
                                         const std::function<void(const AbstractPath& targetPath)>& moveExistingItem) //move source -> target; may throw FileError
{
    assert(!startsWith(relativePath, FILE_NAME_SEPARATOR));
    assert(!endsWith  (relativePath, FILE_NAME_SEPARATOR));
    assert(!relativePath.empty());

    Zstring versionedRelPath;
    switch (versioningStyle_)
    {
        case VersioningStyle::REPLACE:
            versionedRelPath = relativePath;
            break;
        case VersioningStyle::ADD_TIMESTAMP: //assemble time-stamped version name
            versionedRelPath = relativePath + Zstr(' ') + timeStamp_ + getDotExtension(relativePath);
            assert(impl::isMatchingVersion(afterLast(relativePath,     FILE_NAME_SEPARATOR, IF_MISSING_RETURN_ALL),
                                           afterLast(versionedRelPath, FILE_NAME_SEPARATOR, IF_MISSING_RETURN_ALL))); //paranoid? no!
            break;
    }

    const AbstractPath versionedItemPath = AFS::appendRelPath(versioningFolderPath_, versionedRelPath);
    try
    {
        moveExistingItem(versionedItemPath); //throw FileError
    }
    catch (const FileError&) //expected to fail if target directory is not yet existing!
    {
        warn_static("perf: combine with removeTarget!!!")

        //create intermediate directories if missing
        if (Opt<AbstractPath> parentPath = AFS::getParentFolderPath(versionedItemPath))
        {
            Opt<AFS::PathDetails> pd;
            try { pd = AFS::getPathDetails(*parentPath); /*throw FileError*/ }
            catch (FileError&) {} //previous exception is more relevant

            if (pd && !pd->relPath.empty()) //parent folder not existing
            {
                AbstractPath intermediatePath = pd->existingPath;
                for (const Zstring& itemName : pd->relPath)
                    AFS::createFolderPlain(intermediatePath = AFS::appendRelPath(intermediatePath, itemName)); //throw FileError

                //retry: this should work now!
                moveExistingItem(versionedItemPath); //throw FileError
                return;
            }
        }
        throw;
    }
}


namespace
{
//move source to target across volumes
//no need to check if super-directories of target exist: done by moveItemToVersioning()
//if target already exists, it is overwritten, even if it is a different type, e.g. a directory!
template <class Function>
void moveExistingItem(const AbstractPath& sourcePath, //throw FileError
                      const AbstractPath& targetPath,
                      Function copyDelete) //throw FileError; fallback if move failed
{
    auto removeTarget = [&](bool expectExisting)
    {
        try
        {
            //file or (broken) file-symlink:
            AFS::removeFilePlain(targetPath); //throw FileError
        }
        catch (FileError&)
        {
            //(folder-)symlink:
            bool symlinkExists = false;
            try
            {
                if (expectExisting) //perf, perf, perf, every file access counts!
                    symlinkExists = AFS::getItemType(targetPath) == AFS::ItemType::SYMLINK; //throw FileError
                else
                {
                    if (Opt<AFS::ItemType> type = AFS::getItemTypeIfExists(targetPath)) //throw FileError
                        symlinkExists = *type == AFS::ItemType::SYMLINK;
                    else
                        return;
                }
            }
            catch (FileError&) {} //previous exception is more relevant

            if (symlinkExists)
                AFS::removeSymlinkPlain(targetPath); //throw FileError
            else //overwrite AFS::ItemType::FOLDER with FILE? => highly dubious, do not allow
                throw;
        }
    };

    try //first try to move directly without copying
    {
        AFS::renameItem(sourcePath, targetPath); //throw FileError, ErrorTargetExisting, ErrorDifferentVolume
        //great, we get away cheaply!
    }
    catch (const ErrorDifferentVolume&)
    {
        removeTarget(false /*expectExisting*/); //throw FileError
        copyDelete();   //
    }
    catch (const ErrorTargetExisting&)
    {
        removeTarget(true /*expectExisting*/); //throw FileError
        try
        {
            AFS::renameItem(sourcePath, targetPath); //throw FileError, (ErrorTargetExisting), ErrorDifferentVolume
        }
        catch (const ErrorDifferentVolume&)
        {
            copyDelete(); //throw FileError
        }
    }
}


void moveExistingFile(const AbstractPath& sourcePath, //throw FileError
                      const AbstractPath& targetPath,
                      const std::function<void(std::int64_t bytesDelta)>& onNotifyCopyStatus) //may be nullptr
{
    auto copyDelete = [&]
    {
        AFS::copyFileTransactional(sourcePath, targetPath, //throw FileError, ErrorFileLocked
        false /*copyFilePermissions*/, true /*transactionalCopy*/, nullptr /*onDeleteTargetFile*/, onNotifyCopyStatus);
        AFS::removeFilePlain(sourcePath); //throw FileError; newly copied item is NOT deleted if exception is thrown here!
    };
    moveExistingItem(sourcePath, targetPath, copyDelete); //throw FileError
}


void moveExistingSymlink(const AbstractPath& sourcePath, const AbstractPath& targetPath) //throw FileError
{
    auto copyDelete = [&]
    {
        AFS::copySymlink(sourcePath, targetPath, false /*copy filesystem permissions*/); //throw FileError
        AFS::removeSymlinkPlain(sourcePath); //throw FileError; newly copied item is NOT deleted if exception is thrown here!
    };
    moveExistingItem(sourcePath, targetPath, copyDelete); //throw FileError
}


struct FlatTraverserCallback: public AFS::TraverserCallback
{
    FlatTraverserCallback(const AbstractPath& folderPath) : folderPath_(folderPath) {}

    const std::vector<Zstring>& refFileNames   () const { return fileNames_; }
    const std::vector<Zstring>& refFolderNames () const { return folderNames_; }
    const std::vector<Zstring>& refSymlinkNames() const { return symlinkNames_; }

private:
    void                               onFile   (const FileInfo&    fi) override { fileNames_   .push_back(fi.itemName); }
    std::unique_ptr<TraverserCallback> onFolder (const FolderInfo&  fi) override { folderNames_ .push_back(fi.itemName); return nullptr; }
    HandleLink                         onSymlink(const SymlinkInfo& si) override { symlinkNames_.push_back(si.itemName); return TraverserCallback::LINK_SKIP; }

    HandleError reportDirError (const std::wstring& msg, size_t retryNumber)                          override { throw FileError(msg); }
    HandleError reportItemError(const std::wstring& msg, size_t retryNumber, const Zstring& itemName) override { throw FileError(msg); }

    const AbstractPath folderPath_;
    std::vector<Zstring> fileNames_;
    std::vector<Zstring> folderNames_;
    std::vector<Zstring> symlinkNames_;
};
}


bool FileVersioner::revisionFile(const AbstractPath& filePath, const Zstring& relativePath, const std::function<void(std::int64_t bytesDelta)>& onNotifyCopyStatus) //throw FileError
{
    if (Opt<AFS::ItemType> type = AFS::getItemTypeIfExists(filePath)) //throw FileError
    {
        bool moveSuccessful = false;

        moveItemToVersioning(relativePath, //throw FileError
                             [&](const AbstractPath& targetPath)
        {
            if (*type == AFS::ItemType::SYMLINK)
                moveExistingSymlink(filePath, targetPath); //throw FileError
            else
                moveExistingFile(filePath, targetPath, onNotifyCopyStatus); //throw FileError

            moveSuccessful = true;
        });
        return moveSuccessful;
    }
    else
        return false; //missing source item is not an error => check BEFORE oerwriting target
}


bool FileVersioner::revisionSymlink(const AbstractPath& linkPath, const Zstring& relativePath) //throw FileError
{
    if (AFS::getItemTypeIfExists(linkPath)) //throw FileError
    {
        bool moveSuccessful = false;

        moveItemToVersioning(relativePath, //throw FileError
                             [&](const AbstractPath& targetPath)
        {
            moveExistingSymlink(linkPath, targetPath); //throw FileError
            moveSuccessful = true;
        });
        return moveSuccessful;
    }
    else
        return false;
}


void FileVersioner::revisionFolder(const AbstractPath& folderPath, const Zstring& relativePath, //throw FileError
                                   const std::function<void(const std::wstring& displayPathFrom, const std::wstring& displayPathTo)>& onBeforeFileMove,
                                   const std::function<void(const std::wstring& displayPathFrom, const std::wstring& displayPathTo)>& onBeforeFolderMove,
                                   const std::function<void(std::int64_t bytesDelta)>& onNotifyCopyStatus)
{
    if (Opt<AFS::ItemType> type = AFS::getItemTypeIfExists(folderPath)) //throw FileError
    {
        if (*type ==  AFS::ItemType::SYMLINK) //on Linux there is just one type of symlink, and since we do revision file symlinks, we should revision dir symlinks as well!
        {
            moveItemToVersioning(relativePath, //throw FileError
                                 [&](const AbstractPath& targetPath)
            {
                if (onBeforeFileMove)
                    onBeforeFileMove(AFS::getDisplayPath(folderPath), AFS::getDisplayPath(targetPath));
                moveExistingSymlink(folderPath, targetPath); //throw FileError
            });
        }
        else
            revisionFolderImpl(folderPath, relativePath, onBeforeFileMove, onBeforeFolderMove, onNotifyCopyStatus); //throw FileError
    }
    //no error situation if directory is not existing! manual deletion relies on it!
}


void FileVersioner::revisionFolderImpl(const AbstractPath& folderPath, const Zstring& relativePath, //throw FileError
                                       const std::function<void(const std::wstring& displayPathFrom, const std::wstring& displayPathTo)>& onBeforeFileMove,
                                       const std::function<void(const std::wstring& displayPathFrom, const std::wstring& displayPathTo)>& onBeforeFolderMove,
                                       const std::function<void(std::int64_t bytesDelta)>& onNotifyCopyStatus)
{

    //create target directories only when needed in moveFileToVersioning(): avoid empty directories!

    FlatTraverserCallback ft(folderPath); //traverse source directory one level deep
    AFS::traverseFolder(folderPath, ft);

    const Zstring relPathPf = appendSeparator(relativePath);

    for (const Zstring& fileName : ft.refFileNames())
        moveItemToVersioning(relPathPf + fileName, //throw FileError
                             [&](const AbstractPath& targetPath)
    {
        const AbstractPath sourcePath = AFS::appendRelPath(folderPath, fileName);
        if (onBeforeFileMove)
            onBeforeFileMove(AFS::getDisplayPath(sourcePath), AFS::getDisplayPath(targetPath));
        moveExistingFile(sourcePath, targetPath, onNotifyCopyStatus); //throw FileError
    });

    for (const Zstring& symlinkName : ft.refSymlinkNames())
        moveItemToVersioning(relPathPf + symlinkName, //throw FileError
                             [&](const AbstractPath& targetPath)
    {
        const AbstractPath sourcePath = AFS::appendRelPath(folderPath, symlinkName);
        if (onBeforeFileMove)
            onBeforeFileMove(AFS::getDisplayPath(sourcePath), AFS::getDisplayPath(targetPath));
        moveExistingSymlink(sourcePath, targetPath); //throw FileError
    });

    //move folders recursively
    for (const Zstring& folderName : ft.refFolderNames())
        revisionFolderImpl(AFS::appendRelPath(folderPath, folderName), //throw FileError
                           relPathPf + folderName,
                           onBeforeFileMove, onBeforeFolderMove, onNotifyCopyStatus);
    //delete source
    if (onBeforeFolderMove)
        onBeforeFolderMove(AFS::getDisplayPath(folderPath), AFS::getDisplayPath(AFS::appendRelPath(versioningFolderPath_, relativePath)));

    AFS::removeFolderPlain(folderPath); //throw FileError
}


/*
void FileVersioner::limitVersions(std::function<void()> updateUI) //throw FileError
{
    if (versionCountLimit_ < 0) //no limit!
        return;

    //buffer map "directory |-> list of immediate child file and symlink short names"
    std::map<Zstring, std::vector<Zstring>, LessFilePath> dirBuffer;

    auto getVersionsBuffered = [&](const Zstring& dirpath) -> const std::vector<Zstring>&
    {
        auto it = dirBuffer.find(dirpath);
        if (it != dirBuffer.end())
            return it->second;

        std::vector<Zstring> fileShortNames;
        TraverseVersionsOneLevel tol(fileShortNames, updateUI); //throw FileError
        traverseFolder(dirpath, tol);

        auto& newEntry = dirBuffer[dirpath]; //transactional behavior!!!
        newEntry.swap(fileShortNames);       //-> until C++11 emplace is available

        return newEntry;
    };

    std::for_each(fileRelNames.begin(), fileRelNames.end(),
                  [&](const Zstring& relativePath) //e.g. "subdir\Sample.txt"
    {
        const Zstring filepath = appendSeparator(versioningDirectory_) + relativePath; //e.g. "D:\Revisions\subdir\Sample.txt"
        const Zstring parentDir = beforeLast(filepath, FILE_NAME_SEPARATOR);    //e.g. "D:\Revisions\subdir"
        const Zstring shortname = afterLast(relativePath, FILE_NAME_SEPARATOR); //e.g. "Sample.txt"; returns the whole string if seperator not found

        const std::vector<Zstring>& allVersions = getVersionsBuffered(parentDir);

        //filter out only those versions that match the given relative name
        std::vector<Zstring> matches; //e.g. "Sample.txt 2012-05-15 131513.txt"

        std::copy_if(allVersions.begin(), allVersions.end(), std::back_inserter(matches),
        [&](const Zstring& shortnameVer) { return impl::isMatchingVersion(shortname, shortnameVer); });

        //take advantage of version naming convention to find oldest versions
        if (matches.size() <= static_cast<size_t>(versionCountLimit_))
            return;
        std::nth_element(matches.begin(), matches.end() - versionCountLimit_, matches.end(), LessFilePath()); //windows: ignore case!

        //delete obsolete versions
        std::for_each(matches.begin(), matches.end() - versionCountLimit_,
                      [&](const Zstring& shortnameVer)
        {
            updateUI();
            const Zstring fullnameVer = parentDir + FILE_NAME_SEPARATOR + shortnameVer;
            try
            {
                removeFile(fullnameVer); //throw FileError
            }
            catch (FileError&)
            {
#ifdef ZEN_WIN //if it's a directory symlink:
                if (symlinkExists(fullnameVer) && dirExists(fullnameVer))
                    removeDirectory(fullnameVer); //throw FileError
                else
#endif
                    throw;
            }
        });
    });
}
*/
