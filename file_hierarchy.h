// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) 2008-2010 ZenJu (zhnmju123 AT gmx.de)                    *
// **************************************************************************
//
#ifndef FILEHIERARCHY_H_INCLUDED
#define FILEHIERARCHY_H_INCLUDED

#include "shared/zstring.h"
#include "shared/system_constants.h"
#include <wx/longlong.h>
#include <map>
#include <set>
#include <vector>
#include "structures.h"
#include <boost/shared_ptr.hpp>
#include "shared/guid.h"
#include "library/filter.h"
#include "shared/file_id.h"
#include "library/dir_lock.h"

class DirectoryBuffer;


namespace util //helper class to grant algorithms like std::for_each access to private parts of a predicate class
{
template <class T>
class ProxyForEach
{
public:
    ProxyForEach(T& target) : target_(target) {}
    template <class FS> void operator()(FS& obj) const
    {
        target_(obj);
    }

private:
    T& target_;
};
}


namespace ffs3
{
struct FileDescriptor
{
    FileDescriptor(const wxLongLong&  lastWriteTimeRawIn,
                   const wxULongLong& fileSizeIn) :
        //const util::FileID fileId) :
        lastWriteTimeRaw(lastWriteTimeRawIn),
        fileSize(fileSizeIn) {}
    wxLongLong  lastWriteTimeRaw;   //number of seconds since Jan. 1st 1970 UTC, same semantics like time_t (== signed long)
    wxULongLong fileSize;

    //#warning: what about memory consumption?? (assume large comparisons!!?)
    //util::FileID fileIdentifier; //unique file identifier, optional: may be NULL!
};


struct LinkDescriptor
{
    enum LinkType
    {
        TYPE_DIR, //Windows: dir  symlink; Linux: dir symlink
        TYPE_FILE //Windows: file symlink; Linux: file symlink or broken link (or other symlink, pathological)
    };

    LinkDescriptor(const wxLongLong& lastWriteTimeRawIn,
                   const Zstring& targetPathIn,
                   LinkType lt) :
        lastWriteTimeRaw(lastWriteTimeRawIn),
        targetPath(targetPathIn),
        type(lt) {}

    wxLongLong lastWriteTimeRaw; //number of seconds since Jan. 1st 1970 UTC, same semantics like time_t (== signed long)
    Zstring    targetPath;       //symlink "content", may be empty if determination failed
    LinkType   type;             //type is required for Windows only! On Linux there is no such thing => consider this when comparing Symbolic Links!
};


enum SelectedSide
{
    LEFT_SIDE,
    RIGHT_SIDE
};


class FileContainer;
class SymLinkContainer;
class DirMapping;
class FileMapping;
class SymLinkMapping;
class FileSystemObject;
class BaseDirMapping;
class HierarchyObject;
//------------------------------------------------------------------
/*
  DirContainer    FileContainer

ERD:
DirContainer 1 -----> 0..n DirContainer
DirContainer 1 -----> 0..n FileContainer
*/

//------------------------------------------------------------------

class DirContainer
{
public:
    void addSubFile(const Zstring& shortName, const FileDescriptor& fileData);
    void addSubLink(const Zstring& shortName, const LinkDescriptor& linkData);
    DirContainer& addSubDir(const Zstring& shortName);

    //------------------------------------------------------------------
    typedef std::map<Zstring, DirContainer,     LessFilename> SubDirList;  //key: shortName
    typedef std::map<Zstring, FileContainer,    LessFilename> SubFileList; //
    typedef std::map<Zstring, SymLinkContainer, LessFilename> SubLinkList; //
    //------------------------------------------------------------------

    //iterator access: std::map is implementation detail! don't expose
    SubDirList::const_iterator dirBegin()   const;
    SubDirList::const_iterator dirEnd()     const;

    SubFileList::const_iterator fileBegin() const;
    SubFileList::const_iterator fileEnd()   const;

    SubLinkList::const_iterator linkBegin() const;
    SubLinkList::const_iterator linkEnd()   const;

    size_t dirCount() const;
    size_t fileCount() const;
    size_t linkCount() const;

    const DirContainer*     findDir (const Zstring& shortName) const; //returns NULL if not found
    const FileContainer*    findFile(const Zstring& shortName) const; //
    const SymLinkContainer* findLink(const Zstring& shortName) const; //

    DirContainer() {} //default constructor use for base directory only!

private:
    SubDirList  subDirs;  //contained directories
    SubFileList subFiles; //contained files
    SubLinkList subLinks; //contained symlinks (note: only symlinks that are not treated as their target are placed here!)
};

//------------------------------------------------------------------
class FileContainer
{
public:
    const FileDescriptor& getData() const;

private:
    friend class DirContainer;
    FileContainer(const FileDescriptor& fileData) : data(fileData) {}

    const FileDescriptor data;
};

class SymLinkContainer
{
public:
    const LinkDescriptor& getData() const;

private:
    friend class DirContainer;
    SymLinkContainer(const LinkDescriptor& linkData) : data(linkData) {}

