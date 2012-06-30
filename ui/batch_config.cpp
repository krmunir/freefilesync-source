// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) ZenJu (zhnmju123 AT gmx DOT de) - All Rights Reserved    *
// **************************************************************************

#include "batch_config.h"
#include <wx+/dir_picker.h>
#include "folder_pair.h"
#include <iterator>
#include <wx/wupdlock.h>
#include <wx+/context_menu.h>
#include "../lib/help_provider.h"
#include <zen/file_handling.h>
#include "msg_popup.h"
#include "gui_generated.h"
#include <wx/msgdlg.h>
#include <wx+/button.h>
#include <wx+/choice_enum.h>
#include <wx+/mouse_move_dlg.h>

using namespace zen;

class DirectoryPairBatchFirst;
class DirectoryPairBatch;


class BatchDialog: public BatchDlgGenerated
{
    //    friend class BatchFileDropEvent;
    template <class GuiPanel>
    friend class FolderPairCallback;

public:
    BatchDialog(wxWindow* parent,
                const wxString& referenceFile,
                const xmlAccess::XmlBatchConfig& batchCfg,
                const std::shared_ptr<FolderHistory>& folderHistLeft,
                const std::shared_ptr<FolderHistory>& folderHistRight,
                std::vector<std::wstring>& onCompletionHistory,
                size_t onCompletionHistoryMax);

private:
    virtual void OnCmpSettings    (wxCommandEvent& event);
    virtual void OnSyncSettings   (wxCommandEvent& event);
    virtual void OnConfigureFilter(wxCommandEvent& event);
    virtual void OnHelp           (wxCommandEvent& event) { displayHelpEntry(L"html/Schedule a Batch Job.html"); }

    void OnCompSettingsContext(wxCommandEvent& event);
    void OnSyncSettingsContext(wxCommandEvent& event);
    void OnGlobalFilterContext(wxCommandEvent& event);
    virtual void OnCheckSaveLog        (wxCommandEvent& event);
    virtual void OnChangeMaxLogCountTxt(wxCommandEvent& event);
    virtual void OnClose               (wxCloseEvent&   event) { EndModal(0); }
    virtual void OnCancel              (wxCommandEvent& event) { EndModal(0); }
    virtual void OnSaveBatchJob        (wxCommandEvent& event);
    virtual void OnLoadBatchJob        (wxCommandEvent& event);
    virtual void OnAddFolderPair       (wxCommandEvent& event);
    virtual void OnRemoveFolderPair    (wxCommandEvent& event);
    virtual void OnRemoveTopFolderPair (wxCommandEvent& event);
    void OnFilesDropped(FileDropEvent& event);

    void addFolderPair(const std::vector<zen::FolderPairEnh>& newPairs, bool addFront = false);
    void removeAddFolderPair(const int pos);
    void clearAddFolderPairs();

    void updateGuiForFolderPair();

    void updateGui(); //re-evaluate gui after config changes

    //error handling
    //xmlAccess::OnError getSelectionHandleError() const; -> obsolete, use getEnumVal()
    void setSelectionHandleError(xmlAccess::OnError value);
    void OnChangeErrorHandling(wxCommandEvent& event);

    bool saveBatchFile(const wxString& filename);
    void loadBatchFile(const wxString& filename);
    void loadBatchFile(const std::vector<wxString>& filenames);

    void setConfig(const xmlAccess::XmlBatchConfig& batchCfg);
    xmlAccess::XmlBatchConfig getConfig() const;

    std::unique_ptr<DirectoryPairBatchFirst> firstFolderPair; //always bound!!!
    std::vector<DirectoryPairBatch*> additionalFolderPairs;

    //used when saving batch file
    wxString proposedBatchFileName;

    xmlAccess::XmlBatchConfig localBatchCfg;

    std::unique_ptr<DirectoryName<FolderHistoryBox>> logfileDir;

    zen::EnumDescrList<xmlAccess::OnError> enumDescrMap;

