#include "settingsDialog.h"
#include "../shared/systemConstants.h"
#include "../library/resources.h"
#include <wx/msgdlg.h>
#include "../shared/customButton.h"
#include "../synchronization.h"
//#include "../algorithm.h"
#include "../shared/stringConv.h"
#include "util.h"
#include <wx/dnd.h>
#include "../shared/dragAndDrop.h"
#include "../shared/fileHandling.h"
#include "../shared/xmlBase.h"
#include <wx/wupdlock.h>
#include "folderPair.h"

using namespace FreeFileSync;


SyncCfgDialog::SyncCfgDialog(wxWindow* window,
                             const CompareVariant compareVar,
                             SyncConfiguration& syncConfiguration,
                             DeletionPolicy&    handleDeletion,
                             wxString&          customDeletionDirectory,
                             bool*              ignoreErrors) :
    SyncCfgDlgGenerated(window),
    cmpVariant(compareVar),
    localSyncConfiguration(syncConfiguration),  //make working copy of syncConfiguration
    refSyncConfiguration(syncConfiguration),
    refHandleDeletion(handleDeletion),
    refCustomDeletionDirectory(customDeletionDirectory),
    refIgnoreErrors(ignoreErrors),
    dragDropCustomDelFolder(new DragDropOnDlg(m_panelCustomDeletionDir, m_dirPickerCustomDelFolder, m_textCtrlCustomDelFolder))
{
    setDeletionHandling(handleDeletion);
    m_textCtrlCustomDelFolder->SetValue(customDeletionDirectory);

    //error handling
    if (ignoreErrors)
        setErrorHandling(*ignoreErrors);
    else
    {
        sbSizerErrorHandling->Show(false);
        Layout();
    }

    //set sync config icons
    updateConfigIcons(cmpVariant, localSyncConfiguration);

    //set icons for this dialog
    m_bitmapLeftOnly->SetBitmap(*GlobalResources::getInstance().bitmapLeftOnly);
    m_bitmapRightOnly->SetBitmap(*GlobalResources::getInstance().bitmapRightOnly);
    m_bitmapLeftNewer->SetBitmap(*GlobalResources::getInstance().bitmapLeftNewer);
    m_bitmapRightNewer->SetBitmap(*GlobalResources::getInstance().bitmapRightNewer);
    m_bitmapDifferent->SetBitmap(*GlobalResources::getInstance().bitmapDifferent);
    m_bitmapConflict->SetBitmap(*GlobalResources::getInstance().bitmapConflictGrey);

    bSizer201->Layout(); //wxButtonWithImage size might have changed

    m_buttonApply->SetFocus();

    Fit();
}

//#################################################################################################################

SyncCfgDialog::~SyncCfgDialog() {}


void SyncCfgDialog::updateConfigIcons(const FreeFileSync::CompareVariant cmpVar, const FreeFileSync::SyncConfiguration& syncConfig)
{
    //wxWindowUpdateLocker dummy(this); //avoid display distortion
    wxWindowUpdateLocker dummy2(m_panelCustomDeletionDir); //avoid display distortion
    wxWindowUpdateLocker dummy3(m_bpButtonLeftOnly);
    wxWindowUpdateLocker dummy4(m_bpButtonRightOnly);
    wxWindowUpdateLocker dummy5(m_bpButtonLeftNewer);
    wxWindowUpdateLocker dummy6(m_bpButtonRightNewer);
    wxWindowUpdateLocker dummy7(m_bpButtonDifferent);
    wxWindowUpdateLocker dummy8(m_bpButtonConflict);


    updateConfigIcons(cmpVar,
                      syncConfig,
                      m_bpButtonLeftOnly,
                      m_bpButtonRightOnly,
                      m_bpButtonLeftNewer,
                      m_bpButtonRightNewer,
                      m_bpButtonDifferent,
                      m_bpButtonConflict,
                      m_bitmapLeftOnly,
                      m_bitmapRightOnly,
                      m_bitmapLeftNewer,
                      m_bitmapRightNewer,
                      m_bitmapDifferent,
                      m_bitmapConflict,
                      sbSizerSyncDirections);

    //set radiobuttons -> have no parameter-ownership at all!
    switch (localSyncConfiguration.getVariant())
    {
    case SyncConfiguration::AUTOMATIC:
        m_radioBtnAutomatic->SetValue(true); //automatic mode
        break;
    case SyncConfiguration::MIRROR:
        m_radioBtnMirror->SetValue(true);    //one way ->
        break;
    case SyncConfiguration::UPDATE:
        m_radioBtnUpdate->SetValue(true);    //Update ->
        break;
    case SyncConfiguration::TWOWAY:
        m_radioBtnTwoWay->SetValue(true);    //two way <->
        break;
    case SyncConfiguration::CUSTOM:
        m_radioBtnCustom->SetValue(true);    //custom
        break;
    }

    GetSizer()->SetSizeHints(this); //this works like a charm for GTK2 with window resizing problems!!! (includes call to Fit())
}