    const LinkDescriptor data;
};


//------------------------------------------------------------------
//save/load full directory information
const Zstring& getSyncDBFilename(); //get short filename of database file
const Zstring SYNC_DB_FILE_ENDING = Zstr("ffs_db");

//------------------------------------------------------------------
/*    class hierarchy:

               FileSystemObject         HierarchyObject
                     /|\                      /|\
       _______________|______________    ______|______
      |               |              |  |             |
SymLinkMapping    FileMapping     DirMapping    BaseDirMapping
*/

//------------------------------------------------------------------


class HierarchyObject
{
public:
    typedef size_t ObjectID;
    FileSystemObject* retrieveById(ObjectID id);             //returns NULL if object is not found; logarithmic complexity
    const FileSystemObject* retrieveById(ObjectID id) const; //

    DirMapping& addSubDir(bool existsLeft,
                          const Zstring& dirNameShort,
                          bool existsRight);

    FileMapping& addSubFile(const FileDescriptor&   left,          //file exists on both sides
                            const Zstring&          fileNameShort,
                            CompareFilesResult      defaultCmpResult,
                            const FileDescriptor&   right);
    void addSubFile(const FileDescriptor&   left,          //file exists on left side only
                    const Zstring&          fileNameShort);
    void addSubFile(const Zstring&          fileNameShort, //file exists on right side only
                    const FileDescriptor&   right);

    SymLinkMapping& addSubLink(const LinkDescriptor& left,  //link exists on both sides
                               const Zstring&        linkNameShort,
                               CompareSymlinkResult  defaultCmpResult,
                               const LinkDescriptor& right);
    void addSubLink(const LinkDescriptor& left,          //link exists on left side only
                    const Zstring&        linkNameShort);
    void addSubLink(const Zstring&        linkNameShort, //link exists on right side only
                    const LinkDescriptor& right);

    const Zstring& getRelativeNamePf() const; //get name relative to base sync dir with FILE_NAME_SEPARATOR postfix: "blah\"
    template <SelectedSide side> const Zstring& getBaseDir() const; //postfixed!

    typedef std::vector<FileMapping>    SubFileMapping; //MergeSides::execute() requires a structure that doesn't invalidate pointers after push_back()
    typedef std::vector<DirMapping>     SubDirMapping;  //Note: deque<> has circular reference in VCPP!
    typedef std::vector<SymLinkMapping> SubLinkMapping;

    SubFileMapping& useSubFiles();
    SubLinkMapping& useSubLinks();
    SubDirMapping&  useSubDirs();
    const SubFileMapping& useSubFiles() const;
    const SubLinkMapping& useSubLinks() const;
    const SubDirMapping&  useSubDirs()  const;

protected:
    //constructor used by DirMapping
    HierarchyObject(const HierarchyObject& parent, const Zstring& shortName) :
        relNamePf(   parent.getRelativeNamePf() + shortName + common::FILE_NAME_SEPARATOR),
        baseDirLeft( parent.baseDirLeft),
        baseDirRight(parent.baseDirRight) {}

    //constructor used by BaseDirMapping
    HierarchyObject(const Zstring& dirPostfixedLeft,
                    const Zstring& dirPostfixedRight) :
        baseDirLeft(dirPostfixedLeft),
        baseDirRight(dirPostfixedRight) {}

    ~HierarchyObject() {} //don't need polymorphic deletion

    virtual void swap();

private:
    SubFileMapping subFiles; //contained file maps
    SubLinkMapping subLinks; //contained symbolic link maps
    SubDirMapping  subDirs;  //contained directory maps

    Zstring relNamePf;
    Zstring baseDirLeft;  //directory name ending with FILE_NAME_SEPARATOR
    Zstring baseDirRight; //directory name ending with FILE_NAME_SEPARATOR
};

template <>
inline
const Zstring& HierarchyObject::getBaseDir<LEFT_SIDE>() const //postfixed!
{
    return baseDirLeft;
}


template <>
inline
const Zstring& HierarchyObject::getBaseDir<RIGHT_SIDE>() const //postfixed!
{
    return baseDirRight;
}


//------------------------------------------------------------------
struct RelNamesBuffered
{
    RelNamesBuffered(const Zstring& baseDirPfLIn, //base sync dir postfixed
                     const Zstring& baseDirPfRIn,
                     const Zstring& parentRelNamePfIn) : //relative parent name postfixed
        baseDirPfL(baseDirPfLIn),
        baseDirPfR(baseDirPfRIn),
        parentRelNamePf(parentRelNamePfIn) {}

    Zstring baseDirPfL;
    Zstring baseDirPfR;
    Zstring parentRelNamePf;
};



class FSObjectVisitor
{
public:
    virtual ~FSObjectVisitor() {}
    virtual void visit(const FileMapping&    fileObj) = 0;
    virtual void visit(const SymLinkMapping& linkObj) = 0;
    virtual void visit(const DirMapping&     dirObj)  = 0;
};

//------------------------------------------------------------------
class FileSystemObject
{
public:
    virtual void accept(FSObjectVisitor& visitor) const = 0;

    const Zstring getParentRelativeName() const; //get name relative to base sync dir without FILE_NAME_SEPARATOR postfix
    const Zstring getObjRelativeName() const;    //same as getRelativeName() but also returns value if either side is empty
    const Zstring& getObjShortName() const;      //same as getShortName() but also returns value if either side is empty
    template <SelectedSide side>           bool     isEmpty()         const;
    template <SelectedSide side> const Zstring&     getShortName()    const;
    template <SelectedSide side> const Zstring      getRelativeName() const; //get name relative to base sync dir without FILE_NAME_SEPARATOR prefix
    template <SelectedSide side> const Zstring      getFullName()     const; //getFullName() == getBaseDirPf() + getRelativeName()
    template <SelectedSide side> const Zstring&     getBaseDirPf()    const; //base sync directory postfixed with FILE_NAME_SEPARATOR

