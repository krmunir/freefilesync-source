#include "structures.h"
#include "shared/fileHandling.h"
#include <wx/intl.h>
#include "shared/systemConstants.h"

using FreeFileSync::SyncConfiguration;
using FreeFileSync::MainConfiguration;


Zstring FreeFileSync::standardExcludeFilter()
{
#ifdef FFS_WIN
    static Zstring exclude(wxT("\
\\System Volume Information\\\n\
\\RECYCLER\\\n\
\\RECYCLED\\\n\
\\$Recycle.Bin\\")); //exclude Recycle Bin
#elif defined FFS_LINUX
    static Zstring exclude; //exclude nothing
#endif
    return exclude;
}


bool FreeFileSync::recycleBinExistsWrap()
{
    return recycleBinExists();
}


wxString FreeFileSync::getVariantName(CompareVariant var)
{
    switch (var)
    {
    case CMP_BY_CONTENT:
        return _("File content");
    case CMP_BY_TIME_SIZE:
        return _("File size and date");
    }

    assert(false);
    return wxEmptyString;
}


SyncConfiguration::Variant SyncConfiguration::getVariant() const
{
    if (automatic == true)
        return AUTOMATIC;  //automatic mode

    if (    exLeftSideOnly  == SYNC_DIR_RIGHT &&
            exRightSideOnly == SYNC_DIR_RIGHT &&
            leftNewer       == SYNC_DIR_RIGHT &&
            rightNewer      == SYNC_DIR_RIGHT &&
            different       == SYNC_DIR_RIGHT &&
            conflict        == SYNC_DIR_RIGHT)
        return MIRROR;    //one way ->

    else if (exLeftSideOnly  == SYNC_DIR_RIGHT &&
             exRightSideOnly == SYNC_DIR_NONE  &&
             leftNewer       == SYNC_DIR_RIGHT &&
             rightNewer      == SYNC_DIR_NONE  &&
             different       == SYNC_DIR_NONE  &&
             conflict        == SYNC_DIR_NONE)
        return UPDATE;    //Update ->

    else if (exLeftSideOnly  == SYNC_DIR_RIGHT &&
             exRightSideOnly == SYNC_DIR_LEFT  &&
             leftNewer       == SYNC_DIR_RIGHT &&
             rightNewer      == SYNC_DIR_LEFT  &&
             different       == SYNC_DIR_NONE  &&
             conflict        == SYNC_DIR_NONE)
        return TWOWAY;    //two way <->
    else
        return CUSTOM;    //other
}


void SyncConfiguration::setVariant(const Variant var)
{
    switch (var)
    {
    case AUTOMATIC:
        automatic = true;
        break;
    case MIRROR:
        automatic = false;
        exLeftSideOnly  = SYNC_DIR_RIGHT;
        exRightSideOnly = SYNC_DIR_RIGHT;
        leftNewer       = SYNC_DIR_RIGHT;
        rightNewer      = SYNC_DIR_RIGHT;
        different       = SYNC_DIR_RIGHT;
        conflict        = SYNC_DIR_RIGHT;
        break;
    case UPDATE:
        automatic = false;
        exLeftSideOnly  = SYNC_DIR_RIGHT;
        exRightSideOnly = SYNC_DIR_NONE;
        leftNewer       = SYNC_DIR_RIGHT;
        rightNewer      = SYNC_DIR_NONE;
        different       = SYNC_DIR_NONE;
        conflict        = SYNC_DIR_NONE;
        break;
    case TWOWAY:
        automatic = false;
        exLeftSideOnly  = SYNC_DIR_RIGHT;
        exRightSideOnly = SYNC_DIR_LEFT;
        leftNewer       = SYNC_DIR_RIGHT;
        rightNewer      = SYNC_DIR_LEFT;
        different       = SYNC_DIR_NONE;
        conflict        = SYNC_DIR_NONE;
        break;
    case CUSTOM:
        assert(false);
        break;
    }
}


wxString SyncConfiguration::getVariantName() const
{
    switch (getVariant())
    {
    case SyncConfiguration::AUTOMATIC:
        return _("<Automatic>");
    case SyncConfiguration::MIRROR:
        return _("Mirror ->>");
    case SyncConfiguration::UPDATE:
        return _("Update ->");
    case SyncConfiguration::TWOWAY:
        return _("Two way <->");
    case SyncConfiguration::CUSTOM:
        return _("Custom");
    }
    return _("Error");
}


