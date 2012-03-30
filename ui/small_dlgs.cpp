// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) ZenJu (zhnmju123 AT gmx DOT de) - All Rights Reserved    *
// **************************************************************************

#include "gui_generated.h"
#include "small_dlgs.h"
#include "msg_popup.h"
#include "../lib/resources.h"
#include "../algorithm.h"
#include <wx+/string_conv.h>
#include <wx+/format_unit.h>
#include <wx+/choice_enum.h>
#include "../synchronization.h"
#include "custom_grid.h"
#include <wx+/button.h>
#include <zen/build_info.h>
#include <wx/wupdlock.h>
#include <wx/msgdlg.h>
#include <wx+/mouse_move_dlg.h>
#include <wx+/rtl.h>
#include "../lib/help_provider.h"
#include <wx+/image_tools.h>
#include <zen/stl_tools.h>
#include "../lib/hard_filter.h"

using namespace zen;


class AboutDlg : public AboutDlgGenerated
{
public:
    AboutDlg(wxWindow* parent);

private:
    void OnClose(wxCloseEvent&   event) { EndModal(0); }
    void OnOK   (wxCommandEvent& event) { EndModal(0); }
};


AboutDlg::AboutDlg(wxWindow* parent) : AboutDlgGenerated(parent)
{
    m_bitmap9 ->SetBitmap(GlobalResources::getImage(L"website"));
    m_bitmap10->SetBitmap(GlobalResources::getImage(L"email"));
    m_bitmap11->SetBitmap(GlobalResources::getImage(L"logo"));
    m_bitmap13->SetBitmap(GlobalResources::getImage(L"gpl"));
    //m_bitmapTransl->SetBitmap(GlobalResources::getImage(wxT("translation")));
    m_bitmapPaypal->SetBitmap(GlobalResources::getImage(L"paypal"));

    //create language credits
    for (auto iter = ExistingTranslations::get().begin(); iter != ExistingTranslations::get().end(); ++iter)
    {
        //flag
        wxStaticBitmap* staticBitmapFlag = new wxStaticBitmap(m_scrolledWindowTranslators, wxID_ANY, GlobalResources::getImage(iter->languageFlag), wxDefaultPosition, wxSize(-1, 11), 0 );
        fgSizerTranslators->Add(staticBitmapFlag, 0, wxALIGN_CENTER);

        //language name
        wxStaticText* staticTextLanguage = new wxStaticText(m_scrolledWindowTranslators, wxID_ANY, iter->languageName, wxDefaultPosition, wxDefaultSize, 0 );
        staticTextLanguage->Wrap(-1);
        fgSizerTranslators->Add(staticTextLanguage, 0, wxALIGN_CENTER_VERTICAL);

        //translator name
        wxStaticText* staticTextTranslator = new wxStaticText(m_scrolledWindowTranslators, wxID_ANY, iter->translatorName, wxDefaultPosition, wxDefaultSize, 0 );
        staticTextTranslator->Wrap(-1);
        fgSizerTranslators->Add(staticTextTranslator, 0, wxALIGN_CENTER_VERTICAL);
    }

    bSizerTranslators->Fit(m_scrolledWindowTranslators);


    //build information
    wxString build = __TDATE__;
#if wxUSE_UNICODE
    build += wxT(" - Unicode");
#else
    build += wxT(" - ANSI");
#endif //wxUSE_UNICODE

    //compile time info about 32/64-bit build
    if (zen::is64BitBuild)
        build += wxT(" x64");
    else
        build += wxT(" x86");
    assert_static(zen::is32BitBuild || zen::is64BitBuild);

    wxString buildFormatted = _("(Build: %x)");
    buildFormatted.Replace(wxT("%x"), build);

    m_build->SetLabel(buildFormatted);

    //m_animationControl1->SetAnimation(GlobalResources::instance().animationMoney);
    //m_animationControl1->Play();

    m_buttonOkay->SetFocus();
    Fit();

#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving main dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif
}


void zen::showAboutDialog()
{
    AboutDlg aboutDlg(nullptr);
    aboutDlg.ShowModal();
}
//########################################################################################


class FilterDlg : public FilterDlgGenerated
{
public:
    FilterDlg(wxWindow* parent,
              bool isGlobalFilter,
              FilterConfig& filter);
    ~FilterDlg() {}

private:
    void OnClose       (  wxCloseEvent& event) { EndModal(0); }
    void OnCancel      (wxCommandEvent& event) { EndModal(0); }
    void OnHelp        (wxCommandEvent& event);
    void OnDefault     (wxCommandEvent& event);
    void OnApply       (wxCommandEvent& event);
    void OnUpdateChoice(wxCommandEvent& event) { updateGui(); }
    void OnUpdateNameFilter(wxCommandEvent& event) { updateGui(); }