void SyncCfgDialog::updateConfigIcons(const CompareVariant compareVar,
                                      const SyncConfiguration& syncConfig,
                                      wxBitmapButton* buttonLeftOnly,
                                      wxBitmapButton* buttonRightOnly,
                                      wxBitmapButton* buttonLeftNewer,
                                      wxBitmapButton* buttonRightNewer,
                                      wxBitmapButton* buttonDifferent,
                                      wxBitmapButton* buttonConflict,
                                      wxStaticBitmap* bitmapLeftOnly,
                                      wxStaticBitmap* bitmapRightOnly,
                                      wxStaticBitmap* bitmapLeftNewer,
                                      wxStaticBitmap* bitmapRightNewer,
                                      wxStaticBitmap* bitmapDifferent,
                                      wxStaticBitmap* bitmapConflict,
                                      wxSizer*        syncDirections) //sizer containing all sync-directions
{
    //display only relevant sync options
    syncDirections->Show(true);

    buttonLeftOnly  ->Show(); //
    buttonRightOnly ->Show(); //
    buttonLeftNewer ->Show(); //
    buttonRightNewer->Show(); // enable everything by default
    buttonDifferent ->Show(); //
    buttonConflict  ->Show(); //

    bitmapLeftOnly  ->Show(); //
    bitmapRightOnly ->Show(); //
    bitmapLeftNewer ->Show(); //
    bitmapRightNewer->Show(); //
    bitmapDifferent ->Show(); //
    bitmapConflict  ->Show(); //

    switch (compareVar)
    {
    case CMP_BY_TIME_SIZE:
        buttonDifferent ->Hide();

        bitmapDifferent ->Hide();
        break;

    case CMP_BY_CONTENT:
        buttonLeftNewer ->Hide();
        buttonRightNewer->Hide();

        bitmapLeftNewer ->Hide();
        bitmapRightNewer->Hide();
        break;
    }

    if (syncConfig.automatic) //automatic mode needs no sync-directions
        syncDirections->Show(false);

    switch (syncConfig.exLeftSideOnly)
    {
    case SYNC_DIR_RIGHT:
        buttonLeftOnly->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowRightCr);
        buttonLeftOnly->SetToolTip(getDescription(SO_CREATE_NEW_RIGHT));
        break;
    case SYNC_DIR_LEFT:
        buttonLeftOnly->SetBitmapLabel(*GlobalResources::getInstance().bitmapDeleteLeft);
        buttonLeftOnly->SetToolTip(getDescription(SO_DELETE_LEFT));
        break;
    case SYNC_DIR_NONE:
        buttonLeftOnly->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowNone);
        buttonLeftOnly->SetToolTip(getDescription(SO_DO_NOTHING));
        break;
    }

    switch (syncConfig.exRightSideOnly)
    {
    case SYNC_DIR_RIGHT:
        buttonRightOnly->SetBitmapLabel(*GlobalResources::getInstance().bitmapDeleteRight);
        buttonRightOnly->SetToolTip(getDescription(SO_DELETE_RIGHT));
        break;
    case SYNC_DIR_LEFT:
        buttonRightOnly->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowLeftCr);
        buttonRightOnly->SetToolTip(getDescription(SO_CREATE_NEW_LEFT));
        break;
    case SYNC_DIR_NONE:
        buttonRightOnly->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowNone);
        buttonRightOnly->SetToolTip(getDescription(SO_DO_NOTHING));
        break;
    }

    switch (syncConfig.leftNewer)
    {
    case SYNC_DIR_RIGHT:
        buttonLeftNewer->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowRight);
        buttonLeftNewer->SetToolTip(getDescription(SO_OVERWRITE_RIGHT));
        break;
    case SYNC_DIR_LEFT:
        buttonLeftNewer->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowLeft);
        buttonLeftNewer->SetToolTip(getDescription(SO_OVERWRITE_LEFT));
        break;
    case SYNC_DIR_NONE:
        buttonLeftNewer->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowNone);
        buttonLeftNewer->SetToolTip(getDescription(SO_DO_NOTHING));
        break;
    }

    switch (syncConfig.rightNewer)
    {
    case SYNC_DIR_RIGHT:
        buttonRightNewer->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowRight);
        buttonRightNewer->SetToolTip(getDescription(SO_OVERWRITE_RIGHT));
        break;
    case SYNC_DIR_LEFT:
        buttonRightNewer->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowLeft);
        buttonRightNewer->SetToolTip(getDescription(SO_OVERWRITE_LEFT));
        break;
    case SYNC_DIR_NONE:
        buttonRightNewer->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowNone);
        buttonRightNewer->SetToolTip(getDescription(SO_DO_NOTHING));
        break;
    }

    switch (syncConfig.different)
    {
    case SYNC_DIR_RIGHT:
        buttonDifferent->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowRight);
        buttonDifferent->SetToolTip(getDescription(SO_OVERWRITE_RIGHT));
        break;
    case SYNC_DIR_LEFT:
        buttonDifferent->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowLeft);
        buttonDifferent->SetToolTip(getDescription(SO_OVERWRITE_LEFT));
        break;
    case SYNC_DIR_NONE:
        buttonDifferent->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowNone);
        buttonDifferent->SetToolTip(getDescription(SO_DO_NOTHING));
        break;
    }

    switch (syncConfig.conflict)
    {
    case SYNC_DIR_RIGHT:
        buttonConflict->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowRight);
        buttonConflict->SetToolTip(getDescription(SO_OVERWRITE_RIGHT));
        break;
    case SYNC_DIR_LEFT:
        buttonConflict->SetBitmapLabel(*GlobalResources::getInstance().bitmapArrowLeft);
        buttonConflict->SetToolTip(getDescription(SO_OVERWRITE_LEFT));
        break;
    case SYNC_DIR_NONE:
        buttonConflict->SetBitmapLabel(*GlobalResources::getInstance().bitmapConflict);
        buttonConflict->SetToolTip(_("Leave as unresolved conflict"));
        break;
    }
}


void SyncCfgDialog::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}


void SyncCfgDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}


void SyncCfgDialog::OnApply(wxCommandEvent& event)
{
    //write configuration to main dialog
    refSyncConfiguration = localSyncConfiguration;
    refHandleDeletion    = getDeletionHandling();
    refCustomDeletionDirectory = m_textCtrlCustomDelFolder->GetValue();
    if (refIgnoreErrors)
        *refIgnoreErrors = getErrorHandling();

    EndModal(BUTTON_APPLY);
}


void SyncCfgDialog::updateToolTipErrorHandling(bool ignoreErrors)
{
    if (ignoreErrors)
        m_choiceHandleError->SetToolTip(_("Hide all error and warning messages"));
    else
        m_choiceHandleError->SetToolTip(_("Show popup on errors or warnings"));
}


bool SyncCfgDialog::getErrorHandling()
{
    if (m_choiceHandleError->GetSelection() == 1) //Ignore errors
        return true;
    else
        return false; // Show popup
}