    const std::shared_ptr<FolderHistory> folderHistLeft_;
    const std::shared_ptr<FolderHistory> folderHistRight_;
    std::vector<std::wstring>& onCompletionHistory_;
    size_t onCompletionHistoryMax_;
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
  DirectoryPairBatchFirst    DirectoryPairBatch
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
        return batchDlg.getConfig().mainCfg;
    }

    virtual void OnAltCompCfgChange()
    {
        batchDlg.updateGui();
    }

    virtual void OnAltSyncCfgChange()
    {
        batchDlg.updateGui();
    }

    virtual void removeAltCompCfg()
    {
        FolderPairPanelBasic<GuiPanel>::removeAltCompCfg();
        batchDlg.updateGui();
    }

    virtual void removeAltSyncCfg()
    {
        FolderPairPanelBasic<GuiPanel>::removeAltSyncCfg();
        batchDlg.updateGui();
    }

    virtual void OnLocalFilterCfgChange() {}

    BatchDialog& batchDlg;
};


class DirectoryPairBatch:
    public BatchFolderPairGenerated, //DirectoryPairBatch "owns" BatchFolderPairGenerated!
    public FolderPairCallback<BatchFolderPairGenerated>
{
public:
    DirectoryPairBatch(wxWindow* parent, BatchDialog& batchDialog) :
        BatchFolderPairGenerated(parent),
        FolderPairCallback<BatchFolderPairGenerated>(static_cast<BatchFolderPairGenerated&>(*this), batchDialog), //pass BatchFolderPairGenerated part...
        dirNameLeft (*m_panelLeft,  *m_dirPickerLeft,  *m_directoryLeft),
        dirNameRight(*m_panelRight, *m_dirPickerRight, *m_directoryRight) {}

    void setValues(const wxString& leftDir,
                   const wxString& rightDir,
                   AltCompCfgPtr cmpCfg,
                   AltSyncCfgPtr syncCfg,
                   const FilterConfig& filter)
    {
        setConfig(cmpCfg, syncCfg, filter);
        dirNameLeft.setName(leftDir);
        dirNameRight.setName(rightDir);
    }
    wxString getLeftDir () const { return dirNameLeft .getName(); }
    wxString getRightDir() const { return dirNameRight.getName(); }

private:
    //support for drag and drop
    DirectoryName<FolderHistoryBox> dirNameLeft;
    DirectoryName<FolderHistoryBox> dirNameRight;
};


class DirectoryPairBatchFirst : public FolderPairCallback<BatchDlgGenerated>
{
public:
    DirectoryPairBatchFirst(BatchDialog& batchDialog) :
        FolderPairCallback<BatchDlgGenerated>(batchDialog, batchDialog),

        //prepare drag & drop
        dirNameLeft(*batchDialog.m_panelLeft,
                    *batchDialog.m_dirPickerLeft,
                    *batchDialog.m_directoryLeft),
        dirNameRight(*batchDialog.m_panelRight,
                     *batchDialog.m_dirPickerRight,
                     *batchDialog.m_directoryRight) {}

    void setValues(const wxString& leftDir,
                   const wxString& rightDir,
                   AltCompCfgPtr cmpCfg,
                   AltSyncCfgPtr syncCfg,
                   const FilterConfig& filter)
    {
        setConfig(cmpCfg, syncCfg, filter);
        dirNameLeft.setName(leftDir);
        dirNameRight.setName(rightDir);
    }
    wxString getLeftDir () const { return dirNameLeft .getName(); }
    wxString getRightDir() const { return dirNameRight.getName(); }

private:
    //support for drag and drop
    DirectoryName<FolderHistoryBox> dirNameLeft;
    DirectoryName<FolderHistoryBox> dirNameRight;
};
//###################################################################################################################################


