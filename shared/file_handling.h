// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) 2008-2011 ZenJu (zhnmju123 AT gmx.de)                    *
// **************************************************************************
//
#ifndef FILE_HANDLING_H_INCLUDED
#define FILE_HANDLING_H_INCLUDED

#include <wx/longlong.h>
#include "zstring.h"
#include "file_error.h"


namespace ffs3
{
struct CallbackRemoveDir;
struct CallbackMoveFile;
struct CallbackCopyFile;


bool fileExists(     const Zstring& filename); //throw()       replaces wxFileExists()
bool dirExists(      const Zstring& dirname);  //throw()       replaces wxDirExists()
bool symlinkExists(  const Zstring& objname);  //throw()       check whether a symbolic link exists
bool somethingExists(const Zstring& objname);  //throw()       check whether any object with this name exists

//check whether two folders are located on the same (logical) volume
//left and right directories NEED NOT yet exist! volume prefix is sufficient! path may end with PATH_SEPARATOR
enum ResponseSameVol
{
    VOLUME_SAME,
    VOLUME_DIFFERENT,
    VOLUME_CANT_SAY
};
ResponseSameVol onSameVolume(const Zstring& folderLeft, const Zstring& folderRight); //throw()

//copy file or directory create/last change date,
void copyFileTimes(const Zstring& sourceDir, const Zstring& targetDir, bool derefSymlinks); //throw (FileError)

//symlink handling: always evaluate target
wxULongLong getFilesize(const Zstring& filename); //throw (FileError)


//file handling
void removeFile(const Zstring& filename);       //throw (FileError)
void removeDirectory(const Zstring& directory, CallbackRemoveDir* callback = NULL); //throw (FileError)


//rename file or directory: no copying!!!
void renameFile(const Zstring& oldName, const Zstring& newName); //throw (FileError);

//move source to target; expectations: all super-directories of target exist
//"ignoreExisting": if target already exists, source is deleted
void moveFile(const Zstring& sourceFile, const Zstring& targetFile, bool ignoreExisting, CallbackMoveFile* callback); //throw (FileError);

//move source to target including subdirectories
//"ignoreExisting": existing directories and files will be enriched
void moveDirectory(const Zstring& sourceDir, const Zstring& targetDir, bool ignoreExisting, CallbackMoveFile* callback); //throw (FileError);

//creates superdirectories automatically:
void createDirectory(const Zstring& directory, const Zstring& templateDir, bool copyFilePermissions); //throw (FileError);
void createDirectory(const Zstring& directory); //throw (FileError); -> function overload avoids default parameter ambiguity issues!


void copyFile(const Zstring& sourceFile, //throw (FileError: ErrorTargetPathMissing, ErrorFileLocked (Windows-only));
              const Zstring& targetFile,
              bool copyFilePermissions,
              CallbackCopyFile* callback);  //may be NULL
//Note: it MAY happen that copyFile() leaves temp files behind, e.g. temporary network drop.
// => clean them up at an appropriate time (automatically set sync directions to delete them). They have the following ending:
const Zstring TEMP_FILE_ENDING = Zstr(".ffs_tmp");

enum SymlinkType
{
    SYMLINK_TYPE_FILE,
    SYMLINK_TYPE_DIR
};
void copySymlink(const Zstring& sourceLink, const Zstring& targetLink, SymlinkType type, bool copyFilePermissions); //throw (FileError)





//----------- callbacks ---------------
struct CallbackRemoveDir
{
    virtual ~CallbackRemoveDir() {}
    virtual void requestUiRefresh(const Zstring& currentObject) = 0;
};


struct CallbackMoveFile //callback functionality
{
    virtual ~CallbackMoveFile() {}

    enum Response
    {
        CONTINUE,
        CANCEL
    };
    virtual Response requestUiRefresh(const Zstring& currentObject) = 0;  //DON'T throw exceptions here, at least in Windows build!
};


struct CallbackCopyFile //callback functionality
{
    virtual ~CallbackCopyFile() {}

    //if target is existing user needs to implement deletion: copyFile() NEVER deletes target if already existing!
    //at this point full read access on source had been proven, so it's safe to delete it.
    virtual void deleteTargetFile(const Zstring& targetFile) = 0; //may throw exceptions

    enum Response
    {
        CONTINUE,
        CANCEL
    };
    virtual Response updateCopyStatus(const wxULongLong& totalBytesTransferred) = 0; //DON'T throw exceptions here, at least in Windows build!
};
}

#endif //FILE_HANDLING_H_INCLUDED