    void updateGui();
    void setFilter(const FilterConfig& filter);
    FilterConfig getFilter() const;
    void onKeyEvent(wxKeyEvent& event);

    const bool isGlobalFilter_;
    FilterConfig& outputRef;

    EnumDescrList<UnitTime> enumTimeDescr;
    EnumDescrList<UnitSize> enumSizeDescr;
};


FilterDlg::FilterDlg(wxWindow* parent,
                     bool isGlobalFilter, //global or local filter dialog?
                     FilterConfig& filter) :
    FilterDlgGenerated(parent),
    isGlobalFilter_(isGlobalFilter),
    outputRef(filter) //just hold reference
{
#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving main dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif

    m_textCtrlInclude->SetMaxLength(0); //allow large filter entries!
    m_textCtrlExclude->SetMaxLength(0); //

    m_textCtrlInclude->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(FilterDlg::onKeyEvent), nullptr, this);
    m_textCtrlExclude->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(FilterDlg::onKeyEvent), nullptr, this);

    enumTimeDescr.
    add(UTIME_NONE,        _("Inactive")).
    add(UTIME_TODAY,       _("Today")).
    //    add(UTIME_THIS_WEEK,   _("This week")).
    add(UTIME_THIS_MONTH,  _("This month")).
    add(UTIME_THIS_YEAR,   _("This year")).
    add(UTIME_LAST_X_DAYS, _("Last x days"));

    enumSizeDescr.
    add(USIZE_NONE, _("Inactive")).
    add(USIZE_BYTE, _("Byte")).
    add(USIZE_KB,   _("KB")).
    add(USIZE_MB,   _("MB"));

    m_bitmap26->SetBitmap(GlobalResources::getImage(L"filterOn"));
    m_bpButtonHelp->SetBitmapLabel(GlobalResources::getImage(L"help"));

    setFilter(filter);

    m_panel13->Hide();
    m_button10->SetFocus();

    //adapt header for global/local dialog
    //    if (isGlobalFilter_)
    //        m_staticTexHeader->SetLabel("Filter all folder pairs"));
    //    else
    //        m_staticTexHeader->SetLabel("Filter single folder pair"));
    //
    m_staticTexHeader->SetLabel(_("Filter"));

    Fit();
}


void FilterDlg::onKeyEvent(wxKeyEvent& event)
{
    const int keyCode = event.GetKeyCode();

    if (event.ControlDown())
        switch (keyCode)
        {
            case 'A': //CTRL + A
            {
                if (auto textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject()))
                    textCtrl->SetSelection(-1, -1); //select all
                return;
            }
        }
    event.Skip();
}


void FilterDlg::updateGui()
{
    FilterConfig activeCfg = getFilter();

    m_bitmapInclude->SetBitmap(
        !NameFilter::isNull(activeCfg.includeFilter, FilterConfig().excludeFilter) ?
        GlobalResources::getImage(L"include") :
        greyScale(GlobalResources::getImage(L"include")));

    m_bitmapExclude->SetBitmap(
        !NameFilter::isNull(FilterConfig().includeFilter, activeCfg.excludeFilter) ?
        GlobalResources::getImage(L"exclude") :
        greyScale(GlobalResources::getImage(L"exclude")));

    m_bitmapFilterDate->SetBitmap(
        activeCfg.unitTimeSpan != UTIME_NONE ?
        GlobalResources::getImage(L"clock") :
        greyScale(GlobalResources::getImage(L"clock")));

    m_bitmapFilterSize->SetBitmap(
        activeCfg.unitSizeMin != USIZE_NONE ||
        activeCfg.unitSizeMax != USIZE_NONE ?
        GlobalResources::getImage(L"size") :
        greyScale(GlobalResources::getImage(L"size")));

    m_spinCtrlTimespan->Enable(activeCfg.unitTimeSpan == UTIME_LAST_X_DAYS);
    m_spinCtrlMinSize ->Enable(activeCfg.unitSizeMin != USIZE_NONE);
    m_spinCtrlMaxSize ->Enable(activeCfg.unitSizeMax != USIZE_NONE);
}