    HierarchyObject::ObjectID getId() const; //get unique id; ^= logical key

    //comparison result
    virtual CompareFilesResult getCategory() const = 0;
    virtual const wxString& getCatConflict() const = 0; //only filled if getCategory() == FILE_CONFLICT
    //sync operation
    SyncOperation getSyncOperation() const;
    const wxString& getSyncOpConflict() const; //only filled if getSyncOperation() == SYNC_DIR_INT_CONFLICT
    SyncOperation testSyncOperation(bool selected, SyncDirection syncDir) const; //get syncOp with provided settings

    //sync settings
    void setSyncDir(SyncDirection newDir);
    void setSyncDirConflict(const wxString& description);   //set syncDir = SYNC_DIR_INT_CONFLICT

    bool isActive() const;
    void setActive(bool active);

    void synchronizeSides();          //copy one side to the other (NOT recursive!!!)
    template <SelectedSide side> void removeObject();    //removes file or directory (recursively!) without physically removing the element: used by manual deletion
    bool isEmpty() const; //true, if both sides are empty
    static void removeEmpty(BaseDirMapping& baseDir);        //physically remove all invalid entries (where both sides are empty) recursively
    static void removeEmptyNonRec(HierarchyObject& hierObj); //physically remove all invalid entries (where both sides are empty) non-recursively

protected:
    FileSystemObject(bool existsLeft, bool existsRight, const Zstring& shortName, const HierarchyObject& parent) :
        selectedForSynchronization(true),
        syncDir(SYNC_DIR_INT_NONE),
        nameBuffer(parent.getBaseDir<LEFT_SIDE>(), parent.getBaseDir<RIGHT_SIDE>(), parent.getRelativeNamePf()),
        existsLeft_(existsLeft),
        existsRight_(existsRight),
        shortName_(shortName),
        uniqueId(getUniqueId()) {}

    ~FileSystemObject() {} //don't need polymorphic deletion

    virtual void swap();

private:
    virtual           void removeObjectL()  = 0;
    virtual           void removeObjectR()  = 0;
    virtual           void copyToL() = 0;
    virtual           void copyToR() = 0;
    static HierarchyObject::ObjectID getUniqueId();

    enum SyncDirectionIntern //same as SyncDirection, but one additional conflict type
    {
        SYNC_DIR_INT_LEFT  = SYNC_DIR_LEFT,
        SYNC_DIR_INT_RIGHT = SYNC_DIR_RIGHT,
        SYNC_DIR_INT_NONE  = SYNC_DIR_NONE,
        SYNC_DIR_INT_CONFLICT //set if automatic synchronization cannot determine a direction
    };
    static SyncOperation getSyncOperation(const CompareFilesResult cmpResult,
                                          const bool selectedForSynchronization,
                                          const SyncDirectionIntern syncDir); //evaluate comparison result and sync direction

    bool selectedForSynchronization;
    SyncDirectionIntern syncDir;
    wxString syncOpConflictDescr; //only filled if syncDir == SYNC_DIR_INT_CONFLICT


    //buffer some redundant data:
    RelNamesBuffered nameBuffer; //base sync dirs + relative parent name: this does NOT belong into FileDescriptor/DirDescriptor

    bool existsLeft_;
    bool existsRight_;
    Zstring shortName_;
    HierarchyObject::ObjectID uniqueId;
};

//------------------------------------------------------------------
class DirMapping : public FileSystemObject, public HierarchyObject
{
public:
    virtual void accept(FSObjectVisitor& visitor) const;

    virtual CompareFilesResult getCategory() const;
    CompareDirResult getDirCategory() const; //returns actually used subset of CompareFilesResult
    virtual const wxString& getCatConflict() const;

private:
    friend class CompareProcess; //only CompareProcess shall be allowed to change cmpResult
    friend class HierarchyObject;
    virtual void swap();
    virtual void removeObjectL();
    virtual void removeObjectR();
    virtual void copyToL();
    virtual void copyToR();
    //------------------------------------------------------------------

    DirMapping(bool existsLeft,
               const Zstring& dirNameShort,
               bool existsRight,
               const HierarchyObject& parent) :
        FileSystemObject(existsLeft, existsRight, dirNameShort, parent),
        HierarchyObject(parent, dirNameShort),
        cmpResult(!existsRight ? DIR_LEFT_SIDE_ONLY : (existsLeft ? DIR_EQUAL : DIR_RIGHT_SIDE_ONLY)) {}

    //categorization
    CompareDirResult cmpResult;
};

//------------------------------------------------------------------
class FileMapping : public FileSystemObject
{
public:
    virtual void accept(FSObjectVisitor& visitor) const;

    template <SelectedSide side> const wxLongLong&  getLastWriteTime() const;
    template <SelectedSide side> const wxULongLong& getFileSize() const;
    template <SelectedSide side> const Zstring getExtension() const;

