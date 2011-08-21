// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) 2008-2011 ZenJu (zhnmju123 AT gmx.de)                    *
// **************************************************************************

#ifndef FREEFILESYNC_H_INCLUDED
#define FREEFILESYNC_H_INCLUDED

#include <wx/string.h>
#include <vector>
#include "shared/zstring.h"
#include "shared/assert_static.h"
#include <memory>
#include "shared/int64.h"



namespace zen
{
enum CompareVariant
{
    CMP_BY_TIME_SIZE,
    CMP_BY_CONTENT
};

wxString getVariantName(CompareVariant var);


enum SyncDirection
{
    SYNC_DIR_LEFT  = 0,
    SYNC_DIR_RIGHT,
    SYNC_DIR_NONE //NOTE: align with SyncDirectionIntern before adding anything here!
};


enum CompareFilesResult
{
    FILE_LEFT_SIDE_ONLY = 0,
    FILE_RIGHT_SIDE_ONLY,
    FILE_LEFT_NEWER,
    FILE_RIGHT_NEWER,
    FILE_DIFFERENT,
    FILE_EQUAL,
    FILE_DIFFERENT_METADATA, //both sides equal, but different metadata only
    FILE_CONFLICT
};
//attention make sure these /|\  \|/ three enums match!!!
enum CompareDirResult
{
    DIR_LEFT_SIDE_ONLY     = FILE_LEFT_SIDE_ONLY,
    DIR_RIGHT_SIDE_ONLY    = FILE_RIGHT_SIDE_ONLY,
    DIR_EQUAL              = FILE_EQUAL,
    DIR_DIFFERENT_METADATA = FILE_DIFFERENT_METADATA //both sides equal, but different metadata only
};

enum CompareSymlinkResult
{
    SYMLINK_LEFT_SIDE_ONLY  = FILE_LEFT_SIDE_ONLY,
    SYMLINK_RIGHT_SIDE_ONLY = FILE_RIGHT_SIDE_ONLY,
    SYMLINK_LEFT_NEWER      = FILE_LEFT_NEWER,
    SYMLINK_RIGHT_NEWER     = FILE_RIGHT_NEWER,
    SYMLINK_DIFFERENT       = FILE_DIFFERENT,
    SYMLINK_EQUAL           = FILE_EQUAL,
    SYMLINK_DIFFERENT_METADATA = FILE_DIFFERENT_METADATA, //files are considered "equal" but different only in metadata
    SYMLINK_CONFLICT        = FILE_CONFLICT
};


inline
CompareFilesResult convertToFilesResult(CompareDirResult value)
{
    return static_cast<CompareFilesResult>(value);
}


inline
CompareFilesResult convertToFilesResult(CompareSymlinkResult value)
{
    return static_cast<CompareFilesResult>(value);
}


wxString getDescription(CompareFilesResult cmpRes);
wxString getSymbol(CompareFilesResult cmpRes);


enum SyncOperation
{
    SO_CREATE_NEW_LEFT,
    SO_CREATE_NEW_RIGHT,
    SO_DELETE_LEFT,
    SO_DELETE_RIGHT,
    SO_OVERWRITE_LEFT,
    SO_OVERWRITE_RIGHT,
    SO_COPY_METADATA_TO_LEFT, //objects are already equal: transfer metadata only
    SO_COPY_METADATA_TO_RIGHT, //
    SO_DO_NOTHING, //= both sides differ, but nothing will be synced
    SO_EQUAL,      //= both sides are equal, so nothing will be synced
    SO_UNRESOLVED_CONFLICT
};

wxString getDescription(SyncOperation op);
wxString getSymbol(SyncOperation op);


struct DirectionSet
{
    DirectionSet() :
        exLeftSideOnly( SYNC_DIR_RIGHT),
        exRightSideOnly(SYNC_DIR_LEFT),
        leftNewer(      SYNC_DIR_RIGHT),
        rightNewer(     SYNC_DIR_LEFT),
        different(      SYNC_DIR_NONE),
        conflict(       SYNC_DIR_NONE) {}