void FilterDlg::setFilter(const FilterConfig& filter)
{
    m_textCtrlInclude->ChangeValue(toWx(filter.includeFilter));
    m_textCtrlExclude->ChangeValue(toWx(filter.excludeFilter));

    setEnumVal(enumTimeDescr, *m_choiceUnitTimespan, filter.unitTimeSpan);
    setEnumVal(enumSizeDescr, *m_choiceUnitMinSize,  filter.unitSizeMin);
    setEnumVal(enumSizeDescr, *m_choiceUnitMaxSize,  filter.unitSizeMax);

    m_spinCtrlTimespan->SetValue(static_cast<int>(filter.timeSpan));
    m_spinCtrlMinSize ->SetValue(static_cast<int>(filter.sizeMin));
    m_spinCtrlMaxSize ->SetValue(static_cast<int>(filter.sizeMax));

    updateGui();
}


FilterConfig FilterDlg::getFilter() const
{
    return FilterConfig(toZ(m_textCtrlInclude->GetValue()),
                        toZ(m_textCtrlExclude->GetValue()),
                        m_spinCtrlTimespan->GetValue(),
                        getEnumVal(enumTimeDescr, *m_choiceUnitTimespan),
                        m_spinCtrlMinSize->GetValue(),
                        getEnumVal(enumSizeDescr, *m_choiceUnitMinSize),
                        m_spinCtrlMaxSize->GetValue(),
                        getEnumVal(enumSizeDescr, *m_choiceUnitMaxSize));
}


void FilterDlg::OnHelp(wxCommandEvent& event)
{
    m_bpButtonHelp->Hide();
    m_panel13->Show();
    Fit();
    Refresh();

    event.Skip();
}


void FilterDlg::OnDefault(wxCommandEvent& event)
{
    if (isGlobalFilter_)
        setFilter(MainConfiguration().globalFilter);
    else
        setFilter(FilterConfig());

    //changes to mainDialog are only committed when the OK button is pressed
    Fit();
}


void FilterDlg::OnApply(wxCommandEvent& event)
{
    //only if user presses ApplyFilter, he wants the changes to be committed
    outputRef = getFilter();

    //when leaving dialog: filter and redraw grid, if filter is active
    EndModal(ReturnSmallDlg::BUTTON_OKAY);
}


ReturnSmallDlg::ButtonPressed zen::showFilterDialog(bool isGlobalFilter, FilterConfig& filter)
{
    FilterDlg filterDlg(nullptr,
                        isGlobalFilter, //is main filter dialog
                        filter);
    return static_cast<ReturnSmallDlg::ButtonPressed>(filterDlg.ShowModal());
}
//########################################################################################


class DeleteDialog : public DeleteDlgGenerated
{
public:
    DeleteDialog(wxWindow* parent,
                 const std::vector<zen::FileSystemObject*>& rowsOnLeft,
                 const std::vector<zen::FileSystemObject*>& rowsOnRight,
                 bool& deleteOnBothSides,
                 bool& useRecycleBin);

private:
    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event) { EndModal(ReturnSmallDlg::BUTTON_CANCEL); }
    void OnClose (wxCloseEvent&   event) { EndModal(ReturnSmallDlg::BUTTON_CANCEL); }
    void OnDelOnBothSides(wxCommandEvent& event);
    void OnUseRecycler(wxCommandEvent& event);

    void updateGui();

    const std::vector<zen::FileSystemObject*>& rowsToDeleteOnLeft;
    const std::vector<zen::FileSystemObject*>& rowsToDeleteOnRight;
    bool& outRefdeleteOnBothSides;
    bool& outRefuseRecycleBin;
};


DeleteDialog::DeleteDialog(wxWindow* parent,
                           const std::vector<FileSystemObject*>& rowsOnLeft,
                           const std::vector<FileSystemObject*>& rowsOnRight,
                           bool& deleteOnBothSides,
                           bool& useRecycleBin) :
    DeleteDlgGenerated(parent),
    rowsToDeleteOnLeft(rowsOnLeft),
    rowsToDeleteOnRight(rowsOnRight),
    outRefdeleteOnBothSides(deleteOnBothSides),
    outRefuseRecycleBin(useRecycleBin)
{
#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving main dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif

    m_checkBoxDeleteBothSides->SetValue(deleteOnBothSides);
    m_checkBoxUseRecycler->SetValue(useRecycleBin);

    //if both sides contain same rows this checkbox is superfluous
    if (rowsToDeleteOnLeft == rowsToDeleteOnRight)
    {
        m_checkBoxDeleteBothSides->Show(false);
        m_checkBoxDeleteBothSides->SetValue(true);
    }

    updateGui();

    m_buttonOK->SetFocus();
}