BatchDialog::BatchDialog(wxWindow* parent,
                         const wxString& referenceFile,
                         const xmlAccess::XmlBatchConfig& batchCfg,
                         const std::shared_ptr<FolderHistory>& folderHistLeft,
                         const std::shared_ptr<FolderHistory>& folderHistRight,
                         std::vector<std::wstring>& onCompletionHistory,
                         size_t onCompletionHistoryMax) :
    BatchDlgGenerated(parent),
    folderHistLeft_(folderHistLeft),
    folderHistRight_(folderHistRight),
    onCompletionHistory_(onCompletionHistory),
    onCompletionHistoryMax_(onCompletionHistoryMax)
{
    m_directoryLeft ->init(folderHistLeft_);
    m_directoryRight->init(folderHistRight_);

#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving main dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif

    wxWindowUpdateLocker dummy(this); //avoid display distortion

    enumDescrMap.
    add(xmlAccess::ON_ERROR_POPUP , _("Show pop-up")    , _("Show pop-up on errors or warnings")).
    add(xmlAccess::ON_ERROR_IGNORE, _("Ignore errors") , _("Hide all error and warning messages")).
    add(xmlAccess::ON_ERROR_EXIT  , _("Exit instantly"), _("Abort synchronization immediately"));


    m_bpButtonCmpConfig->SetBitmapLabel(GlobalResources::getImage(wxT("cmpConfig")));
    m_bpButtonSyncConfig->SetBitmapLabel(GlobalResources::getImage(wxT("syncConfig")));

    m_bpButtonHelp->SetBitmapLabel(GlobalResources::getImage(wxT("help")));

    //init handling of first folder pair
    firstFolderPair.reset(new DirectoryPairBatchFirst(*this));

    m_bpButtonCmpConfig ->Connect(wxEVT_RIGHT_DOWN, wxCommandEventHandler(BatchDialog::OnCompSettingsContext), nullptr, this);
    m_bpButtonSyncConfig->Connect(wxEVT_RIGHT_DOWN, wxCommandEventHandler(BatchDialog::OnSyncSettingsContext), nullptr, this);
    m_bpButtonFilter    ->Connect(wxEVT_RIGHT_DOWN, wxCommandEventHandler(BatchDialog::OnGlobalFilterContext), nullptr, this);

    //prepare drag & drop for loading of *.ffs_batch files
    setupFileDrop(*this);
    Connect(EVENT_DROP_FILE, FileDropEventHandler(BatchDialog::OnFilesDropped), nullptr, this);

    logfileDir.reset(new DirectoryName<FolderHistoryBox>(*m_panelBatchSettings, *m_dirPickerLogfileDir, *m_comboBoxLogfileDir));

    //set icons for this dialog
    m_bpButtonAddPair->SetBitmapLabel(GlobalResources::getImage(wxT("addFolderPair")));
    m_bitmap27->SetBitmap(GlobalResources::getImage(wxT("batch")));

    m_buttonSave->SetFocus();

    if (!referenceFile.empty())
    {
        SetTitle(referenceFile);
        proposedBatchFileName = referenceFile; //may be used on next save
    }

    setConfig(batchCfg);
}

//------------------- error handling --------------------------

void BatchDialog::setSelectionHandleError(xmlAccess::OnError value)
{
    //    if (m_checkBoxSilent->GetValue())
    setEnumVal(enumDescrMap, *m_choiceHandleError, value);
    /*  else
      {
          EnumDescrList<xmlAccess::OnError> tmp(enumDescrMap);
          tmp.descrList.pop_back(); //remove "Exit instantly" -> this option shall be available for silent mode only!
          setEnumVal(tmp, *m_choiceHandleError, value);
      }
      */
}

void BatchDialog::OnChangeErrorHandling(wxCommandEvent& event)
{
    updateTooltipEnumVal(enumDescrMap, *m_choiceHandleError);
}


void BatchDialog::OnCmpSettings(wxCommandEvent& event)
{
    //show window right next to the compare-config button
    //wxPoint windowPos = m_bpButtonCmpConfig->GetScreenPosition();
    //windowPos.x += m_bpButtonCmpConfig->GetSize().GetWidth() + 5;

    if (zen::showCompareCfgDialog(this,localBatchCfg.mainCfg.cmpConfig) == ReturnSmallDlg::BUTTON_OKAY)
    {
        updateGui();
    }
}


void BatchDialog::OnSyncSettings(wxCommandEvent& event)
{
    ExecWhenFinishedCfg ewfCfg = { &localBatchCfg.mainCfg.onCompletion,
                                   &onCompletionHistory_,
                                   onCompletionHistoryMax_
                                 };

    if (showSyncConfigDlg(this,
                          localBatchCfg.mainCfg.cmpConfig.compareVar,
                          localBatchCfg.mainCfg.syncCfg,
                          nullptr,
                          &ewfCfg) == ReturnSyncConfig::BUTTON_OKAY) //optional input parameter
    {
        updateGui();
    }
}


void BatchDialog::OnConfigureFilter(wxCommandEvent& event)
{
    if (showFilterDialog(this,
                         true, //is main filter dialog
                         localBatchCfg.mainCfg.globalFilter) == ReturnSmallDlg::BUTTON_OKAY)
    {
        updateGui();
    }
}