void SyncCfgDialog::setErrorHandling(bool ignoreErrors)
{
    m_choiceHandleError->Clear();
    m_choiceHandleError->Append(_("Show popup"));
    m_choiceHandleError->Append(_("Ignore errors"));

    if (ignoreErrors)
        m_choiceHandleError->SetSelection(1);
    else
        m_choiceHandleError->SetSelection(0);

    updateToolTipErrorHandling(ignoreErrors);
}


void SyncCfgDialog::OnChangeErrorHandling(wxCommandEvent& event)
{
    updateToolTipErrorHandling(getErrorHandling());
}

//-------------------

void updateToolTipDeletionHandling(wxChoice* choiceHandleError, wxPanel* customDir, const FreeFileSync::DeletionPolicy value)
{
    customDir->Disable();

    switch (value)
    {
    case FreeFileSync::DELETE_PERMANENTLY:
        choiceHandleError->SetToolTip(_("Delete or overwrite files permanently."));
        break;

    case FreeFileSync::MOVE_TO_RECYCLE_BIN:
        choiceHandleError->SetToolTip(_("Use Recycle Bin when deleting or overwriting files."));
        break;

    case FreeFileSync::MOVE_TO_CUSTOM_DIRECTORY:
        choiceHandleError->SetToolTip(_("Move files to a user-defined directory."));
        customDir->Enable();
        break;
    }
}


FreeFileSync::DeletionPolicy SyncCfgDialog::getDeletionHandling()
{
    switch (m_choiceHandleDeletion->GetSelection())
    {
    case 0:
        return FreeFileSync::DELETE_PERMANENTLY;
    case 1:
        return FreeFileSync::MOVE_TO_RECYCLE_BIN;
    case 2:
        return FreeFileSync::MOVE_TO_CUSTOM_DIRECTORY;
    default:
        assert(false);
        return FreeFileSync::MOVE_TO_RECYCLE_BIN;
    }
}


void SyncCfgDialog::setDeletionHandling(FreeFileSync::DeletionPolicy newValue)
{
    m_choiceHandleDeletion->Clear();
    m_choiceHandleDeletion->Append(_("Delete permanently"));
    m_choiceHandleDeletion->Append(_("Use Recycle Bin"));
    m_choiceHandleDeletion->Append(_("User-defined directory"));

    switch (newValue)
    {
    case FreeFileSync::DELETE_PERMANENTLY:
        m_choiceHandleDeletion->SetSelection(0);
        break;
    case FreeFileSync::MOVE_TO_RECYCLE_BIN:
        m_choiceHandleDeletion->SetSelection(1);
        break;
    case FreeFileSync::MOVE_TO_CUSTOM_DIRECTORY:
        m_choiceHandleDeletion->SetSelection(2);
        break;
    }

    updateToolTipDeletionHandling(m_choiceHandleDeletion, m_panelCustomDeletionDir, newValue);
}


void SyncCfgDialog::OnChangeDeletionHandling(wxCommandEvent& event)
{
    updateToolTipDeletionHandling(m_choiceHandleDeletion, m_panelCustomDeletionDir, getDeletionHandling());
}