void DeleteDialog::updateGui()
{
    const std::pair<wxString, int> delInfo = zen::deleteFromGridAndHDPreview(
                                                 rowsToDeleteOnLeft,
                                                 rowsToDeleteOnRight,
                                                 m_checkBoxDeleteBothSides->GetValue());
    wxString header;
    if (m_checkBoxUseRecycler->GetValue())
    {
        header = _P("Do you really want to move the following object to the Recycle Bin?",
                    "Do you really want to move the following %x objects to the Recycle Bin?", delInfo.second);
        m_bitmap12->SetBitmap(GlobalResources::getImage(wxT("recycler")));
    }
    else
    {
        header = _P("Do you really want to delete the following object?",
                    "Do you really want to delete the following %x objects?", delInfo.second);
        m_bitmap12->SetBitmap(GlobalResources::getImage(wxT("deleteFile")));
    }
    replace(header, L"%x", toStringSep(delInfo.second));
    m_staticTextHeader->SetLabel(header);

    const wxString& filesToDelete = delInfo.first;
    m_textCtrlMessage->SetValue(filesToDelete);

    Layout();
}


void DeleteDialog::OnOK(wxCommandEvent& event)
{
    outRefuseRecycleBin = m_checkBoxUseRecycler->GetValue();
    if (rowsToDeleteOnLeft != rowsToDeleteOnRight)
        outRefdeleteOnBothSides = m_checkBoxDeleteBothSides->GetValue();

    EndModal(ReturnSmallDlg::BUTTON_OKAY);
}

void DeleteDialog::OnDelOnBothSides(wxCommandEvent& event)
{
    updateGui();
}

void DeleteDialog::OnUseRecycler(wxCommandEvent& event)
{
    updateGui();
}


ReturnSmallDlg::ButtonPressed zen::showDeleteDialog(const std::vector<zen::FileSystemObject*>& rowsOnLeft,
                                                    const std::vector<zen::FileSystemObject*>& rowsOnRight,
                                                    bool& deleteOnBothSides,
                                                    bool& useRecycleBin)
{
    DeleteDialog confirmDeletion(nullptr,
                                 rowsOnLeft,
                                 rowsOnRight,
                                 deleteOnBothSides,
                                 useRecycleBin);
    return static_cast<ReturnSmallDlg::ButtonPressed>(confirmDeletion.ShowModal());
}

//########################################################################################

class SyncPreviewDlg : public SyncPreviewDlgGenerated
{
public:
    SyncPreviewDlg(wxWindow* parent,
                   const wxString& variantName,
                   const zen::SyncStatistics& statistics,
                   bool& dontShowAgain);
private:
    void OnClose(wxCloseEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnStartSync(wxCommandEvent& event);

    bool& m_dontShowAgain;
};



SyncPreviewDlg::SyncPreviewDlg(wxWindow* parent,
                               const wxString& variantName,
                               const zen::SyncStatistics& statistics,
                               bool& dontShowAgain) :
    SyncPreviewDlgGenerated(parent),
    m_dontShowAgain(dontShowAgain)
{
#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving main dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif

    using zen::toStringSep;

    m_buttonStartSync->setBitmapFront(GlobalResources::getImage(wxT("startSync")));

    m_bitmapCreate->SetBitmap(mirrorIfRtl(GlobalResources::getImage(L"create")));
    m_bitmapUpdate->SetBitmap(mirrorIfRtl(GlobalResources::getImage(L"update")));
    m_bitmapDelete->SetBitmap(mirrorIfRtl(GlobalResources::getImage(L"delete")));
    m_bitmapData  ->SetBitmap(mirrorIfRtl(GlobalResources::getImage(L"data")));

    m_staticTextVariant->SetLabel(variantName);
    m_textCtrlData->SetValue(zen::filesizeToShortString(statistics.getDataToProcess()));

    m_textCtrlCreateL->SetValue(toStringSep(statistics.getCreate<LEFT_SIDE>()));
    m_textCtrlUpdateL->SetValue(toStringSep(statistics.getUpdate<LEFT_SIDE>()));
    m_textCtrlDeleteL->SetValue(toStringSep(statistics.getDelete<LEFT_SIDE>()));

    m_textCtrlCreateR->SetValue(toStringSep(statistics.getCreate<RIGHT_SIDE>()));
    m_textCtrlUpdateR->SetValue(toStringSep(statistics.getUpdate<RIGHT_SIDE>()));
    m_textCtrlDeleteR->SetValue(toStringSep(statistics.getDelete<RIGHT_SIDE>()));

    m_checkBoxDontShowAgain->SetValue(dontShowAgain);

    m_buttonStartSync->SetFocus();
    Fit();
}


void SyncPreviewDlg::OnClose(wxCloseEvent& event)
{
    EndModal(ReturnSmallDlg::BUTTON_CANCEL);
}


void SyncPreviewDlg::OnCancel(wxCommandEvent& event)
{
    EndModal(ReturnSmallDlg::BUTTON_CANCEL);
}


void SyncPreviewDlg::OnStartSync(wxCommandEvent& event)
{
    m_dontShowAgain = m_checkBoxDontShowAgain->GetValue();
    EndModal(ReturnSmallDlg::BUTTON_OKAY);
}


ReturnSmallDlg::ButtonPressed zen::showSyncPreviewDlg(
    const wxString& variantName,
    const zen::SyncStatistics& statistics,
    bool& dontShowAgain)
{
    SyncPreviewDlg preview(nullptr,
                           variantName,
                           statistics,
                           dontShowAgain);

    return static_cast<ReturnSmallDlg::ButtonPressed>(preview.ShowModal());
}
//########################################################################################


class CompareCfgDialog : public CmpCfgDlgGenerated
{
public:
    CompareCfgDialog(wxWindow* parent,
                     CompConfig& cmpConfig);

private:
    void OnOkay(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event) { EndModal(0); }
    void OnCancel(wxCommandEvent& event) { EndModal(0); }
    void OnShowHelp(wxCommandEvent& event);