void BatchDialog::updateGui() //re-evaluate gui after config changes
{
    xmlAccess::XmlBatchConfig cfg = getConfig();

    //showNotebookpage(m_panelLogging, _("Logging"), cfg.silent);

    m_panelLogfile->Enable(cfg.logFileCountMax > 0);

    //update compare variant name
    m_staticTextCmpVariant->SetLabel(cfg.mainCfg.getCompVariantName());

    //update sync variant name
    m_staticTextSyncVariant->SetLabel(cfg.mainCfg.getSyncVariantName());

    //set filter icon
    if (!isNullFilter(cfg.mainCfg.globalFilter))
    {
        setImage(*m_bpButtonFilter, GlobalResources::getImage(L"filter"));
        m_bpButtonFilter->SetToolTip(_("Filter is active"));
    }
    else
    {
        setImage(*m_bpButtonFilter, greyScale(GlobalResources::getImage(L"filter")));
        m_bpButtonFilter->SetToolTip(_("No filter selected"));
    }

    m_panelOverview->Layout (); //adjust stuff inside scrolled window
    m_panelOverview->Refresh(); //refresh filter button (if nothing else)
}


void BatchDialog::OnCompSettingsContext(wxCommandEvent& event)
{
    ContextMenu menu;

    auto setVariant = [&](CompareVariant var)
    {
        localBatchCfg.mainCfg.cmpConfig.compareVar = var;
        updateGui();
    };

    auto currentVar = localBatchCfg.mainCfg.cmpConfig.compareVar;

    menu.addRadio(_("File time and size"), [&] { setVariant(CMP_BY_TIME_SIZE); }, currentVar == CMP_BY_TIME_SIZE);
    menu.addRadio(_("File content"      ), [&] { setVariant(CMP_BY_CONTENT);   }, currentVar == CMP_BY_CONTENT);

    menu.popup(*this);
}


void BatchDialog::OnSyncSettingsContext(wxCommandEvent& event)
{

    ContextMenu menu;

    auto setVariant = [&](DirectionConfig::Variant var)
    {
        localBatchCfg.mainCfg.syncCfg.directionCfg.var = var;
        updateGui();
    };

    const auto currentVar = localBatchCfg.mainCfg.syncCfg.directionCfg.var;

    menu.addRadio(_("<Automatic>"), [&] { setVariant(DirectionConfig::AUTOMATIC); }, currentVar == DirectionConfig::AUTOMATIC);
    menu.addRadio(_("Mirror ->>") , [&] { setVariant(DirectionConfig::MIRROR);    }, currentVar == DirectionConfig::MIRROR);
    menu.addRadio(_("Update ->")  , [&] { setVariant(DirectionConfig::UPDATE);    }, currentVar == DirectionConfig::UPDATE);
    menu.addRadio(_("Custom")     , [&] { setVariant(DirectionConfig::CUSTOM);    }, currentVar == DirectionConfig::CUSTOM);

    menu.popup(*this);
}


void BatchDialog::OnGlobalFilterContext(wxCommandEvent& event)
{
    ContextMenu menu;

    auto clearFilter = [&]
    {
        localBatchCfg.mainCfg.globalFilter = FilterConfig();
        updateGui();
    };
    menu.addItem( _("Clear filter settings"), clearFilter, nullptr, !isNullFilter(localBatchCfg.mainCfg.globalFilter));

    menu.popup(*this);
}


void BatchDialog::OnCheckSaveLog(wxCommandEvent& event)
{
    updateGui();

    //reset error handling depending on "m_checkBoxSilent"
    //setSelectionHandleError(getEnumVal(enumDescrMap, *m_choiceHandleError));
}


void BatchDialog::OnChangeMaxLogCountTxt(wxCommandEvent& event)
{
    updateGui();
}


void BatchDialog::OnFilesDropped(FileDropEvent& event)
{
    loadBatchFile(event.getFiles());
}


/*
void BatchDialog::showNotebookpage(wxWindow* page, const wxString& pageName, bool show)
{
    int windowPosition = -1;
    for (size_t i = 0; i < m_notebookSettings->GetPageCount(); ++i)
        if (m_notebookSettings->GetPage(i) == page)
        {
            windowPosition = static_cast<int>(i);
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
*/