    SyncDirection exLeftSideOnly;
    SyncDirection exRightSideOnly;
    SyncDirection leftNewer;
    SyncDirection rightNewer;
    SyncDirection different;
    SyncDirection conflict;
};

DirectionSet getTwoWaySet();

inline
bool operator==(const DirectionSet& lhs, const DirectionSet& rhs)
{
    return lhs.exLeftSideOnly  == rhs.exLeftSideOnly  &&
           lhs.exRightSideOnly == rhs.exRightSideOnly &&
           lhs.leftNewer       == rhs.leftNewer       &&
           lhs.rightNewer      == rhs.rightNewer      &&
           lhs.different       == rhs.different       &&
           lhs.conflict        == rhs.conflict;
}

struct SyncConfig //technical representation of sync-config
{
    enum Variant
    {
        AUTOMATIC, //use sync-database to determine directions
        MIRROR,  //predefined
        UPDATE,  //
        CUSTOM    //use custom directions
    };

    SyncConfig() : var(AUTOMATIC) {}

    Variant var;

    //custom sync directions
    DirectionSet custom;
};

inline
bool operator==(const SyncConfig& lhs, const SyncConfig& rhs)
{
    return lhs.var == rhs.var &&
           (lhs.var != SyncConfig::CUSTOM ||  lhs.custom == rhs.custom); //directions are only relevant if variant "custom" is active
}

//get sync directions: DON'T call for variant AUTOMATIC!
DirectionSet extractDirections(const SyncConfig& cfg);

wxString getVariantName(SyncConfig::Variant var);



enum DeletionPolicy
{
    DELETE_PERMANENTLY = 5,
    MOVE_TO_RECYCLE_BIN,
    MOVE_TO_CUSTOM_DIRECTORY
};


struct AlternateSyncConfig
{
    AlternateSyncConfig(const SyncConfig&    syncCfg,
                        const DeletionPolicy handleDel,
                        const wxString&      customDelDir) :
        syncConfiguration(syncCfg),
        handleDeletion(handleDel),
        customDeletionDirectory(customDelDir) {};

    AlternateSyncConfig() :  //construct with default values
        handleDeletion(MOVE_TO_RECYCLE_BIN) {}

    //Synchronisation settings
    SyncConfig syncConfiguration;

    //misc options
    DeletionPolicy handleDeletion; //use Recycle, delete permanently or move to user-defined location
    wxString customDeletionDirectory;
};

inline
bool operator==(const AlternateSyncConfig& lhs, const AlternateSyncConfig& rhs)
{
    return lhs.syncConfiguration       == rhs.syncConfiguration &&
           lhs.handleDeletion          == rhs.handleDeletion    &&
           lhs.customDeletionDirectory == rhs.customDeletionDirectory;
}


enum UnitSize
{
    USIZE_NONE,
    USIZE_BYTE,
    USIZE_KB,
    USIZE_MB
};

enum UnitTime
{
    UTIME_NONE,
    UTIME_SEC,
    UTIME_MIN,
    UTIME_HOUR,
    UTIME_DAY
};

struct FilterConfig
{
    FilterConfig(const Zstring& include  = Zstr("*"),
                 const Zstring& exclude  = Zstring(),
                 size_t   timeSpanIn     = 0,
                 UnitTime unitTimeSpanIn = UTIME_NONE,
                 size_t   sizeMinIn      = 0,
                 UnitSize unitSizeMinIn  = USIZE_NONE,
                 size_t   sizeMaxIn      = 0,
                 UnitSize unitSizeMaxIn  = USIZE_NONE) :
        includeFilter(include),
        excludeFilter(exclude),
        timeSpan     (timeSpanIn),
        unitTimeSpan (unitTimeSpanIn),
        sizeMin      (sizeMinIn),
        unitSizeMin  (unitSizeMinIn),
        sizeMax      (sizeMaxIn),
        unitSizeMax  (unitSizeMaxIn) {}

    /*
    Semantics of HardFilter:
    1. using it creates a NEW folder hierarchy! -> must be considered by <Automatic>-mode! (fortunately it turns out, doing nothing already has perfect semantics :)
    2. it applies equally to both sides => it always matches either both sides or none! => can be used while traversing a single folder!
    */
    Zstring includeFilter;
    Zstring excludeFilter;

    /*
    Semantics of SoftFilter:
    1. It potentially may match only one side => it MUST NOT be applied while traversing a single folder to avoid mismatches
    2. => it is applied after traversing and just marks rows, (NO deletions after comparison are allowed)
    3. => equivalent to a user temporarily (de-)selecting rows -> not relevant for <Automatic>-mode! ;)
    */
    size_t timeSpan;
    UnitTime unitTimeSpan;

    size_t sizeMin;
    UnitSize unitSizeMin;