wxString MainConfiguration::getSyncVariantName()
{
    const SyncConfiguration firstSyncCfg =
        firstPair.altSyncConfig.get() ?
        firstPair.altSyncConfig->syncConfiguration :
        syncConfiguration; //fallback to main sync cfg

    const SyncConfiguration::Variant firstVariant = firstSyncCfg.getVariant();

    //test if there's a deviating variant within the additional folder pairs
    for (std::vector<FolderPairEnh>::const_iterator i = additionalPairs.begin(); i != additionalPairs.end(); ++i)
    {
        const SyncConfiguration::Variant thisVariant =
            i->altSyncConfig.get() ?
            i->altSyncConfig->syncConfiguration.getVariant() :
            syncConfiguration.getVariant();

        if (thisVariant != firstVariant)
            return _("Multiple...");
    }

    //seems to be all in sync...
    return firstSyncCfg.getVariantName();
}


wxString FreeFileSync::getDescription(CompareFilesResult cmpRes)
{
    switch (cmpRes)
    {
    case FILE_LEFT_SIDE_ONLY:
        return _("Files/folders that exist on left side only");
    case FILE_RIGHT_SIDE_ONLY:
        return _("Files/folders that exist on right side only");
    case FILE_LEFT_NEWER:
        return _("Files that exist on both sides, left one is newer");
    case FILE_RIGHT_NEWER:
        return _("Files that exist on both sides, right one is newer");
    case FILE_DIFFERENT:
        return _("Files that exist on both sides and have different content");
    case FILE_EQUAL:
        return _("Files that are equal on both sides");
    case FILE_CONFLICT:
        return _("Conflicts/files that cannot be categorized");
    }

    assert(false);
    return wxEmptyString;
}


wxString FreeFileSync::getSymbol(CompareFilesResult cmpRes)
{
    switch (cmpRes)
    {
    case FILE_LEFT_SIDE_ONLY:
        return wxT("<|");
    case FILE_RIGHT_SIDE_ONLY:
        return wxT("|>");
    case FILE_LEFT_NEWER:
        return wxT("<<");
    case FILE_RIGHT_NEWER:
        return wxT(">>");
    case FILE_DIFFERENT:
        return wxT("!=");
    case FILE_EQUAL:
        return wxT("'=="); //added quotation mark to avoid error in Excel cell when exporting to *.cvs
    case FILE_CONFLICT:
        return wxT("\\/\\->");
    }

    assert(false);
    return wxEmptyString;
}


wxString FreeFileSync::getDescription(SyncOperation op)
{
    switch (op)
    {
    case SO_CREATE_NEW_LEFT:
        return _("Copy from right to left");
    case SO_CREATE_NEW_RIGHT:
        return _("Copy from left to right");
    case SO_DELETE_LEFT:
        return _("Delete files/folders existing on left side only");
    case SO_DELETE_RIGHT:
        return _("Delete files/folders existing on right side only");
    case SO_OVERWRITE_LEFT:
        return _("Copy from right to left overwriting");
    case SO_OVERWRITE_RIGHT:
        return _("Copy from left to right overwriting");
    case SO_DO_NOTHING:
        return _("Do nothing");
    case SO_UNRESOLVED_CONFLICT:
        return _("Conflicts/files that cannot be categorized");
    };

    assert(false);
    return wxEmptyString;
}


wxString FreeFileSync::getSymbol(SyncOperation op)
{
    switch (op)
    {
    case SO_CREATE_NEW_LEFT:
        return wxT("*-");
    case SO_CREATE_NEW_RIGHT:
        return wxT("-*");
    case SO_DELETE_LEFT:
        return wxT("D-");
    case SO_DELETE_RIGHT:
        return wxT("-D");
    case SO_OVERWRITE_LEFT:
        return wxT("<-");
    case SO_OVERWRITE_RIGHT:
        return wxT("->");
    case SO_DO_NOTHING:
        return wxT("-");
    case SO_UNRESOLVED_CONFLICT:
        return wxT("\\/\\->");
    };

    assert(false);
    return wxEmptyString;
}