void BatchDialog::OnSaveBatchJob(wxCommandEvent& event)
{
    //get a filename
    wxString defaultFileName = proposedBatchFileName.empty() ? wxT("SyncJob.ffs_batch") : proposedBatchFileName;

    //attention: proposedBatchFileName may be an imported *.ffs_gui file! We don't want to overwrite it with a BATCH config!
    if (endsWith(defaultFileName, L".ffs_gui"))
        replace(defaultFileName, L".ffs_gui", L".ffs_batch");

    wxFileDialog filePicker(this,
                            wxEmptyString,
                            wxEmptyString,
                            defaultFileName,
                            _("FreeFileSync batch") + L" (*.ffs_batch)|*.ffs_batch" + L"|" +_("All files") + L" (*.*)|*",
                            wxFD_SAVE | wxFD_OVERWRITE_PROMPT); //creating this on freestore leads to memleak!
    if (filePicker.ShowModal() == wxID_OK)
    {
        const wxString newFileName = filePicker.GetPath();

        saveBatchFile(newFileName);
        //if ()
        //    EndModal(ReturnBatchConfig::BATCH_FILE_SAVED);
    }
}


void BatchDialog::OnLoadBatchJob(wxCommandEvent& event)
{
    wxFileDialog filePicker(this,
                            wxEmptyString,
                            beforeLast(proposedBatchFileName, utfCvrtTo<wxString>(FILE_NAME_SEPARATOR)), //set default dir: empty string if "currentConfigFileName" is empty or has no path separator
                            wxEmptyString,
                            _("FreeFileSync batch") + L" (*.ffs_batch;*.ffs_gui)|*.ffs_batch;*.ffs_gui" + L"|" +_("All files") + L" (*.*)|*",
                            wxFD_OPEN | wxFD_MULTIPLE); //creating this on freestore leads to memleak!
    if (filePicker.ShowModal() == wxID_OK)
    {
        wxArrayString tmp;
        filePicker.GetPaths(tmp);
        std::vector<wxString> fileNames(tmp.begin(), tmp.end());

        loadBatchFile(fileNames);
    }
}



inline
FolderPairEnh getEnhancedPair(const DirectoryPairBatch* panel)
{
    return FolderPairEnh(toZ(panel->getLeftDir()),
                         toZ(panel->getRightDir()),
                         panel->getAltCompConfig(),
                         panel->getAltSyncConfig(),
                         panel->getAltFilterConfig());
}


bool BatchDialog::saveBatchFile(const wxString& filename)
{
    const xmlAccess::XmlBatchConfig batchCfg = getConfig();

    //write config to XML
    try
    {
        xmlAccess::writeConfig(batchCfg, toZ(filename));
    }
    catch (const xmlAccess::FfsXmlError& error)
    {
        wxMessageBox(error.toString().c_str(), _("Error"), wxOK | wxICON_ERROR, this);
        return false;
    }

    SetTitle(filename);
    proposedBatchFileName = filename; //may be used on next save

    return true;
}


void BatchDialog::loadBatchFile(const wxString& filename)
{
    std::vector<wxString> filenames;
    filenames.push_back(filename);
    loadBatchFile(filenames);
}


void BatchDialog::loadBatchFile(const std::vector<wxString>& filenames)
{
    if (filenames.empty())
        return;

    //load XML settings
    xmlAccess::XmlBatchConfig batchCfg;  //structure to receive gui settings
    try
    {
        //open a *.ffs_gui or *.ffs_batch file!
        xmlAccess::convertConfig(toZ(filenames), batchCfg); //throw FfsXmlError

        //xmlAccess::readConfig(filename, batchCfg);
    }
    catch (const xmlAccess::FfsXmlError& error)
    {
        if (error.getSeverity() == xmlAccess::FfsXmlError::WARNING)
            wxMessageBox(error.toString(), _("Warning"), wxOK | wxICON_WARNING, this);
        else
        {
            wxMessageBox(error.toString(), _("Error"), wxOK | wxICON_ERROR, this);
            return;
        }
    }

    const wxString activeFile = filenames.size() == 1 ? filenames[0] : wxString();

    if (activeFile.empty())
        SetTitle(_("Create a batch job"));
    else
        SetTitle(activeFile);

    proposedBatchFileName = activeFile; //may be used on next save

    setConfig(batchCfg);
}