    void OnTimeSize(wxCommandEvent& event) { m_radioBtnSizeDate->SetValue(true); }
    //void OnFilesize(wxCommandEvent& event) { m_radioBtnSize    ->SetValue(true); }
    void OnContent (wxCommandEvent& event) { m_radioBtnContent ->SetValue(true); }

    void OnTimeSizeDouble(wxMouseEvent& event);
    void OnFilesizeDouble(wxMouseEvent& event);
    void OnContentDouble(wxMouseEvent& event);

    void updateView();

    CompConfig& cmpConfigOut;

    zen::EnumDescrList<SymLinkHandling>  enumDescrHandleSyml;
};


CompareCfgDialog::CompareCfgDialog(wxWindow* parent,
                                   CompConfig& cmpConfig) :
    CmpCfgDlgGenerated(parent),
    cmpConfigOut(cmpConfig)
{
#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving main dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif

    enumDescrHandleSyml.
    add(SYMLINK_IGNORE,       _("Exclude")).
    add(SYMLINK_USE_DIRECTLY, _("Direct")).
    add(SYMLINK_FOLLOW_LINK,  _("Follow"));

    //move dialog up so that compare-config button and first config-variant are on same level
    //   Move(wxPoint(position.x, std::max(0, position.y - (m_buttonTimeSize->GetScreenPosition() - GetScreenPosition()).y)));

    m_bpButtonHelp   ->SetBitmapLabel(GlobalResources::getImage(wxT("help")));
    m_bitmapByTime   ->SetBitmap     (GlobalResources::getImage(wxT("clock")));
    //m_bitmapBySize   ->SetBitmap     (GlobalResources::getImage(wxT("size")));
    m_bitmapByContent->SetBitmap     (GlobalResources::getImage(wxT("cmpByContent")));

    switch (cmpConfig.compareVar)
    {
        case CMP_BY_TIME_SIZE:
            m_radioBtnSizeDate->SetValue(true);
            m_buttonContent->SetFocus(); //set focus on the other button
            break;
        case CMP_BY_CONTENT:
            m_radioBtnContent->SetValue(true);
            m_buttonTimeSize->SetFocus(); //set focus on the other button
            break;
    }

    setEnumVal(enumDescrHandleSyml, *m_choiceHandleSymlinks, cmpConfig.handleSymlinks);

    updateView();
}

void CompareCfgDialog::updateView()
{
    Fit();
}

void CompareCfgDialog::OnOkay(wxCommandEvent& event)
{
    if (m_radioBtnContent->GetValue())
        cmpConfigOut.compareVar = CMP_BY_CONTENT;
    else
        cmpConfigOut.compareVar = CMP_BY_TIME_SIZE;

    cmpConfigOut.handleSymlinks = getEnumVal(enumDescrHandleSyml, *m_choiceHandleSymlinks);

    EndModal(ReturnSmallDlg::BUTTON_OKAY);
}


void CompareCfgDialog::OnTimeSizeDouble(wxMouseEvent& event)
{
    wxCommandEvent dummy;
    OnTimeSize(dummy);
    OnOkay(dummy);
}


void CompareCfgDialog::OnContentDouble(wxMouseEvent& event)
{
    wxCommandEvent dummy;
    OnContent(dummy);
    OnOkay(dummy);
}


void CompareCfgDialog::OnShowHelp(wxCommandEvent& event)
{
#ifdef FFS_WIN
    zen::displayHelpEntry(wxT("html\\ComparisonSettings.html"));
#elif defined FFS_LINUX
    zen::displayHelpEntry(wxT("html/ComparisonSettings.html"));
#endif
}


ReturnSmallDlg::ButtonPressed zen::showCompareCfgDialog(CompConfig& cmpConfig)
{
    CompareCfgDialog syncDlg(nullptr, cmpConfig);

    return static_cast<ReturnSmallDlg::ButtonPressed>(syncDlg.ShowModal());
}
//########################################################################################


class GlobalSettingsDlg : public GlobalSettingsDlgGenerated
{
public:
    GlobalSettingsDlg(wxWindow* parent, xmlAccess::XmlGlobalSettings& globalSettings);

private:
    void OnOkay(wxCommandEvent& event);
    void OnResetDialogs(wxCommandEvent& event);
    void OnDefault(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnAddRow(wxCommandEvent& event);
    void OnRemoveRow(wxCommandEvent& event);
    void OnResize(wxSizeEvent& event);

    void set(const xmlAccess::ExternalApps& extApp);
    xmlAccess::ExternalApps getExtApp();

    xmlAccess::XmlGlobalSettings& settings;
};


GlobalSettingsDlg::GlobalSettingsDlg(wxWindow* parent, xmlAccess::XmlGlobalSettings& globalSettings) :
    GlobalSettingsDlgGenerated(parent),
    settings(globalSettings)
{
#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif

    m_bitmapSettings    ->SetBitmap     (GlobalResources::getImage(wxT("settings")));
    m_buttonResetDialogs->setBitmapFront(GlobalResources::getImage(wxT("warningSmall")), 5);
    m_bpButtonAddRow    ->SetBitmapLabel(GlobalResources::getImage(wxT("addFolderPair")));
    m_bpButtonRemoveRow ->SetBitmapLabel(GlobalResources::getImage(wxT("removeFolderPair")));

    m_checkBoxCopyLocked     ->SetValue(globalSettings.copyLockedFiles);
    m_checkBoxTransCopy      ->SetValue(globalSettings.transactionalFileCopy);
    m_checkBoxCopyPermissions->SetValue(globalSettings.copyFilePermissions);

#ifdef FFS_WIN
    m_checkBoxCopyPermissions->SetLabel(_("Copy NTFS permissions"));
#else
    m_checkBoxCopyLocked->Hide();
    m_staticTextCopyLocked->Hide();
#endif

    set(globalSettings.gui.externelApplications);

    const wxString toolTip = wxString(_("Integrate external applications into context menu. The following macros are available:")) + wxT("\n\n") +
                             wxT("%name   \t") + _("- full file or directory name") + wxT("\n") +
                             wxT("%dir        \t") + _("- directory part only") + wxT("\n") +
                             wxT("%nameCo \t") + _("- Other side's counterpart to %name") + wxT("\n") +
                             wxT("%dirCo   \t") + _("- Other side's counterpart to %dir");

    m_gridCustomCommand->GetGridWindow()->SetToolTip(toolTip);
    m_gridCustomCommand->GetGridColLabelWindow()->SetToolTip(toolTip);
    m_gridCustomCommand->SetMargins(0, 0);

    m_buttonOkay->SetFocus();
    Fit();

    //automatically fit column width to match totl grid width
    Connect(wxEVT_SIZE, wxSizeEventHandler(GlobalSettingsDlg::OnResize), nullptr, this);
    wxSizeEvent dummy;
    OnResize(dummy);
}


void GlobalSettingsDlg::OnResize(wxSizeEvent& event)
{
    const int widthTotal = m_gridCustomCommand->GetGridWindow()->GetClientSize().GetWidth() - 20;

    if (widthTotal >= 0 && m_gridCustomCommand->GetCols() == 2)
    {
        const int w0 = widthTotal / 2;
        const int w1 = widthTotal - w0;
        m_gridCustomCommand->SetColumnWidth(0, w0);
        m_gridCustomCommand->SetColumnWidth(1, w1);

        m_gridCustomCommand->Refresh(); //required on Ubuntu
    }

    event.Skip();
}


void GlobalSettingsDlg::OnOkay(wxCommandEvent& event)
{
    //write global settings only when okay-button is pressed!
    settings.copyLockedFiles       = m_checkBoxCopyLocked->GetValue();
    settings.transactionalFileCopy = m_checkBoxTransCopy->GetValue();
    settings.copyFilePermissions   = m_checkBoxCopyPermissions->GetValue();
    settings.gui.externelApplications = getExtApp();

    EndModal(ReturnSmallDlg::BUTTON_OKAY);
}


void GlobalSettingsDlg::OnResetDialogs(wxCommandEvent& event)
{
    if (showQuestionDlg(ReturnQuestionDlg::BUTTON_YES | ReturnQuestionDlg::BUTTON_CANCEL,
                        _("Make hidden dialogs and warning messages visible again?")) == ReturnQuestionDlg::BUTTON_YES)
        settings.optDialogs.resetDialogs();
}


void GlobalSettingsDlg::OnDefault(wxCommandEvent& event)
{
    xmlAccess::XmlGlobalSettings defaultCfg;

    m_checkBoxCopyLocked     ->SetValue(defaultCfg.copyLockedFiles);
    m_checkBoxTransCopy      ->SetValue(defaultCfg.transactionalFileCopy);
    m_checkBoxCopyPermissions->SetValue(defaultCfg.copyFilePermissions);
    set(defaultCfg.gui.externelApplications);
}


void GlobalSettingsDlg::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}


void GlobalSettingsDlg::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}


