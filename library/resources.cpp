#include "resources.h"
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/mstream.h>
#include "../shared/systemConstants.h"
#include <memory>
#include "../shared/standardPaths.h"


const GlobalResources& GlobalResources::getInstance()
{
    static GlobalResources instance;
    return instance;
}


GlobalResources::GlobalResources()
{
    //map, allocate and initialize pictures
    bitmapResource[wxT("left arrow.png")]         = (bitmapArrowLeft         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("right arrow.png")]        = (bitmapArrowRight        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("left arrow create.png")]  = (bitmapArrowLeftCr       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("right arrow create.png")] = (bitmapArrowRightCr      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("no arrow.png")]           = (bitmapArrowNone         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("start sync.png")]         = (bitmapStartSync         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("start sync dis.png")]     = (bitmapStartSyncDis      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("left delete.png")]        = (bitmapDeleteLeft        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("right delete.png")]       = (bitmapDeleteRight       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("email.png")]              = (bitmapEmail             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("about.png")]              = (bitmapAbout             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("about_small.png")]        = (bitmapAboutSmall        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("website.png")]            = (bitmapWebsite           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("exit.png")]               = (bitmapExit              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("compare.png")]            = (bitmapCompare           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("compare disabled.png")]   = (bitmapCompareDisabled   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("sync.png")]               = (bitmapSync              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("sync disabled.png")]      = (bitmapSyncDisabled      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("swap.png")]               = (bitmapSwap              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("swapSmall.png")]          = (bitmapSwapSmall         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("help.png")]               = (bitmapHelp              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftOnly.png")]           = (bitmapLeftOnly          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftOnlyAct.png")]        = (bitmapLeftOnlyAct       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftOnlyDeact.png")]      = (bitmapLeftOnlyDeact     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightOnly.png")]          = (bitmapRightOnly         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightOnlyAct.png")]       = (bitmapRightOnlyAct      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightOnlyDeact.png")]     = (bitmapRightOnlyDeact    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftNewer.png")]          = (bitmapLeftNewer         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftNewerAct.png")]       = (bitmapLeftNewerAct      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftNewerDeact.png")]     = (bitmapLeftNewerDeact    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightNewer.png")]         = (bitmapRightNewer        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightNewerAct.png")]      = (bitmapRightNewerAct     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightNewerDeact.png")]    = (bitmapRightNewerDeact   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("different.png")]          = (bitmapDifferent         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("differentAct.png")]       = (bitmapDifferentAct      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("differentDeact.png")]     = (bitmapDifferentDeact    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("equal.png")]              = (bitmapEqual             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("equalAct.png")]           = (bitmapEqualAct          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("equalDeact.png")]         = (bitmapEqualDeact        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("conflict.png")]           = (bitmapConflict          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("conflictGrey.png")]       = (bitmapConflictGrey      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("conflictAct.png")]        = (bitmapConflictAct       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("conflictDeact.png")]      = (bitmapConflictDeact     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("include.png")]            = (bitmapInclude           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("exclude.png")]            = (bitmapExclude           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("filter active.png")]      = (bitmapFilterOn          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("filter not active.png")]  = (bitmapFilterOff         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("filter_small.png")]       = (bitmapFilterSmall       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("filterSmallGrey.png")]    = (bitmapFilterSmallGrey   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("warning.png")]            = (bitmapWarning           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("warningSmall.png")]       = (bitmapWarningSmall      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("error.png")]              = (bitmapError             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("small arrow up.png"])     = (bitmapSmallUp           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("small arrow down.png")]   = (bitmapSmallDown         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("save.png")]               = (bitmapSave              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("load.png")]               = (bitmapLoad              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("saveSmall.png")]          = (bitmapSaveSmall         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("loadSmall.png")]          = (bitmapLoadSmall         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("newSmall.png")]           = (bitmapNewSmall          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("FFS.png")]                = (bitmapFFS               = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("FFS paused.png")]         = (bitmapFFSPaused         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("deleteFile.png")]         = (bitmapDeleteFile        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("gpl.png")]                = (bitmapGPL               = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusPause.png")]        = (bitmapStatusPause       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusError.png")]        = (bitmapStatusError       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusSuccess.png")]      = (bitmapStatusSuccess     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusWarning.png")]      = (bitmapStatusWarning     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusScanning.png")]     = (bitmapStatusScanning    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusBinaryCompare.png")]= (bitmapStatusBinCompare  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusSyncing.png")]      = (bitmapStatusSyncing     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("logo.png")]               = (bitmapLogo              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("statusEdge.png")]         = (bitmapStatusEdge        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("add pair.png")]           = (bitmapAddFolderPair     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("remove pair.png")]        = (bitmapRemoveFolderPair  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("remove pair disabl.png")] = (bitmapRemoveFolderPairD = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("link.png")]               = (bitmapLink              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("background.png")]         = (bitmapBackground        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("compare_small.png")]      = (bitmapCompareSmall      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("sync_small.png")]         = (bitmapSyncSmall         = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("clock_small.png")]        = (bitmapClockSmall        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("clock.png")]              = (bitmapClock             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("filter.png")]             = (bitmapFilter            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("batch.png")]              = (bitmapBatch             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("batch_small.png")]        = (bitmapBatchSmall        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("move up.png")]            = (bitmapMoveUp            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("move down.png")]          = (bitmapMoveDown          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("checkbox_true.png")]        = (bitmapCheckBoxTrue       = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("checkbox_true_focus.png")]  = (bitmapCheckBoxTrueFocus  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("checkbox_false.png")]       = (bitmapCheckBoxFalse      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("checkbox_false_focus.png")] = (bitmapCheckBoxFalseFocus = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("settings.png")]           = (bitmapSettings          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("settings_small.png")]     = (bitmapSettingsSmall     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("recycler.png")]           = (bitmapRecycler          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("shift.png")]              = (bitmapShift             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncConfig.png")]         = (bitmapSyncCfg           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncConfigSmall.png")]    = (bitmapSyncCfgSmall      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncConfigSmallGrey.png")] = (bitmapSyncCfgSmallGrey  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("cmpConfig.png")]          = (bitmapCmpCfg            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncPreview.png")]        = (bitmapPreview           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncPreviewDisabl.png")]  = (bitmapPreviewDisabled   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("question.png")]           = (bitmapQuestion          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("czechRep.png")]           = (bitmapCzechRep          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("china.png")]              = (bitmapChina             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("holland.png")]            = (bitmapHolland           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("england.png")]            = (bitmapEngland           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("france.png")]             = (bitmapFrance            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("germany.png")]            = (bitmapGermany           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("hungary.png")]            = (bitmapHungary           = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("taiwan.png")]             = (bitmapTaiwan            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("italy.png")]              = (bitmapItaly             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("japan.png")]              = (bitmapJapan             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("poland.png")]             = (bitmapPoland            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("portugal.png")]           = (bitmapPortugal          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("brazil.png")]             = (bitmapBrazil            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("slovakia.png")]           = (bitmapSlovakia          = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("spain.png")]                = (bitmapSpain             = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("russia.png")]               = (bitmapRussia            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncCreateLeftAct.png")]    = (bitmapSyncCreateLeftAct    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncCreateLeftDeact.png")]  = (bitmapSyncCreateLeftDeact  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncCreateRightAct.png")]   = (bitmapSyncCreateRightAct   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncCreateRightDeact.png")] = (bitmapSyncCreateRightDeact = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDeleteLeftAct.png")]    = (bitmapSyncDeleteLeftAct    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDeleteLeftDeact.png")]  = (bitmapSyncDeleteLeftDeact  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDeleteRightAct.png")]   = (bitmapSyncDeleteRightAct   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDeleteRightDeact.png")] = (bitmapSyncDeleteRightDeact = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirLeftAct.png")]     = (bitmapSyncDirLeftAct    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirLeftDeact.png")]   = (bitmapSyncDirLeftDeact  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirRightAct.png")]    = (bitmapSyncDirRightAct   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirRightDeact.png")]  = (bitmapSyncDirRightDeact = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirNoneAct.png")]     = (bitmapSyncDirNoneAct    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirNoneDeact.png")]   = (bitmapSyncDirNoneDeact  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirLeftSmall.png")]   = (bitmapSyncDirLeftSmall  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirRightSmall.png")]  = (bitmapSyncDirRightSmall = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncDirNoneSmall.png")]   = (bitmapSyncDirNoneSmall  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("createLeftSmall.png")]    = (bitmapCreateLeftSmall   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("createRightSmall.png")]   = (bitmapCreateRightSmall  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("deleteLeftSmall.png")]    = (bitmapDeleteLeftSmall   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("deleteRightSmall.png")]   = (bitmapDeleteRightSmall  = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftOnlySmall.png")]      = (bitmapLeftOnlySmall     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightOnlySmall.png")]     = (bitmapRightOnlySmall    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("leftNewerSmall.png")]     = (bitmapLeftNewerSmall    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("rightNewerSmall.png")]    = (bitmapRightNewerSmall   = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("equalSmall.png")]         = (bitmapEqualSmall        = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("differentSmall.png")]     = (bitmapDifferentSmall    = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("conflictSmall.png")]      = (bitmapConflictSmall     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("create.png")]             = (bitmapCreate            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("update.png")]             = (bitmapUpdate            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("delete.png")]             = (bitmapDelete            = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("data.png")]               = (bitmapData              = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("cmpViewSmall.png")]       = (bitmapCmpViewSmall      = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("syncViewSmall.png")]      = (bitmapSyncViewSmall     = new wxBitmap(wxNullBitmap));
    bitmapResource[wxT("toggleViewSmall.png")]    = (bitmapSwitchViewSmall   = new wxBitmap(wxNullBitmap));


    //init all the other resource files
    animationMoney = new wxAnimation(wxNullAnimation);
    animationSync  = new wxAnimation(wxNullAnimation);
    programIcon    = new wxIcon(wxNullIcon);
}