void BatchDialog::setConfig(const xmlAccess::XmlBatchConfig& batchCfg)
{
    wxWindowUpdateLocker dummy(this); //avoid display distortion

    //make working copy
    localBatchCfg = batchCfg;

    m_checkBoxShowProgress->SetValue(batchCfg.showProgress);

    //error handling is dependent from m_checkBoxShowProgress! /|\   \|/
    setSelectionHandleError(batchCfg.handleError);

    logfileDir->setName(batchCfg.logFileDirectory);
    m_spinCtrlLogCountMax->SetValue(static_cast<int>(batchCfg.logFileCountMax)); //attention: this one emits a "change value" event!! => updateGui() called implicitly!

    //set first folder pair
    firstFolderPair->setValues(toWx(batchCfg.mainCfg.firstPair.leftDirectory),
                               toWx(batchCfg.mainCfg.firstPair.rightDirectory),
                               batchCfg.mainCfg.firstPair.altCmpConfig,
                               batchCfg.mainCfg.firstPair.altSyncConfig,
                               batchCfg.mainCfg.firstPair.localFilter);

    //remove existing additional folder pairs
    clearAddFolderPairs();

    //set additional pairs
    addFolderPair(batchCfg.mainCfg.additionalPairs);

    updateGui(); //re-evaluate gui after config changes

    Fit(); //needed
    Refresh(); //needed
    Centre();
}


xmlAccess::XmlBatchConfig BatchDialog::getConfig() const
{
    xmlAccess::XmlBatchConfig batchCfg = localBatchCfg;

    //load parameter with ownership within wxWidgets controls...

    //first folder pair
    batchCfg.mainCfg.firstPair = FolderPairEnh(toZ(firstFolderPair->getLeftDir()),
                                               toZ(firstFolderPair->getRightDir()),
                                               firstFolderPair->getAltCompConfig(),
                                               firstFolderPair->getAltSyncConfig(),
                                               firstFolderPair->getAltFilterConfig());

    //add additional pairs
    batchCfg.mainCfg.additionalPairs.clear();
    std::transform(additionalFolderPairs.begin(), additionalFolderPairs.end(),
                   std::back_inserter(batchCfg.mainCfg.additionalPairs), getEnhancedPair);


    //load structure with batch settings "batchCfg"
    batchCfg.showProgress     = m_checkBoxShowProgress->GetValue();
    batchCfg.logFileDirectory = logfileDir->getName();
    batchCfg.logFileCountMax  = m_spinCtrlLogCountMax->GetValue();
    batchCfg.handleError      = getEnumVal(enumDescrMap, *m_choiceHandleError);

    return batchCfg;
}


void BatchDialog::OnAddFolderPair(wxCommandEvent& event)
{
    wxWindowUpdateLocker dummy(this); //avoid display distortion

    std::vector<FolderPairEnh> newPairs;
    newPairs.push_back(getConfig().mainCfg.firstPair);

    //clear first pair
    const FolderPairEnh cfgEmpty;
    firstFolderPair->setValues(toWx(cfgEmpty.leftDirectory),
                               toWx(cfgEmpty.rightDirectory),
                               cfgEmpty.altCmpConfig,
                               cfgEmpty.altSyncConfig,
                               cfgEmpty.localFilter);

    //keep sequence to update GUI as last step
    addFolderPair(newPairs, true); //add pair in front of additonal pairs
}


void BatchDialog::OnRemoveFolderPair(wxCommandEvent& event)
{
    //find folder pair originating the event
    const wxObject* const eventObj = event.GetEventObject();
    for (std::vector<DirectoryPairBatch*>::const_iterator i = additionalFolderPairs.begin(); i != additionalFolderPairs.end(); ++i)
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
    if (!additionalFolderPairs.empty())
    {
        //get settings from second folder pair
        const FolderPairEnh cfgSecond = getEnhancedPair(additionalFolderPairs[0]);

        //reset first pair
        firstFolderPair->setValues(toWx(cfgSecond.leftDirectory),
                                   toWx(cfgSecond.rightDirectory),
                                   cfgSecond.altCmpConfig,
                                   cfgSecond.altSyncConfig,
                                   cfgSecond.localFilter);

        removeAddFolderPair(0); //remove second folder pair (first of additional folder pairs)
    }
}