    virtual CompareFilesResult getCategory() const;
    virtual const wxString& getCatConflict() const;

private:
    friend class CompareProcess;  //only CompareProcess shall be allowed to change cmpResult
    friend class HierarchyObject; //construction

    template <CompareFilesResult res>
    void setCategory();
    void setCategoryConflict(const wxString& description);

    FileMapping(const FileDescriptor&  left, //file exists on both sides
                const Zstring&         fileNameShort,
                CompareFilesResult     defaultCmpResult,
                const FileDescriptor&  right,
                const HierarchyObject& parent) :
        FileSystemObject(true, true, fileNameShort, parent),
        cmpResult(defaultCmpResult),
        dataLeft(left),
        dataRight(right) {}
    FileMapping(const FileDescriptor&  left, //file exists on left side only
                const Zstring&         fileNameShort,
                const HierarchyObject& parent) :
        FileSystemObject(true, false, fileNameShort, parent),
        cmpResult(FILE_LEFT_SIDE_ONLY),
        dataLeft(left),
        dataRight(0, 0) {}
    FileMapping(const Zstring&         fileNameShort, //file exists on right side only
                const FileDescriptor&  right,
                const HierarchyObject& parent) :
        FileSystemObject(false, true, fileNameShort, parent),
        cmpResult(FILE_RIGHT_SIDE_ONLY),
        dataLeft(0, 0),
        dataRight(right) {}

    virtual void swap();
    virtual void removeObjectL();
    virtual void removeObjectR();
    virtual void copyToL();
    virtual void copyToR();
    //------------------------------------------------------------------

    //categorization
    CompareFilesResult cmpResult;
    wxString cmpConflictDescr; //only filled if cmpResult == FILE_CONFLICT

    FileDescriptor dataLeft;
    FileDescriptor dataRight;
};

//------------------------------------------------------------------
class SymLinkMapping : public FileSystemObject //this class models a TRUE symbolic link, i.e. one that is NEVER dereferenced: deref-links should be directly placed in class File/DirMapping
{
public:
    virtual void accept(FSObjectVisitor& visitor) const;

    template <SelectedSide side> const wxLongLong&  getLastWriteTime() const; //write time of the link, NOT target!
    template <SelectedSide side> LinkDescriptor::LinkType getLinkType() const;
    template <SelectedSide side> const Zstring& getTargetPath() const;

    virtual CompareFilesResult getCategory() const;
    CompareSymlinkResult getLinkCategory()   const; //returns actually used subset of CompareFilesResult
    virtual const wxString& getCatConflict() const;

private:
    friend class CompareProcess;  //only CompareProcess shall be allowed to change cmpResult
    friend class HierarchyObject; //construction
    virtual void swap();
    virtual void removeObjectL();
    virtual void removeObjectR();
    virtual void copyToL();
    virtual void copyToR();

    template <CompareSymlinkResult res>
    void setCategory();
    void setCategoryConflict(const wxString& description);

    SymLinkMapping(const LinkDescriptor& left, //link exists on both sides
                   const Zstring&           linkNameShort,
                   CompareSymlinkResult     defaultCmpResult,
                   const LinkDescriptor&    right,
                   const HierarchyObject&   parent) :
        FileSystemObject(true, true, linkNameShort, parent),
        cmpResult(defaultCmpResult),
        dataLeft(left),
        dataRight(right) {}
    SymLinkMapping(const LinkDescriptor& left, //link exists on left side only
                   const Zstring&           linkNameShort,
                   const HierarchyObject&    parent) :
        FileSystemObject(true, false, linkNameShort, parent),
        cmpResult(SYMLINK_LEFT_SIDE_ONLY),
        dataLeft(left),
        dataRight(0, Zstring(), LinkDescriptor::TYPE_FILE) {}
    SymLinkMapping(const Zstring&      linkNameShort, //link exists on right side only
                   const LinkDescriptor&  right,
                   const HierarchyObject& parent) :
        FileSystemObject(false, true, linkNameShort, parent),
        cmpResult(SYMLINK_RIGHT_SIDE_ONLY),
        dataLeft(0, Zstring(), LinkDescriptor::TYPE_FILE),
        dataRight(right) {}
    //------------------------------------------------------------------

    //categorization
    CompareSymlinkResult cmpResult;
    wxString cmpConflictDescr; //only filled if cmpResult == SYMLINK_CONFLICT

    LinkDescriptor dataLeft;
    LinkDescriptor dataRight;
};

//------------------------------------------------------------------
class BaseDirMapping : public HierarchyObject //synchronization base directory
{
public:
    BaseDirMapping(const Zstring& dirPostfixedLeft,
                   const Zstring& dirPostfixedRight,
                   const BaseFilter::FilterRef& filterIn) :
        HierarchyObject(dirPostfixedLeft, dirPostfixedRight),
        filter(filterIn) {}

    const BaseFilter::FilterRef& getFilter() const;
    template <SelectedSide side> Zstring getDBFilename() const;
    virtual void swap();

    //BaseDirMapping is created incrementally and not once via constructor => same for locks
    template <SelectedSide side> void holdLock(const DirLock& lock);

private:
    boost::shared_ptr<DirLock> leftDirLock;
    boost::shared_ptr<DirLock> rightDirLock;