void GlobalSettingsDlg::set(const xmlAccess::ExternalApps& extApp)
{
    auto extAppTmp = extApp;
    vector_remove_if(extAppTmp, [](decltype(extAppTmp[0])& entry) { return entry.first.empty() && entry.second.empty(); });

    extAppTmp.resize(extAppTmp.size() + 1); //append empty row to facilitate insertions

    const int rowCount = m_gridCustomCommand->GetNumberRows();
    if (rowCount > 0)
        m_gridCustomCommand->DeleteRows(0, rowCount);

    m_gridCustomCommand->AppendRows(static_cast<int>(extAppTmp.size()));
    for (auto iter = extAppTmp.begin(); iter != extAppTmp.end(); ++iter)
    {
        const int row = iter - extAppTmp.begin();
        m_gridCustomCommand->SetCellValue(row, 0, iter->first);  //description
        m_gridCustomCommand->SetCellValue(row, 1, iter->second); //commandline
    }
    //Fit();
}


xmlAccess::ExternalApps GlobalSettingsDlg::getExtApp()
{
    xmlAccess::ExternalApps output;
    for (int i = 0; i < m_gridCustomCommand->GetNumberRows(); ++i)
    {
        auto entry = std::make_pair(m_gridCustomCommand->GetCellValue(i, 0),  //description
                                    m_gridCustomCommand->GetCellValue(i, 1)); //commandline
        if (!entry.first.empty() || !entry.second.empty())
            output.push_back(entry);
    }
    return output;
}