void SyncCfgDialog::OnSyncAutomatic(wxCommandEvent& event)
{
    localSyncConfiguration.setVariant(SyncConfiguration::AUTOMATIC);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnSyncLeftToRight(wxCommandEvent& event)
{
    localSyncConfiguration.setVariant(SyncConfiguration::MIRROR);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnSyncUpdate(wxCommandEvent& event)
{
    localSyncConfiguration.setVariant(SyncConfiguration::UPDATE);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnSyncBothSides(wxCommandEvent& event)
{
    localSyncConfiguration.setVariant(SyncConfiguration::TWOWAY);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void toggleSyncDirection(SyncDirection& current)
{
    switch (current)
    {
    case SYNC_DIR_RIGHT:
        current = SYNC_DIR_LEFT;
        break;
    case SYNC_DIR_LEFT:
        current = SYNC_DIR_NONE;
        break;
    case SYNC_DIR_NONE:
        current = SYNC_DIR_RIGHT;
        break;
    }
}


void SyncCfgDialog::OnExLeftSideOnly( wxCommandEvent& event )
{
    toggleSyncDirection(localSyncConfiguration.exLeftSideOnly);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnExRightSideOnly( wxCommandEvent& event )
{
    toggleSyncDirection(localSyncConfiguration.exRightSideOnly);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnLeftNewer( wxCommandEvent& event )
{
    toggleSyncDirection(localSyncConfiguration.leftNewer);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnRightNewer( wxCommandEvent& event )
{
    toggleSyncDirection(localSyncConfiguration.rightNewer);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnDifferent( wxCommandEvent& event )
{
    toggleSyncDirection(localSyncConfiguration.different);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


void SyncCfgDialog::OnConflict(wxCommandEvent& event)
{
    toggleSyncDirection(localSyncConfiguration.conflict);
    updateConfigIcons(cmpVariant, localSyncConfiguration);
}


//###################################################################################################################################

class BatchFileDropEvent : public wxFileDropTarget
{
public:
    BatchFileDropEvent(BatchDialog& dlg) :
        batchDlg(dlg) {}

    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
    {
        if (!filenames.IsEmpty())
        {
            const wxString droppedFileName = filenames[0];

            xmlAccess::XmlType fileType = xmlAccess::getXmlType(droppedFileName);

            //test if ffs batch file has been dropped
            if (fileType == xmlAccess::XML_BATCH_CONFIG)
                batchDlg.loadBatchFile(droppedFileName);
            else
            {
                wxString errorMessage = _("%x is not a valid FreeFileSync batch file!");
                errorMessage.Replace(wxT("%x"), wxString(wxT("\"")) + droppedFileName + wxT("\""), false);
                wxMessageBox(errorMessage, _("Error"), wxOK | wxICON_ERROR);
            }
        }
        return false;
    }

private:
    BatchDialog& batchDlg;
};

//###################################################################################################################################

//------------------------------------------------------------------
/*    class hierarchy:

            template<>
            FolderPairPanelBasic
                    /|\
                     |
             template<>
             FolderPairCallback      BatchFolderPairGenerated
                    /|\                        /|\
            _________|______________    ________|
           |                        |  |
  FirstBatchFolderPairCfg    BatchFolderPairPanel
*/

template <class GuiPanel>
class FolderPairCallback : public FolderPairPanelBasic<GuiPanel> //implements callback functionality to BatchDialog as imposed by FolderPairPanelBasic
{
public:
    FolderPairCallback(GuiPanel& basicPanel, BatchDialog& batchDialog) :
        FolderPairPanelBasic<GuiPanel>(basicPanel), //pass FolderPairGenerated part...
        batchDlg(batchDialog) {}

private:
    virtual wxWindow* getParentWindow()
    {
        return &batchDlg;
    }

    virtual MainConfiguration getMainConfig() const
    {
        return batchDlg.getCurrentConfiguration().mainCfg;
    }

    BatchDialog& batchDlg;
};


class BatchFolderPairPanel :
    public BatchFolderPairGenerated, //BatchFolderPairPanel "owns" BatchFolderPairGenerated!
    public FolderPairCallback<BatchFolderPairGenerated>
{
public:
    BatchFolderPairPanel(wxWindow* parent, BatchDialog& batchDialog) :
        BatchFolderPairGenerated(parent),
        FolderPairCallback<BatchFolderPairGenerated>(static_cast<BatchFolderPairGenerated&>(*this), batchDialog), //pass BatchFolderPairGenerated part...
        dragDropOnLeft( m_panelLeft,  m_dirPickerLeft,  m_directoryLeft),
        dragDropOnRight(m_panelRight, m_dirPickerRight, m_directoryRight) {}

private:
    //support for drag and drop
    DragDropOnDlg dragDropOnLeft;
    DragDropOnDlg dragDropOnRight;
};


class FirstBatchFolderPairCfg : public FolderPairCallback<BatchDlgGenerated>
{
public:
    FirstBatchFolderPairCfg(BatchDialog& batchDialog) :
        FolderPairCallback<BatchDlgGenerated>(batchDialog, batchDialog),

        //prepare drag & drop
        dragDropOnLeft(batchDialog.m_panelLeft,
                       batchDialog.m_dirPickerLeft,
                       batchDialog.m_directoryLeft),
        dragDropOnRight(batchDialog.m_panelRight,
                        batchDialog.m_dirPickerRight,
                        batchDialog.m_directoryRight) {}

private:
    //support for drag and drop
    DragDropOnDlg dragDropOnLeft;
    DragDropOnDlg dragDropOnRight;
};


//###################################################################################################################################
BatchDialog::BatchDialog(wxWindow* window, const xmlAccess::XmlBatchConfig& batchCfg) :
    BatchDlgGenerated(window)
{
    init();
    loadBatchCfg(batchCfg);
}


BatchDialog::BatchDialog(wxWindow* window, const wxString& filename) :
    BatchDlgGenerated(window)
{
    init();
    loadBatchFile(filename);
}


void BatchDialog::init()
{
    wxWindowUpdateLocker dummy(this); //avoid display distortion

    //init handling of first folder pair
    firstFolderPair.reset(new FirstBatchFolderPairCfg(*this));


    //prepare drag & drop for loading of *.ffs_batch files
    SetDropTarget(new BatchFileDropEvent(*this));
    dragDropOnLogfileDir.reset(new DragDropOnDlg(m_panelLogging, m_dirPickerLogfileDir, m_textCtrlLogfileDir));

    //support for drag and drop: user-defined deletion directory
    dragDropCustomDelFolder.reset(new DragDropOnDlg(m_panelCustomDeletionDir, m_dirPickerCustomDelFolder, m_textCtrlCustomDelFolder));


    //set icons for this dialog
    m_bpButtonAddPair->SetBitmapLabel(*GlobalResources::getInstance().bitmapAddFolderPair);
    m_bitmapLeftOnly->SetBitmap(*GlobalResources::getInstance().bitmapLeftOnly);
    m_bitmapRightOnly->SetBitmap(*GlobalResources::getInstance().bitmapRightOnly);
    m_bitmapLeftNewer->SetBitmap(*GlobalResources::getInstance().bitmapLeftNewer);
    m_bitmapRightNewer->SetBitmap(*GlobalResources::getInstance().bitmapRightNewer);
    m_bitmapDifferent->SetBitmap(*GlobalResources::getInstance().bitmapDifferent);
    m_bitmapConflict->SetBitmap(*GlobalResources::getInstance().bitmapConflictGrey);
    m_bitmap8->SetBitmap(*GlobalResources::getInstance().bitmapInclude);
    m_bitmap9->SetBitmap(*GlobalResources::getInstance().bitmapExclude);
    m_bitmap27->SetBitmap(*GlobalResources::getInstance().bitmapBatch);

    m_buttonSave->SetFocus();
}

//------------------- error handling --------------------------

xmlAccess::OnError BatchDialog::getSelectionHandleError() const
{
    switch (m_choiceHandleError->GetSelection())
    {
    case 0:
        return xmlAccess::ON_ERROR_POPUP;
    case 1:
        return xmlAccess::ON_ERROR_IGNORE;
    case 2:
        return xmlAccess::ON_ERROR_EXIT;
    default:
        assert(false);
        return xmlAccess::ON_ERROR_POPUP;
    }
}


void BatchDialog::updateToolTipErrorHandling(const xmlAccess::OnError value)
{
    switch (value)
    {
    case xmlAccess::ON_ERROR_POPUP:
        m_choiceHandleError->SetToolTip(_("Show popup on errors or warnings"));
        break;
    case xmlAccess::ON_ERROR_IGNORE:
        m_choiceHandleError->SetToolTip(_("Hide all error and warning messages"));
        break;
    case xmlAccess::ON_ERROR_EXIT:
        m_choiceHandleError->SetToolTip(_("Exit immediately and set returncode < 0"));
        break;
    }
}


void BatchDialog::setSelectionHandleError(const xmlAccess::OnError value)
{
    m_choiceHandleError->Clear();
    m_choiceHandleError->Append(_("Show popup"));
    m_choiceHandleError->Append(_("Ignore errors"));
    if (m_checkBoxSilent->GetValue()) //this option shall be available for silent mode only!
        m_choiceHandleError->Append(_("Exit with RC < 0"));

    //default
    m_choiceHandleError->SetSelection(0);

    switch (value)
    {
    case xmlAccess::ON_ERROR_POPUP:
        m_choiceHandleError->SetSelection(0);
        break;
    case xmlAccess::ON_ERROR_IGNORE:
        m_choiceHandleError->SetSelection(1);
        break;
    case xmlAccess::ON_ERROR_EXIT:
        if (m_checkBoxSilent->GetValue()) //this option shall be available for silent mode only!
            m_choiceHandleError->SetSelection(2);
        break;
    }

    updateToolTipErrorHandling(getSelectionHandleError());
}


void BatchDialog::OnChangeErrorHandling(wxCommandEvent& event)
{
    updateToolTipErrorHandling(getSelectionHandleError());
}


void BatchDialog::OnExLeftSideOnly(wxCommandEvent& event)
{
    toggleSyncDirection(localSyncConfiguration.exLeftSideOnly);
    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);
}


//------------------- deletion handling --------------------------

FreeFileSync::DeletionPolicy BatchDialog::getDeletionHandling() const
{
    switch (m_choiceHandleDeletion->GetSelection())
    {
    case 0:
        return FreeFileSync::DELETE_PERMANENTLY;
    case 1:
        return FreeFileSync::MOVE_TO_RECYCLE_BIN;
    case 2:
        return FreeFileSync::MOVE_TO_CUSTOM_DIRECTORY;
    default:
        assert(false);
        return FreeFileSync::MOVE_TO_RECYCLE_BIN;
    }
}


void BatchDialog::setDeletionHandling(FreeFileSync::DeletionPolicy newValue)
{
    m_choiceHandleDeletion->Clear();
    m_choiceHandleDeletion->Append(_("Delete permanently"));
    m_choiceHandleDeletion->Append(_("Use Recycle Bin"));
    m_choiceHandleDeletion->Append(_("User-defined directory"));

    switch (newValue)
    {
    case FreeFileSync::DELETE_PERMANENTLY:
        m_choiceHandleDeletion->SetSelection(0);
        break;
    case FreeFileSync::MOVE_TO_RECYCLE_BIN:
        m_choiceHandleDeletion->SetSelection(1);
        break;
    case FreeFileSync::MOVE_TO_CUSTOM_DIRECTORY:
        m_choiceHandleDeletion->SetSelection(2);
        break;
    }

    updateToolTipDeletionHandling(m_choiceHandleDeletion, m_panelCustomDeletionDir, newValue);
}


void BatchDialog::OnChangeDeletionHandling(wxCommandEvent& event)
{
    updateToolTipDeletionHandling(m_choiceHandleDeletion, m_panelCustomDeletionDir, getDeletionHandling());
}



void BatchDialog::OnExRightSideOnly(wxCommandEvent& event)
{
    toggleSyncDirection(localSyncConfiguration.exRightSideOnly);
    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);
}


void BatchDialog::OnLeftNewer(wxCommandEvent& event)
{
    toggleSyncDirection(localSyncConfiguration.leftNewer);
    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);
}


void BatchDialog::OnRightNewer(wxCommandEvent& event)
{
    toggleSyncDirection(localSyncConfiguration.rightNewer);
    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);
}


void BatchDialog::OnDifferent(wxCommandEvent& event)
{
    toggleSyncDirection(localSyncConfiguration.different);
    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);
}


void BatchDialog::OnConflict(wxCommandEvent& event)
{
    toggleSyncDirection(localSyncConfiguration.conflict);
    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);
}


void BatchDialog::OnCheckFilter(wxCommandEvent& event)
{
    updateVisibleTabs();

    //update main local filter
    firstFolderPair->refreshButtons();

    //update folder pairs
    for (std::vector<BatchFolderPairPanel*>::const_iterator i = additionalFolderPairs.begin(); i != additionalFolderPairs.end(); ++i)
    {
        BatchFolderPairPanel* dirPair = *i;
        dirPair->refreshButtons();
    }
}


void BatchDialog::OnCheckAutomatic(wxCommandEvent& event)
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    //toggle automatic setting
    localSyncConfiguration.automatic = !localSyncConfiguration.automatic;

    if (localSyncConfiguration.automatic)
        localSyncConfiguration.setVariant(SyncConfiguration::AUTOMATIC); //reset conflict-setting

    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);
    Fit();
}


void BatchDialog::OnCheckSilent(wxCommandEvent& event)
{
    updateVisibleTabs();

    //reset error handling depending on "m_checkBoxSilent"
    setSelectionHandleError(getSelectionHandleError());
}


void BatchDialog::updateVisibleTabs()
{
    showNotebookpage(m_panelFilter,  _("Filter"),  m_checkBoxFilter->GetValue());
    showNotebookpage(m_panelLogging, _("Logging"), m_checkBoxSilent->GetValue());
}


void BatchDialog::showNotebookpage(wxWindow* page, const wxString& pageName, bool show)
{
    int windowPosition = -1;
    for (size_t i = 0; i < m_notebookSettings->GetPageCount(); ++i)
        if (    static_cast<wxWindow*>(m_notebookSettings->GetPage(i)) ==
                static_cast<wxWindow*>(page))
        {
            windowPosition = i;
            break;
        }

    if (show)
    {
        if (windowPosition == -1)
            m_notebookSettings->AddPage(page, pageName, false);
    }
    else
    {
        if (windowPosition != -1)
        {
            //do not delete currently selected tab!!
            if (m_notebookSettings->GetCurrentPage() == m_notebookSettings->GetPage(windowPosition))
                m_notebookSettings->ChangeSelection(0);

            m_notebookSettings->RemovePage(windowPosition);
        }
    }
}


CompareVariant BatchDialog::getCurrentCompareVar() const
{
    if (m_radioBtnSizeDate->GetValue())
        return CMP_BY_TIME_SIZE;
    else if (m_radioBtnContent->GetValue())
        return CMP_BY_CONTENT;
    else
    {
        assert(false);
        return CMP_BY_TIME_SIZE;
    }
}


void BatchDialog::updateConfigIcons(const FreeFileSync::CompareVariant cmpVar, const FreeFileSync::SyncConfiguration& syncConfig)
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    SyncCfgDialog::updateConfigIcons(cmpVar,
                                     syncConfig,
                                     m_bpButtonLeftOnly,
                                     m_bpButtonRightOnly,
                                     m_bpButtonLeftNewer,
                                     m_bpButtonRightNewer,
                                     m_bpButtonDifferent,
                                     m_bpButtonConflict,
                                     m_bitmapLeftOnly,
                                     m_bitmapRightOnly,
                                     m_bitmapLeftNewer,
                                     m_bitmapRightNewer,
                                     m_bitmapDifferent,
                                     m_bitmapConflict,
                                     sbSizerSyncDirections);

    //parameter ownership lies within localSyncConfiguration, NOT m_checkBoxAutomatic!!!
    m_checkBoxAutomatic->SetValue(localSyncConfiguration.automatic);

    m_panelOverview->Layout(); //needed
}