    BaseFilter::FilterRef filter;
};


typedef std::vector<BaseDirMapping> FolderComparison;

//------------------------------------------------------------------

































//---------------Inline Implementation---------------------------------------------------
inline //inline virtual... admittedly its use may be limited
void FileMapping::accept(FSObjectVisitor& visitor) const
{
    visitor.visit(*this);
}


inline
void DirMapping::accept(FSObjectVisitor& visitor) const
{
    visitor.visit(*this);
}


inline
void SymLinkMapping::accept(FSObjectVisitor& visitor) const
{
    visitor.visit(*this);
}


inline
FileSystemObject* HierarchyObject::retrieveById(ObjectID id) //returns NULL if object is not found
{
    //code re-use of const method: see Meyers Effective C++
    return const_cast<FileSystemObject*>(static_cast<const HierarchyObject&>(*this).retrieveById(id));
}


inline
HierarchyObject::ObjectID FileSystemObject::getId() const
{
    return uniqueId;
}


inline
HierarchyObject::ObjectID FileSystemObject::getUniqueId()
{
    static HierarchyObject::ObjectID id = 0;
    return ++id;
}


inline
DirContainer::SubDirList::const_iterator DirContainer::dirBegin() const
{
    return subDirs.begin();
}


inline
DirContainer::SubDirList::const_iterator DirContainer::dirEnd() const
{
    return subDirs.end();
}


inline
DirContainer::SubFileList::const_iterator DirContainer::fileBegin() const
{
    return subFiles.begin();
}


inline
DirContainer::SubFileList::const_iterator DirContainer::fileEnd() const
{
    return subFiles.end();
}


inline
DirContainer::SubLinkList::const_iterator DirContainer::linkBegin() const
{
    return subLinks.begin();
}


inline
DirContainer::SubLinkList::const_iterator DirContainer::linkEnd() const
{
    return subLinks.end();
}


inline
size_t DirContainer::dirCount() const
{
    return subDirs.size();
}


inline
size_t DirContainer::fileCount() const
{
    return subFiles.size();
}


inline
size_t DirContainer::linkCount() const
{
    return subLinks.size();
}


inline
const DirContainer* DirContainer::findDir(const Zstring& shortName) const
{
    const SubDirList::const_iterator iter = subDirs.find(shortName);
    return iter == subDirs.end() ? NULL : &iter->second;
}


inline
const FileContainer* DirContainer::findFile(const Zstring& shortName) const
{
    const SubFileList::const_iterator iter = subFiles.find(shortName);
    return iter == subFiles.end() ? NULL : &iter->second;
}


inline
const SymLinkContainer* DirContainer::findLink(const Zstring& shortName) const
{
    const SubLinkList::const_iterator iter = subLinks.find(shortName);
    return iter == subLinks.end() ? NULL : &iter->second;
}


inline
const FileDescriptor& FileContainer::getData() const
{
    return data;
}


inline
const LinkDescriptor& SymLinkContainer::getData() const
{
    return data;
}


inline
DirContainer& DirContainer::addSubDir(const Zstring& shortName)
{
    return subDirs.insert(std::make_pair(shortName, DirContainer())).first->second;
}


inline
void DirContainer::addSubFile(const Zstring& shortName, const FileDescriptor& fileData)
{
    subFiles.insert(std::make_pair(shortName, FileContainer(fileData)));
}


inline
void DirContainer::addSubLink(const Zstring& shortName, const LinkDescriptor& linkData)
{
    subLinks.insert(std::make_pair(shortName, SymLinkContainer(linkData)));
}


inline
CompareFilesResult FileMapping::getCategory() const
{
    return cmpResult;
}


inline
const wxString& FileMapping::getCatConflict() const
{
    return cmpConflictDescr;
}


inline
CompareFilesResult DirMapping::getCategory() const
{
    return convertToFilesResult(cmpResult);
}


inline
CompareDirResult DirMapping::getDirCategory() const
{
    return cmpResult;
}


inline
const wxString& DirMapping::getCatConflict() const
{
    static wxString empty;
    return empty;
}


inline
void FileSystemObject::setSyncDir(SyncDirection newDir)
{
    syncDir = static_cast<SyncDirectionIntern>(newDir); //should be safe by design
}


inline
const wxString& FileSystemObject::getSyncOpConflict() const
{
    //a sync operation conflict can occur when:
    //1. category-conflict and syncDir == NONE -> problem finding category
    //2. syncDir == SYNC_DIR_INT_CONFLICT -> problem finding sync direction
    return syncDir == SYNC_DIR_INT_CONFLICT ? syncOpConflictDescr : getCatConflict();
}


inline
void FileSystemObject::setSyncDirConflict(const wxString& description) //set syncDir = SYNC_DIR_INT_CONFLICT
{
    syncDir = SYNC_DIR_INT_CONFLICT;
    syncOpConflictDescr = description;
}


inline
bool FileSystemObject::isActive() const
{
    return selectedForSynchronization;
}


inline
void FileSystemObject::setActive(bool active)
{
    selectedForSynchronization = active;
}


inline
SyncOperation FileSystemObject::getSyncOperation() const
{
    return getSyncOperation(getCategory(), selectedForSynchronization, syncDir);
}


inline
SyncOperation FileSystemObject::testSyncOperation(bool selected, SyncDirection proposedDir) const
{
    return getSyncOperation(getCategory(), selected, static_cast<SyncDirectionIntern>(proposedDir)); //should be safe by design
}


template <>
inline
bool FileSystemObject::isEmpty<LEFT_SIDE>() const
{
    return !existsLeft_;
}


template <>
inline
bool FileSystemObject::isEmpty<RIGHT_SIDE>() const
{
    return !existsRight_;
}


inline
bool FileSystemObject::isEmpty() const
{
    return isEmpty<LEFT_SIDE>() && isEmpty<RIGHT_SIDE>();
}


template <SelectedSide side>
inline
const Zstring& FileSystemObject::getShortName() const
{
    static Zstring null;
    return isEmpty<side>() ? null : shortName_;
}


template <SelectedSide side>
inline
const Zstring FileSystemObject::getRelativeName() const
{
    return isEmpty<side>() ? Zstring() : nameBuffer.parentRelNamePf + shortName_;
}


inline
const Zstring FileSystemObject::getObjRelativeName() const
{
    return nameBuffer.parentRelNamePf + shortName_;
}


inline
const Zstring& FileSystemObject::getObjShortName() const
{
    return shortName_;
}


inline
const Zstring FileSystemObject::getParentRelativeName() const
{
    return nameBuffer.parentRelNamePf.BeforeLast(common::FILE_NAME_SEPARATOR);  //returns empty string if char not found
}


template <SelectedSide side>
inline
const Zstring FileSystemObject::getFullName() const
{
    return isEmpty<side>() ? Zstring() : getBaseDirPf<side>() + nameBuffer.parentRelNamePf + shortName_;
}


template <>
inline
const Zstring& FileSystemObject::getBaseDirPf<LEFT_SIDE>() const
{
    return nameBuffer.baseDirPfL;
}


template <>
inline
const Zstring& FileSystemObject::getBaseDirPf<RIGHT_SIDE>() const
{
    return nameBuffer.baseDirPfR;
}


template <>
inline
void FileSystemObject::removeObject<LEFT_SIDE>()
{
    existsLeft_ = false;
    removeObjectL();
}


template <>
inline
void FileSystemObject::removeObject<RIGHT_SIDE>()
{
    existsRight_ = false;
    removeObjectR();
}


inline
void FileSystemObject::synchronizeSides()
{
    switch (syncDir)
    {
    case SYNC_DIR_INT_LEFT:
        existsLeft_ = existsRight_;
        copyToL();
        break;
    case SYNC_DIR_INT_RIGHT:
        existsRight_ = existsLeft_;
        copyToR();
        break;
    case SYNC_DIR_INT_NONE:
    case SYNC_DIR_INT_CONFLICT:
        assert(!"if nothing's todo then why arrive here?");
        break;
    }

    syncDir = SYNC_DIR_INT_NONE;
}


inline
void FileSystemObject::swap()
{
    std::swap(nameBuffer.baseDirPfL, nameBuffer.baseDirPfR);
    std::swap(existsLeft_, existsRight_);
}


inline
void HierarchyObject::swap()
{
    std::swap(baseDirLeft, baseDirRight);

    //files
    std::for_each(subFiles.begin(), subFiles.end(), std::mem_fun_ref(&FileMapping::swap));
    //directories
    std::for_each(subDirs.begin(), subDirs.end(), std::mem_fun_ref(&DirMapping::swap));
    //symbolic links
    std::for_each(subLinks.begin(), subLinks.end(), std::mem_fun_ref(&SymLinkMapping::swap));
}


inline
const Zstring& HierarchyObject::getRelativeNamePf() const
{
    return relNamePf;
}


inline
DirMapping& HierarchyObject::addSubDir(bool existsLeft,
                                       const Zstring& dirNameShort,
                                       bool existsRight)
{
    subDirs.push_back(DirMapping(existsLeft, dirNameShort, existsRight, *this));
    return subDirs.back();
}


inline
FileMapping& HierarchyObject::addSubFile(const FileDescriptor&   left,          //file exists on both sides
        const Zstring&          fileNameShort,
        CompareFilesResult      defaultCmpResult,
        const FileDescriptor&   right)
{
    subFiles.push_back(FileMapping(left, fileNameShort, defaultCmpResult, right, *this));
    return subFiles.back();
}


inline
void HierarchyObject::addSubFile(const FileDescriptor&   left,          //file exists on left side only
                                 const Zstring&          fileNameShort)
{
    subFiles.push_back(FileMapping(left, fileNameShort, *this));
}


inline
void HierarchyObject::addSubFile(const Zstring&          fileNameShort, //file exists on right side only
                                 const FileDescriptor&   right)
{
    subFiles.push_back(FileMapping(fileNameShort, right, *this));
}


inline
SymLinkMapping& HierarchyObject::addSubLink(const LinkDescriptor& left,          //link exists on both sides
        const Zstring&          linkNameShort,
        CompareSymlinkResult      defaultCmpResult,
        const LinkDescriptor&   right)
{
    subLinks.push_back(SymLinkMapping(left, linkNameShort, defaultCmpResult, right, *this));
    return subLinks.back();
}


inline
void HierarchyObject::addSubLink(const LinkDescriptor& left,          //link exists on left side only
                                 const Zstring&        linkNameShort)
{
    subLinks.push_back(SymLinkMapping(left, linkNameShort, *this));
}


inline
void HierarchyObject::addSubLink(const Zstring&        linkNameShort, //link exists on right side only
                                 const LinkDescriptor& right)
{
    subLinks.push_back(SymLinkMapping(linkNameShort, right, *this));
}


inline
const HierarchyObject::SubDirMapping& HierarchyObject::useSubDirs() const
{
    return subDirs;
}


inline
const HierarchyObject::SubFileMapping& HierarchyObject::useSubFiles() const
{
    return subFiles;
}


inline
const HierarchyObject::SubLinkMapping& HierarchyObject::useSubLinks() const
{
    return subLinks;
}


inline
HierarchyObject::SubDirMapping& HierarchyObject::useSubDirs()
{
    return const_cast<SubDirMapping&>(static_cast<const HierarchyObject*>(this)->useSubDirs());
}


inline
HierarchyObject::SubFileMapping& HierarchyObject::useSubFiles()
{
    return const_cast<SubFileMapping&>(static_cast<const HierarchyObject*>(this)->useSubFiles());
}


inline
HierarchyObject::SubLinkMapping& HierarchyObject::useSubLinks()
{
    return const_cast<SubLinkMapping&>(static_cast<const HierarchyObject*>(this)->useSubLinks());
}


inline
void BaseDirMapping::swap()
{
    //call base class versions
    HierarchyObject::swap();
}


inline
void DirMapping::swap()
{
    //call base class versions
    HierarchyObject::swap();
    FileSystemObject::swap();

    //swap compare result
    switch (cmpResult)
    {
    case DIR_LEFT_SIDE_ONLY:
        cmpResult = DIR_RIGHT_SIDE_ONLY;
        break;
    case DIR_RIGHT_SIDE_ONLY:
        cmpResult = DIR_LEFT_SIDE_ONLY;
        break;
    case DIR_EQUAL:
        break;
    }
}


inline
void DirMapping::removeObjectL()
{
    cmpResult = DIR_RIGHT_SIDE_ONLY;
    std::for_each(useSubFiles().begin(), useSubFiles().end(), std::mem_fun_ref(&FileSystemObject::removeObject<LEFT_SIDE>));
    std::for_each(useSubLinks().begin(), useSubLinks().end(), std::mem_fun_ref(&FileSystemObject::removeObject<LEFT_SIDE>));
    std::for_each(useSubDirs(). begin(), useSubDirs() .end(), std::mem_fun_ref(&FileSystemObject::removeObject<LEFT_SIDE>));
}


inline
void DirMapping::removeObjectR()
{
    cmpResult = DIR_LEFT_SIDE_ONLY;
    std::for_each(useSubFiles().begin(), useSubFiles().end(), std::mem_fun_ref(&FileSystemObject::removeObject<RIGHT_SIDE>));
    std::for_each(useSubLinks().begin(), useSubLinks().end(), std::mem_fun_ref(&FileSystemObject::removeObject<RIGHT_SIDE>));
    std::for_each(useSubDirs(). begin(), useSubDirs(). end(), std::mem_fun_ref(&FileSystemObject::removeObject<RIGHT_SIDE>));
}


inline
void DirMapping::copyToL()
{
    cmpResult = DIR_EQUAL;
}


inline
void DirMapping::copyToR()
{
    cmpResult = DIR_EQUAL;
}


inline
const BaseFilter::FilterRef& BaseDirMapping::getFilter() const
{
    return filter;
}


template <SelectedSide side>
inline
Zstring BaseDirMapping::getDBFilename() const
{
    return getBaseDir<side>() + getSyncDBFilename();
}


template <>
inline
void BaseDirMapping::holdLock<LEFT_SIDE>(const DirLock& lock)
{
    leftDirLock.reset(new DirLock(lock));
}


template <>
inline
void BaseDirMapping::holdLock<RIGHT_SIDE>(const DirLock& lock)
{
    rightDirLock.reset(new DirLock(lock));
}


inline
void FileMapping::swap()
{
    //call base class version
    FileSystemObject::swap();

    //swap compare result
    switch (cmpResult)
    {
    case FILE_LEFT_SIDE_ONLY:
        cmpResult = FILE_RIGHT_SIDE_ONLY;
        break;
    case FILE_RIGHT_SIDE_ONLY:
        cmpResult = FILE_LEFT_SIDE_ONLY;
        break;
    case FILE_LEFT_NEWER:
        cmpResult = FILE_RIGHT_NEWER;
        break;
    case FILE_RIGHT_NEWER:
        cmpResult = FILE_LEFT_NEWER;
        break;
    case FILE_DIFFERENT:
    case FILE_EQUAL:
    case FILE_CONFLICT:
        break;
    }

    std::swap(dataLeft, dataRight);
}


template <CompareFilesResult res>
inline
void FileMapping::setCategory()
{
    cmpResult = res;
}

template <>
inline
void FileMapping::setCategory<FILE_CONFLICT>(); //if conflict is detected, use setCategoryConflict! => method is not defined!


inline
void FileMapping::setCategoryConflict(const wxString& description)
{
    cmpResult = FILE_CONFLICT;
    cmpConflictDescr = description;
}


inline
void FileMapping::removeObjectL()
{
    cmpResult = FILE_RIGHT_SIDE_ONLY;
    dataLeft  = FileDescriptor(0, 0);
}


inline
void FileMapping::removeObjectR()
{
    cmpResult = FILE_LEFT_SIDE_ONLY;
    dataRight = FileDescriptor(0, 0);
}


inline
void FileMapping::copyToL()
{
    cmpResult = FILE_EQUAL;
    dataLeft = dataRight;
    //util::FileID()); //attention! do not copy FileID! It is retained on file renaming only!
}


inline
void FileMapping::copyToR()
{
    cmpResult = FILE_EQUAL;
    dataRight = dataLeft;
    //util::FileID()); //attention! do not copy FileID! It is retained on file renaming only!
}


template <>
inline
const wxLongLong& FileMapping::getLastWriteTime<LEFT_SIDE>() const
{
    return dataLeft.lastWriteTimeRaw;
}


template <>
inline
const wxLongLong& FileMapping::getLastWriteTime<RIGHT_SIDE>() const
{
    return dataRight.lastWriteTimeRaw;
}


template <>
inline
const wxULongLong& FileMapping::getFileSize<LEFT_SIDE>() const
{
    return dataLeft.fileSize;
}


template <>
inline
const wxULongLong& FileMapping::getFileSize<RIGHT_SIDE>() const
{
    return dataRight.fileSize;
}


template <SelectedSide side>
inline
const Zstring FileMapping::getExtension() const
{
    //attention: Zstring::AfterLast() returns whole string if char not found! -> don't use
    const Zstring& shortName = getShortName<side>();

    const size_t pos = shortName.rfind(Zchar('.'));
    return pos == Zstring::npos ?
           Zstring() :
           Zstring(shortName.c_str() + pos + 1);
}


template <>
inline
const wxLongLong& SymLinkMapping::getLastWriteTime<LEFT_SIDE>() const
{
    return dataLeft.lastWriteTimeRaw;
}


template <>
inline
const wxLongLong& SymLinkMapping::getLastWriteTime<RIGHT_SIDE>() const
{
    return dataRight.lastWriteTimeRaw;
}


template <>
inline

LinkDescriptor::LinkType SymLinkMapping::getLinkType<LEFT_SIDE>() const
{
    return dataLeft.type;
}


template <>
inline
LinkDescriptor::LinkType SymLinkMapping::getLinkType<RIGHT_SIDE>() const
{
    return dataRight.type;
}


template <>
inline
const Zstring& SymLinkMapping::getTargetPath<LEFT_SIDE>() const
{
    return dataLeft.targetPath;
}


template <>
inline
const Zstring& SymLinkMapping::getTargetPath<RIGHT_SIDE>() const
{
    return dataRight.targetPath;
}


inline
CompareFilesResult SymLinkMapping::getCategory() const
{
    return convertToFilesResult(cmpResult);
}


inline
CompareSymlinkResult SymLinkMapping::getLinkCategory() const
{
    return cmpResult;
}


inline
const wxString& SymLinkMapping::getCatConflict() const
{
    return cmpConflictDescr;
}


inline
void SymLinkMapping::swap()
{
    //call base class versions
    FileSystemObject::swap();

    //swap compare result
    switch (cmpResult)
    {
    case SYMLINK_LEFT_SIDE_ONLY:
        cmpResult = SYMLINK_RIGHT_SIDE_ONLY;
        break;
    case SYMLINK_RIGHT_SIDE_ONLY:
        cmpResult = SYMLINK_LEFT_SIDE_ONLY;
        break;
    case SYMLINK_LEFT_NEWER:
        cmpResult = SYMLINK_RIGHT_NEWER;
        break;
    case SYMLINK_RIGHT_NEWER:
        cmpResult = SYMLINK_LEFT_NEWER;
        break;
    case SYMLINK_EQUAL:
    case SYMLINK_DIFFERENT:
    case SYMLINK_CONFLICT:
        break;
    }

    std::swap(dataLeft, dataRight);
}


inline
void SymLinkMapping::removeObjectL()
{
    cmpResult = SYMLINK_RIGHT_SIDE_ONLY;
    dataLeft  = LinkDescriptor(0, Zstring(), LinkDescriptor::TYPE_FILE);
}


inline
void SymLinkMapping::removeObjectR()
{
    cmpResult = SYMLINK_LEFT_SIDE_ONLY;
    dataRight = LinkDescriptor(0, Zstring(), LinkDescriptor::TYPE_FILE);
}


inline
void SymLinkMapping::copyToL()
{
    cmpResult = SYMLINK_EQUAL;
    dataLeft = dataRight;
}


inline
void SymLinkMapping::copyToR()
{
    cmpResult = SYMLINK_EQUAL;
    dataRight = dataLeft;
}


template <CompareSymlinkResult res>
inline
void SymLinkMapping::setCategory()
{
    cmpResult = res;
}


template <>
inline
void SymLinkMapping::setCategory<SYMLINK_CONFLICT>(); //if conflict is detected, use setCategoryConflict! => method is not defined!


inline
void SymLinkMapping::setCategoryConflict(const wxString& description)
{
    cmpResult = SYMLINK_CONFLICT;
    cmpConflictDescr = description;
}

}

#endif // FILEHIERARCHY_H_INCLUDED