    size_t sizeMax;
    UnitSize unitSizeMax;
};

inline
bool operator==(const FilterConfig& lhs, const FilterConfig& rhs)
{
    return lhs.includeFilter == rhs.includeFilter &&
           lhs.excludeFilter == rhs.excludeFilter &&
           lhs.timeSpan      == rhs.timeSpan      &&
           lhs.unitTimeSpan  == rhs.unitTimeSpan  &&
           lhs.sizeMin       == rhs.sizeMin       &&
           lhs.unitSizeMin   == rhs.unitSizeMin   &&
           lhs.sizeMax       == rhs.sizeMax       &&
           lhs.unitSizeMax   == rhs.unitSizeMax;
}

void resolveUnits(size_t timeSpan, UnitTime unitTimeSpan,
                  size_t sizeMin,  UnitSize unitSizeMin,
                  size_t sizeMax,  UnitSize unitSizeMax,
                  zen::Int64&  timeSpanSec, //unit: seconds
                  zen::UInt64& sizeMinBy,   //unit: bytes
                  zen::UInt64& sizeMaxBy);  //unit: bytes


struct FolderPairEnh //enhanced folder pairs with (optional) alternate configuration
{
    FolderPairEnh() {}

    FolderPairEnh(const Zstring& leftDir,
                  const Zstring& rightDir,
                  const std::shared_ptr<const AlternateSyncConfig>& syncConfig,
                  const FilterConfig& filter) :
        leftDirectory(leftDir),
        rightDirectory(rightDir) ,
        altSyncConfig(syncConfig),
        localFilter(filter) {}

    Zstring leftDirectory;
    Zstring rightDirectory;

    std::shared_ptr<const AlternateSyncConfig> altSyncConfig; //optional
    FilterConfig localFilter;
};


inline
bool operator==(const FolderPairEnh& lhs, const FolderPairEnh& rhs)
{
    return lhs.leftDirectory  == rhs.leftDirectory  &&
           lhs.rightDirectory == rhs.rightDirectory &&

           (lhs.altSyncConfig.get() && rhs.altSyncConfig.get() ?
            *lhs.altSyncConfig == *rhs.altSyncConfig :
            lhs.altSyncConfig.get() == rhs.altSyncConfig.get()) &&

           lhs.localFilter == rhs.localFilter;
}

enum SymLinkHandling
{
    SYMLINK_IGNORE,
    SYMLINK_USE_DIRECTLY,
    SYMLINK_FOLLOW_LINK
};


struct MainConfiguration
{
    MainConfiguration() :
        compareVar(CMP_BY_TIME_SIZE),
        handleSymlinks(SYMLINK_IGNORE),
#ifdef FFS_WIN
        globalFilter(Zstr("*"),
                     Zstr("\
\\System Volume Information\\\n\
\\RECYCLED\\\n\
\\RECYCLER\\\n\
\\$Recycle.Bin\\")),
#elif defined FFS_LINUX
        globalFilter(Zstr("*"),
                     Zstr("/.Trash-*/\n\
                          /.recycle/")),
#endif
        handleDeletion(MOVE_TO_RECYCLE_BIN) {}

    FolderPairEnh firstPair; //there needs to be at least one pair!
    std::vector<FolderPairEnh> additionalPairs;

    //Compare setting
    CompareVariant compareVar;

    SymLinkHandling handleSymlinks;

    //GLOBAL filter settings
    FilterConfig globalFilter;

    //Synchronisation settings
    SyncConfig syncConfiguration;
    DeletionPolicy handleDeletion; //use Recycle, delete permanently or move to user-defined location
    wxString customDeletionDirectory;

    wxString getSyncVariantName();
};


inline
bool operator==(const MainConfiguration& lhs, const MainConfiguration& rhs)
{
    return lhs.firstPair         == rhs.firstPair         &&
           lhs.additionalPairs   == rhs.additionalPairs   &&
           lhs.compareVar        == rhs.compareVar        &&
           lhs.handleSymlinks    == rhs.handleSymlinks    &&
           lhs.syncConfiguration == rhs.syncConfiguration &&
           lhs.globalFilter      == rhs.globalFilter      &&
           lhs.handleDeletion    == rhs.handleDeletion    &&
           lhs.customDeletionDirectory == rhs.customDeletionDirectory;
}

//facilitate drag & drop config merge:
MainConfiguration merge(const std::vector<MainConfiguration>& mainCfgs);
}

#endif // FREEFILESYNC_H_INCLUDED