void GlobalSettingsDlg::OnAddRow(wxCommandEvent& event)
{
    wxWindowUpdateLocker dummy(this); //avoid display distortion

    const int selectedRow = m_gridCustomCommand->GetGridCursorRow();
    if (0 <= selectedRow && selectedRow < m_gridCustomCommand->GetNumberRows())
        m_gridCustomCommand->InsertRows(selectedRow);
    else
        m_gridCustomCommand->AppendRows();
    //Fit();
}


void GlobalSettingsDlg::OnRemoveRow(wxCommandEvent& event)
{
    if (m_gridCustomCommand->GetNumberRows() > 0)
    {
        wxWindowUpdateLocker dummy(this); //avoid display distortion

        const int selectedRow = m_gridCustomCommand->GetGridCursorRow();
        if (0 <= selectedRow && selectedRow < m_gridCustomCommand->GetNumberRows())
            m_gridCustomCommand->DeleteRows(selectedRow);
        else
            m_gridCustomCommand->DeleteRows(m_gridCustomCommand->GetNumberRows() - 1);
        //Fit();
    }
}


ReturnSmallDlg::ButtonPressed zen::showGlobalSettingsDlg(xmlAccess::XmlGlobalSettings& globalSettings)
{
    GlobalSettingsDlg settingsDlg(nullptr, globalSettings);
    return static_cast<ReturnSmallDlg::ButtonPressed>(settingsDlg.ShowModal());
}
//########################################################################################


class SelectTimespanDlg : public SelectTimespanDlgGenerated
{
public:
    SelectTimespanDlg(wxWindow* parent, Int64& timeFrom, Int64& timeTo);

private:
    void OnOkay(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event) { EndModal(0); }
    void OnClose(wxCloseEvent& event) { EndModal(0); }