void BatchDialog::updateGuiForFolderPair()
{
    //adapt delete top folder pair button
    if (additionalFolderPairs.empty())
        m_bpButtonRemovePair->Hide();
    else
        m_bpButtonRemovePair->Show();

    //adapt local filter and sync cfg for first folder pair
    if (additionalFolderPairs.empty() &&
        firstFolderPair->getAltCompConfig().get() == nullptr &&
        firstFolderPair->getAltSyncConfig().get() == nullptr &&
        isNullFilter(firstFolderPair->getAltFilterConfig()))
    {
        m_bpButtonAltCompCfg ->Hide();
        m_bpButtonLocalFilter->Hide();
        m_bpButtonAltSyncCfg ->Hide();
    }
    else
    {
        m_bpButtonAltCompCfg ->Show();
        m_bpButtonLocalFilter->Show();
        m_bpButtonAltSyncCfg ->Show();
    }

    //update controls
    const int maxAddFolderPairsVisible = 2;

    int pairHeight = sbSizerMainPair->GetSize().GetHeight(); //respect height of main pair
    if (!additionalFolderPairs.empty())
        pairHeight += std::min<double>(maxAddFolderPairsVisible + 0.5, additionalFolderPairs.size()) * //have 0.5 * height indicate that more folders are there
                      additionalFolderPairs[0]->GetSize().GetHeight();

    m_scrolledWindow6->SetMinSize(wxSize( -1, pairHeight));


    m_scrolledWindow6->Fit();     //adjust scrolled window size
    m_scrolledWindow6->Layout();  //adjust scrolled window size

    //bSizerAddFolderPairs->FitInside(m_scrolledWindow6);  //adjust scrolled window size

    m_panelOverview->Layout(); //adjust stuff inside scrolled window
    //m_panelOverview->InvalidateBestSize(); //needed for Fit() to work correctly!

    if (m_scrolledWindow6->GetSize().GetHeight() < pairHeight)
        Fit();                     //adapt dialog size
}


void BatchDialog::addFolderPair(const std::vector<zen::FolderPairEnh>& newPairs, bool addFront)
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    if (!newPairs.empty())
    {
        //add folder pairs
        for (std::vector<zen::FolderPairEnh>::const_iterator i = newPairs.begin(); i != newPairs.end(); ++i)
        {
            DirectoryPairBatch* newPair = new DirectoryPairBatch(m_scrolledWindow6, *this); //owned by m_scrolledWindow6!

            //init dropdown history
            newPair->m_directoryLeft ->init(folderHistLeft_);
            newPair->m_directoryRight->init(folderHistRight_);

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

            //register events
            newPair->m_bpButtonRemovePair->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BatchDialog::OnRemoveFolderPair), nullptr, this );

            //set alternate configuration
            newPair->setValues(toWx(i->leftDirectory),
                               toWx(i->rightDirectory),
                               i->altCmpConfig,
                               i->altSyncConfig,
                               i->localFilter);
        }

        //after changing folder pairs window focus is lost: results in scrolled window scrolling to top each time window is shown: we don't want this
        m_bpButtonAddPair->SetFocus();
    }

    updateGuiForFolderPair();

    updateGui(); //mainly to update sync dir description text
}


void BatchDialog::removeAddFolderPair(const int pos)
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    if (0 <= pos && pos < static_cast<int>(additionalFolderPairs.size()))
    {
        //remove folder pairs from window
        DirectoryPairBatch* pairToDelete = additionalFolderPairs[pos];

        bSizerAddFolderPairs->Detach(pairToDelete); //Remove() does not work on Window*, so do it manually
        pairToDelete->Destroy();                 //
        additionalFolderPairs.erase(additionalFolderPairs.begin() + pos); //remove last element in vector

        //after changing folder pairs window focus is lost: results in scrolled window scrolling to top each time window is shown: we don't want this
        m_bpButtonRemovePair->SetFocus();
    }

    updateGuiForFolderPair();

    updateGui(); //mainly to update sync dir description text
}


void BatchDialog::clearAddFolderPairs()
{
    wxWindowUpdateLocker dummy(m_panelOverview); //avoid display distortion

    additionalFolderPairs.clear();
    bSizerAddFolderPairs->Clear(true);

    updateGuiForFolderPair();
}


void zen::showSyncBatchDlg(wxWindow* parent,
                           const wxString& referenceFile,
                           const xmlAccess::XmlBatchConfig& batchCfg,
                           const std::shared_ptr<FolderHistory>& folderHistLeft,
                           const std::shared_ptr<FolderHistory>& folderHistRight,
                           std::vector<std::wstring>& execFinishedhistory,
                           size_t execFinishedhistoryMax)
{
    BatchDialog batchDlg(parent, referenceFile, batchCfg, folderHistLeft, folderHistRight, execFinishedhistory, execFinishedhistoryMax);
    batchDlg.ShowModal();
}