void BatchDialog::OnChangeCompareVar(wxCommandEvent& event)
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion
    updateConfigIcons(getCurrentCompareVar(), localSyncConfiguration);

    Fit();
}


void BatchDialog::OnClose(wxCloseEvent&   event)
{
    EndModal(0);
}


void BatchDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}


void BatchDialog::OnSaveBatchJob(wxCommandEvent& event)
{
    //get a filename
    const wxString defaultFileName = proposedBatchFileName.empty() ? wxT("SyncJob.ffs_batch") : proposedBatchFileName;

    wxFileDialog* filePicker = new wxFileDialog(this, wxEmptyString, wxEmptyString, defaultFileName, wxString(_("FreeFileSync batch file")) + wxT(" (*.ffs_batch)|*.ffs_batch"), wxFD_SAVE);
    if (filePicker->ShowModal() == wxID_OK)
    {
        const wxString newFileName = filePicker->GetPath();
        if (FreeFileSync::fileExists(wxToZ(newFileName)))
        {
            QuestionDlg* messageDlg = new QuestionDlg(this,
                    QuestionDlg::BUTTON_YES | QuestionDlg::BUTTON_CANCEL,
                    wxString(_("File already exists. Overwrite?")) + wxT(" \"") + newFileName + wxT("\""));

            if (messageDlg->ShowModal() != QuestionDlg::BUTTON_YES)
            {
                OnSaveBatchJob(event); //retry
                return;
            }
        }

        //create batch file
        if (saveBatchFile(newFileName))
            EndModal(BATCH_FILE_SAVED);
    }
}