    virtual void OnChangeSelectionFrom(wxCalendarEvent& event)
    {
        if (m_calendarFrom->GetDate() > m_calendarTo->GetDate())
            m_calendarTo->SetDate(m_calendarFrom->GetDate());
    }
    virtual void OnChangeSelectionTo(wxCalendarEvent& event)
    {
        if (m_calendarFrom->GetDate() > m_calendarTo->GetDate())
            m_calendarFrom->SetDate(m_calendarTo->GetDate());
    }

    Int64& timeFrom_;
    Int64& timeTo_;
};


wxDateTime utcToLocalDateTime(time_t utcTime)
{
    //wxDateTime models local(!) time (in contrast to what documentation says), but this constructor takes time_t UTC
    return wxDateTime(utcTime);
}

time_t localDateTimeToUtc(const wxDateTime& localTime)
{
    return localTime.GetTicks();
}


SelectTimespanDlg::SelectTimespanDlg(wxWindow* parent, Int64& timeFrom, Int64& timeTo) :
    SelectTimespanDlgGenerated(parent),
    timeFrom_(timeFrom),
    timeTo_(timeTo)
{
#ifdef FFS_WIN
    new zen::MouseMoveWindow(*this); //allow moving main dialog by clicking (nearly) anywhere...; ownership passed to "this"
#endif

    long style = wxCAL_SHOW_HOLIDAYS | wxCAL_SHOW_SURROUNDING_WEEKS;

#ifdef FFS_WIN
    DWORD firstDayOfWeek = 0;
    if (::GetLocaleInfo(LOCALE_USER_DEFAULT,                   //__in   LCID Locale,
                        LOCALE_IFIRSTDAYOFWEEK |               // first day of week specifier, 0-6, 0=Monday, 6=Sunday
                        LOCALE_RETURN_NUMBER,                  //__in   LCTYPE LCType,
                        reinterpret_cast<LPTSTR>(&firstDayOfWeek),      //__out  LPTSTR lpLCData,
                        sizeof(firstDayOfWeek) / sizeof(TCHAR)) != 0 && //__in   int cchData
        firstDayOfWeek == 6)
        style |= wxCAL_SUNDAY_FIRST;
    else //default
#endif
        style |= wxCAL_MONDAY_FIRST;

    m_calendarFrom->SetWindowStyleFlag(style);
    m_calendarTo  ->SetWindowStyleFlag(style);

    //set default values
    if (timeTo_ == 0)
        timeTo_ = wxGetUTCTime(); //
    if (timeFrom_ == 0)
        timeFrom_ = timeTo_ - 7 * 24 * 3600; //default time span: one week from "now"

    m_calendarFrom->SetDate(utcToLocalDateTime(to<time_t>(timeFrom_)));
    m_calendarTo  ->SetDate(utcToLocalDateTime(to<time_t>(timeTo_)));

    m_buttonOkay->SetFocus();

    //wxDatePickerCtrl::BestSize() does not respect year field and trims it, both wxMSW/wxGTK - why isn't there anybody testing this wxWidgets stuff???
    wxSize minSz = m_calendarFrom->GetBestSize();
    minSz.x += 30;
    m_calendarFrom->SetMinSize(minSz);
    m_calendarTo  ->SetMinSize(minSz);

    Fit();
}


void SelectTimespanDlg::OnOkay(wxCommandEvent& event)
{
    wxDateTime from = m_calendarFrom->GetDate();
    wxDateTime to   = m_calendarTo  ->GetDate();

    //align to full days
    from.ResetTime();
    to += wxTimeSpan::Day();
    to.ResetTime(); //reset local(!) time
    to -= wxTimeSpan::Second(); //go back to end of previous day

    timeFrom_ = localDateTimeToUtc(from);
    timeTo_   = localDateTimeToUtc(to);

    /*
    {
        time_t current = zen::to<time_t>(timeFrom_);
        struct tm* tdfewst = ::localtime(&current);
        int budfk = 3;
    }
    {
        time_t current = zen::to<time_t>(timeTo_);
        struct tm* tdfewst = ::localtime(&current);
        int budfk = 3;
    }
    */

    EndModal(ReturnSmallDlg::BUTTON_OKAY);
}


ReturnSmallDlg::ButtonPressed zen::showSelectTimespanDlg(Int64& timeFrom, Int64& timeTo)
{
    SelectTimespanDlg timeSpanDlg(nullptr, timeFrom, timeTo);
    return static_cast<ReturnSmallDlg::ButtonPressed>(timeSpanDlg.ShowModal());
}