GlobalResources::~GlobalResources()
{
    //free bitmap resources
    for (std::map<wxString, wxBitmap*>::iterator i = bitmapResource.begin(); i != bitmapResource.end(); ++i)
        delete i->second;

    //free other resources
    delete animationMoney;
    delete animationSync;
    delete programIcon;
}


void loadAnimFromZip(wxZipInputStream& zipInput, wxAnimation* animation)
{
    //Workaround for wxWidgets:
    //construct seekable input stream (zip-input stream is non-seekable) for wxAnimation::Load()
    //luckily this method call is very fast: below measurement precision!
    std::vector<unsigned char> data;
    data.reserve(10000);

    int newValue = 0;
    while ((newValue = zipInput.GetC()) != wxEOF)
        data.push_back(newValue);

    wxMemoryInputStream seekAbleStream(&data.front(), data.size()); //stream does not take ownership of data

    animation->Load(seekAbleStream, wxANIMATION_TYPE_GIF);
}


void GlobalResources::load() const
{
    wxFFileInputStream input(FreeFileSync::getInstallationDir() + globalFunctions::FILE_NAME_SEPARATOR + wxT("Resources.dat"));
    if (input.IsOk()) //if not... we don't want to react too harsh here
    {
        //activate support for .png files
        wxImage::AddHandler(new wxPNGHandler); //ownership passed

        wxZipInputStream resourceFile(input);

        std::map<wxString, wxBitmap*>::iterator bmp;
        while (true)
        {
            std::auto_ptr<wxZipEntry> entry(resourceFile.GetNextEntry());
            if (entry.get() == NULL)
                break;

            const wxString name = entry->GetName();

            //search if entry is available in map
            if ((bmp = bitmapResource.find(name)) != bitmapResource.end())
                *(bmp->second) = wxBitmap(wxImage(resourceFile, wxBITMAP_TYPE_PNG));
            else if (name == wxT("money.gif"))
                loadAnimFromZip(resourceFile, animationMoney);
            else if (name == wxT("working.gif"))
                loadAnimFromZip(resourceFile, animationSync);
        }
    }

#ifdef FFS_WIN
    *programIcon = wxIcon(wxT("A_PROGRAM_ICON"));
#else
#include "FreeFileSync.xpm"
    *programIcon = wxIcon(FreeFileSync_xpm);
#endif
}


const wxBitmap& GlobalResources::getImageByName(const wxString& imageName) const
{
    std::map<wxString, wxBitmap*>::const_iterator bmp = bitmapResource.find(imageName);
    if (bmp != bitmapResource.end())
        return *bmp->second;
    else
        return wxNullBitmap;
}