void BatchDialog::OnLoadBatchJob(wxCommandEvent& event)
{
    wxFileDialog* filePicker = new wxFileDialog(this, wxEmptyString, wxEmptyString, wxEmptyString, wxString(_("FreeFileSync batch file")) + wxT(" (*.ffs_batch)|*.ffs_batch"), wxFD_OPEN);;
    if (filePicker->ShowModal() == wxID_OK)
        loadBatchFile(filePicker->GetPath());
}



inline
FolderPairEnh getEnahncedPair(const BatchFolderPairPanel* panel)
{
    return FolderPairEnh(panel->getLeftDir(),
                         panel->getRightDir(),
                         panel->getAltSyncConfig(),
                         panel->getAltFilterConfig());
}


xmlAccess::XmlBatchConfig BatchDialog::getCurrentConfiguration() const
{
    xmlAccess::XmlBatchConfig batchCfg;

    //load structure with basic settings "mainCfg"
    batchCfg.mainCfg.compareVar        = getCurrentCompareVar();
    batchCfg.mainCfg.syncConfiguration = localSyncConfiguration;
    batchCfg.mainCfg.filterIsActive    = m_checkBoxFilter->GetValue();
    batchCfg.mainCfg.includeFilter     = wxToZ(m_textCtrlInclude->GetValue());
    batchCfg.mainCfg.excludeFilter     = wxToZ(m_textCtrlExclude->GetValue());
    batchCfg.mainCfg.handleDeletion    = getDeletionHandling();
    batchCfg.mainCfg.customDeletionDirectory = m_textCtrlCustomDelFolder->GetValue();

    //first folder pair
    batchCfg.mainCfg.firstPair = FolderPairEnh(firstFolderPair->getLeftDir(),
                                 firstFolderPair->getRightDir(),
                                 firstFolderPair->getAltSyncConfig(),
                                 firstFolderPair->getAltFilterConfig());

    //add additional pairs
    batchCfg.mainCfg.additionalPairs.clear();
    std::transform(additionalFolderPairs.begin(), additionalFolderPairs.end(),
                   std::back_inserter(batchCfg.mainCfg.additionalPairs), getEnahncedPair);


    //load structure with batch settings "batchCfg"
    batchCfg.silent           = m_checkBoxSilent->GetValue();
    batchCfg.handleError      = getSelectionHandleError();
    batchCfg.logFileDirectory = m_textCtrlLogfileDir->GetValue();

    return batchCfg;
}


bool BatchDialog::saveBatchFile(const wxString& filename)
{
    const xmlAccess::XmlBatchConfig batchCfg = getCurrentConfiguration();

    //write config to XML
    try
    {
        xmlAccess::writeBatchConfig(batchCfg, filename);
    }
    catch (const xmlAccess::XmlError& error)
    {
        wxMessageBox(error.show().c_str(), _("Error"), wxOK | wxICON_ERROR);
        return false;
    }

    SetTitle(wxString(_("Create a batch job")) + wxT(" - ") + filename);
    proposedBatchFileName = filename; //may be used on next save

    return true;
}


void BatchDialog::loadBatchFile(const wxString& filename)
{
    //load XML settings
    xmlAccess::XmlBatchConfig batchCfg;  //structure to receive gui settings
    try
    {
        xmlAccess::readBatchConfig(filename, batchCfg);
    }
    catch (const xmlAccess::XmlError& error)
    {
        if (error.getSeverity() == xmlAccess::XmlError::WARNING)
            wxMessageBox(error.show(), _("Warning"), wxOK | wxICON_WARNING);
        else
        {
            wxMessageBox(error.show(), _("Error"), wxOK | wxICON_ERROR);
            return;
        }
    }

    SetTitle(wxString(_("Create a batch job")) + wxT(" - ") + filename);
    proposedBatchFileName = filename; //may be used on next save
    this->loadBatchCfg(batchCfg);
}


void BatchDialog::loadBatchCfg(const xmlAccess::XmlBatchConfig& batchCfg)
{
    wxWindowUpdateLocker dummy(this); //avoid display distortion

    //make working copy of mainDialog.cfg.syncConfiguration and recycler setting
    localSyncConfiguration = batchCfg.mainCfg.syncConfiguration;

    setDeletionHandling(batchCfg.mainCfg.handleDeletion);
    m_textCtrlCustomDelFolder->SetValue(batchCfg.mainCfg.customDeletionDirectory);

    switch (batchCfg.mainCfg.compareVar)
    {
    case CMP_BY_TIME_SIZE:
        m_radioBtnSizeDate->SetValue(true);
        break;
    case CMP_BY_CONTENT:
        m_radioBtnContent->SetValue(true);
        break;
    }

    updateConfigIcons(batchCfg.mainCfg.compareVar, batchCfg.mainCfg.syncConfiguration);

    m_checkBoxFilter->SetValue(batchCfg.mainCfg.filterIsActive);
    m_textCtrlInclude->SetValue(zToWx(batchCfg.mainCfg.includeFilter));
    m_textCtrlExclude->SetValue(zToWx(batchCfg.mainCfg.excludeFilter));

    m_checkBoxSilent->SetValue(batchCfg.silent);
    m_textCtrlLogfileDir->SetValue(batchCfg.logFileDirectory);
    //error handling is dependent from m_checkBoxSilent! /|\   \|/
    setSelectionHandleError(batchCfg.handleError);

    //set first folder pair
    firstFolderPair->setValues(batchCfg.mainCfg.firstPair.leftDirectory,
                               batchCfg.mainCfg.firstPair.rightDirectory,
                               batchCfg.mainCfg.firstPair.altSyncConfig,
                               batchCfg.mainCfg.firstPair.localFilter);

    //remove existing additional folder pairs
    clearAddFolderPairs();

    //set additional pairs
    addFolderPair(batchCfg.mainCfg.additionalPairs);

    updateVisibleTabs();

    Fit(); //needed
    Refresh(); //needed
    Centre();
}


void BatchDialog::OnAddFolderPair(wxCommandEvent& event)
{
    wxWindowUpdateLocker dummy(this); //avoid display distortion

    std::vector<FolderPairEnh> newPairs;
    newPairs.push_back(getCurrentConfiguration().mainCfg.firstPair);

    addFolderPair(newPairs, true); //add pair in front of additonal pairs

    //clear first pair
                const FolderPairEnh cfgEmpty;
        firstFolderPair->setValues(cfgEmpty.leftDirectory,
                                   cfgEmpty.rightDirectory,
                                   cfgEmpty.altSyncConfig,
                                   cfgEmpty.localFilter);
}


void BatchDialog::OnRemoveFolderPair(wxCommandEvent& event)
{
    //find folder pair originating the event
    const wxObject* const eventObj = event.GetEventObject();
    for (std::vector<BatchFolderPairPanel*>::const_iterator i = additionalFolderPairs.begin(); i != additionalFolderPairs.end(); ++i)
    {
        if (eventObj == static_cast<wxObject*>((*i)->m_bpButtonRemovePair))
        {
            removeAddFolderPair(i - additionalFolderPairs.begin());
            return;
        }
    }
}


void BatchDialog::OnRemoveTopFolderPair(wxCommandEvent& event)
{
    if (additionalFolderPairs.size() > 0)
    {
        //get settings from second folder pair
        const FolderPairEnh cfgSecond = getEnahncedPair(additionalFolderPairs[0]);

        //reset first pair
        firstFolderPair->setValues(cfgSecond.leftDirectory,
                                   cfgSecond.rightDirectory,
                                   cfgSecond.altSyncConfig,
                                   cfgSecond.localFilter);

        removeAddFolderPair(0); //remove second folder pair (first of additional folder pairs)
    }
}


const size_t MAX_FOLDER_PAIRS = 3;


void BatchDialog::updateGuiForFolderPair()
{
    //adapt delete top folder pair button
    if (additionalFolderPairs.size() == 0)
        m_bpButtonRemovePair->Hide();
    else
        m_bpButtonRemovePair->Show();

    //adapt local filter and sync cfg for first folder pair
    if (    additionalFolderPairs.size() == 0 &&
            firstFolderPair->getAltSyncConfig().get() == NULL &&
            NameFilter(firstFolderPair->getAltFilterConfig().includeFilter,
                       firstFolderPair->getAltFilterConfig().excludeFilter).isNull())
    {
        m_bpButtonLocalFilter->Hide();
        m_bpButtonAltSyncCfg->Hide();
    }
    else
    {
        m_bpButtonLocalFilter->Show();
        m_bpButtonAltSyncCfg->Show();
    }

    m_scrolledWindow6->Fit();  //adjust scrolled window size
    m_panelOverview->Layout(); //adjust stuff inside scrolled window
}


void BatchDialog::addFolderPair(const std::vector<FreeFileSync::FolderPairEnh>& newPairs, bool addFront)
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    if (!newPairs.empty())
{
    //add folder pairs
    int pairHeight = 0;
    for (std::vector<FreeFileSync::FolderPairEnh>::const_iterator i = newPairs.begin(); i != newPairs.end(); ++i)
    {
        BatchFolderPairPanel* newPair = new BatchFolderPairPanel(m_scrolledWindow6, *this);

        if (addFront)
        {
            bSizerAddFolderPairs->Insert(0, newPair, 0, wxEXPAND, 5);
            additionalFolderPairs.insert(additionalFolderPairs.begin(), newPair);
        }
        else
        {
            bSizerAddFolderPairs->Add(newPair, 0, wxEXPAND, 5);
            additionalFolderPairs.push_back(newPair);
        }

        //get size of scrolled window
        pairHeight = newPair->GetSize().GetHeight();

        //register events
        newPair->m_bpButtonRemovePair->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BatchDialog::OnRemoveFolderPair), NULL, this );

        //set alternate configuration
        newPair->setValues(i->leftDirectory,
                           i->rightDirectory,
                           i->altSyncConfig,
                           i->localFilter);
    }
    //set size of scrolled window
    const int visiblePairs = std::min(additionalFolderPairs.size() + 1, MAX_FOLDER_PAIRS); //up to MAX_FOLDER_PAIRS pairs shall be shown
    m_scrolledWindow6->SetMinSize(wxSize( -1, pairHeight * visiblePairs));

    //update controls
    m_scrolledWindow6->Fit();  //adjust scrolled window size
    m_panelOverview->Layout(); //adjust stuff inside scrolled window
    Fit();                     //adapt dialog size

    //after changing folder pairs window focus is lost: results in scrolled window scrolling to top each time window is shown: we don't want this
    m_bpButtonLeftOnly->SetFocus();
}

    updateGuiForFolderPair();
}


void BatchDialog::removeAddFolderPair(const int pos)
{
        wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    if (0 <= pos && pos < static_cast<int>(additionalFolderPairs.size()))
    {
        //remove folder pairs from window
        BatchFolderPairPanel* pairToDelete = additionalFolderPairs[pos];
        const int pairHeight = pairToDelete->GetSize().GetHeight();

        bSizerAddFolderPairs->Detach(pairToDelete); //Remove() does not work on Window*, so do it manually
        pairToDelete->Destroy();                 //
        additionalFolderPairs.erase(additionalFolderPairs.begin() + pos); //remove last element in vector

        //set size of scrolled window
        const int visiblePairs = std::min(additionalFolderPairs.size() + 1, MAX_FOLDER_PAIRS); //up to MAX_FOLDER_PAIRS pairs shall be shown
        m_scrolledWindow6->SetMinSize(wxSize(-1, pairHeight * visiblePairs));

        //update controls
        m_scrolledWindow6->Fit();  //adjust scrolled window size
        m_panelOverview->Layout(); //adjust stuff inside scrolled window

        m_panelOverview->InvalidateBestSize(); //needed for Fit() to work correctly!
        Fit();                     //adapt dialog size

        //after changing folder pairs window focus is lost: results in scrolled window scrolling to top each time window is shown: we don't want this
        m_bpButtonLeftOnly->SetFocus();
    }

        updateGuiForFolderPair();
}


void BatchDialog::clearAddFolderPairs()
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    additionalFolderPairs.clear();
    bSizerAddFolderPairs->Clear(true);

    m_scrolledWindow6->SetMinSize(wxSize(-1, sbSizerMainPair->GetSize().GetHeight())); //respect height of main pair
}


/*
#ifdef FFS_WIN
#include <wx/msw/wrapwin.h> //includes "windows.h"
#include <shlobj.h>
#endif  // FFS_WIN

template <typename T>
struct CleanUp
{
    CleanUp(T* element) : m_element(element) {}

    ~CleanUp()
    {
        m_element->Release();
    }

    T* m_element;
};


bool BatchDialog::createBatchFile(const wxString& filename)
{
    //create shell link (instead of batch file) for full Unicode support
    HRESULT hResult = E_FAIL;
    IShellLink* pShellLink = NULL;

    if (FAILED(CoCreateInstance(CLSID_ShellLink,       //class identifier
                                NULL,                  //object isn't part of an aggregate
                                CLSCTX_INPROC_SERVER,  //context for running executable code
                                IID_IShellLink,        //interface identifier
                                (void**)&pShellLink))) //pointer to storage of interface pointer
        return false;
    CleanUp<IShellLink> cleanOnExit(pShellLink);

    wxString freeFileSyncExe = wxStandardPaths::Get().GetExecutablePath();
    if (FAILED(pShellLink->SetPath(freeFileSyncExe.c_str())))
        return false;

    if (FAILED(pShellLink->SetArguments(getCommandlineArguments().c_str())))
        return false;

    if (FAILED(pShellLink->SetIconLocation(freeFileSyncExe.c_str(), 1))) //second icon from executable file is used
        return false;

    if (FAILED(pShellLink->SetDescription(_("FreeFileSync Batch Job"))))
        return false;

    IPersistFile*  pPersistFile = NULL;
    if (FAILED(pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)))
        return false;
    CleanUp<IPersistFile> cleanOnExit2(pPersistFile);

    //pPersistFile->Save accepts unicode input only
#ifdef _UNICODE
    hResult = pPersistFile->Save(filename.c_str(), TRUE);
#else
    WCHAR wszTemp [MAX_PATH];
    if (MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, wszTemp, MAX_PATH) == 0)
        return false;

    hResult = pPersistFile->Save(wszTemp, TRUE);
#endif
    if (FAILED(hResult))
        return false;

    return true;
}
*/
