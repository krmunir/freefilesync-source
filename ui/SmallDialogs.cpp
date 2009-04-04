#include "smallDialogs.h"
#include "../library/globalFunctions.h"
#include "../library/resources.h"
#include "../algorithm.h"
#include <wx/msgdlg.h>
#include "../library/customGrid.h"
#include "../library/customButton.h"

using namespace FreeFileSync;


AboutDlg::AboutDlg(wxWindow* window) : AboutDlgGenerated(window)
{
    m_bitmap9->SetBitmap(*globalResource.bitmapWebsite);
    m_bitmap10->SetBitmap(*globalResource.bitmapEmail);
    m_bitmap11->SetBitmap(*globalResource.bitmapLogo);
    m_bitmap13->SetBitmap(*globalResource.bitmapGPL);

    //build information
    wxString build = wxString(wxT("(")) + _("Build:") + wxT(" ") + __TDATE__;
#if wxUSE_UNICODE
    build+= wxT(" - Unicode");
#else
    build+= wxT(" - ANSI");
#endif //wxUSE_UNICODE
    build+= + wxT(")");
    m_build->SetLabel(build);

    m_animationControl1->SetAnimation(*globalResource.animationMoney);
    m_animationControl1->Play(); //Note: The animation is created hidden(!) to not disturb constraint based window creation;
    m_animationControl1->Show(); //

    m_button8->SetFocus();
    Fit();
}


AboutDlg::~AboutDlg() {}


void AboutDlg::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}


void AboutDlg::OnOK(wxCommandEvent& event)
{
    EndModal(0);
}
//########################################################################################


HelpDlg::HelpDlg(wxWindow* window) : HelpDlgGenerated(window)
{
    m_notebook1->SetFocus();

    m_bitmap25->SetBitmap(*globalResource.bitmapHelp);

    //populate decision trees: "compare by date"
    wxTreeItemId treeRoot      = m_treeCtrl1->AddRoot(_("DECISION TREE"));
    wxTreeItemId treeBothSides = m_treeCtrl1->AppendItem(treeRoot, _("file exists on both sides"));
    wxTreeItemId treeOneSide   = m_treeCtrl1->AppendItem(treeRoot, _("on one side only"));

    m_treeCtrl1->AppendItem(treeOneSide, _("- left"));
    m_treeCtrl1->AppendItem(treeOneSide, _("- right"));

    m_treeCtrl1->AppendItem(treeBothSides, _("- equal"));
    wxTreeItemId treeDifferent = m_treeCtrl1->AppendItem(treeBothSides, _("different"));

    m_treeCtrl1->AppendItem(treeDifferent, _("- left newer"));
    m_treeCtrl1->AppendItem(treeDifferent, _("- right newer"));
    m_treeCtrl1->AppendItem(treeDifferent, _("- same date (different size)"));

    m_treeCtrl1->ExpandAll();

    //populate decision trees: "compare by content"
    wxTreeItemId tree2Root      = m_treeCtrl2->AddRoot(_("DECISION TREE"));
    wxTreeItemId tree2BothSides = m_treeCtrl2->AppendItem(tree2Root, _("file exists on both sides"));
    wxTreeItemId tree2OneSide   = m_treeCtrl2->AppendItem(tree2Root, _("on one side only"));

    m_treeCtrl2->AppendItem(tree2OneSide, _("- left"));
    m_treeCtrl2->AppendItem(tree2OneSide, _("- right"));

    m_treeCtrl2->AppendItem(tree2BothSides, _("- equal"));
    m_treeCtrl2->AppendItem(tree2BothSides, _("- different"));

    m_treeCtrl2->ExpandAll();
}


HelpDlg::~HelpDlg() {}


void HelpDlg::OnClose(wxCloseEvent& event)
{
    Destroy();
}


void HelpDlg::OnOK(wxCommandEvent& event)
{
    Destroy();
}
//########################################################################################


FilterDlg::FilterDlg(wxWindow* window, wxString& filterIncl, wxString& filterExcl) :
        FilterDlgGenerated(window),
        includeFilter(filterIncl),
        excludeFilter(filterExcl)
{
    m_bitmap8->SetBitmap(*globalResource.bitmapInclude);
    m_bitmap9->SetBitmap(*globalResource.bitmapExclude);
    m_bitmap26->SetBitmap(*globalResource.bitmapFilter);
    m_bpButtonHelp->SetBitmapLabel(*globalResource.bitmapHelp);

    m_textCtrlInclude->SetValue(includeFilter);
    m_textCtrlExclude->SetValue(excludeFilter);

    m_panel13->Hide();
    m_button10->SetFocus();

    Fit();
}

FilterDlg::~FilterDlg() {}


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
    m_textCtrlInclude->SetValue(wxT("*"));
    m_textCtrlExclude->SetValue(wxEmptyString);

    //changes to mainDialog are only committed when the OK button is pressed
    event.Skip();
}


void FilterDlg::OnOK(wxCommandEvent& event)
{
    //only if user presses ApplyFilter, he wants the changes to be committed
    includeFilter = m_textCtrlInclude->GetValue();
    excludeFilter = m_textCtrlExclude->GetValue();

    //when leaving dialog: filter and redraw grid, if filter is active
    EndModal(BUTTON_OKAY);
}


void FilterDlg::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}


void FilterDlg::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}


//########################################################################################
DeleteDialog::DeleteDialog(wxWindow* main,
                           const FileCompareResult& grid,
                           const std::set<int>& rowsOnLeft,
                           const std::set<int>& rowsOnRight,
                           bool& deleteOnBothSides,
                           bool& useRecycleBin) :
        DeleteDlgGenerated(main),
        mainGrid(grid),
        rowsToDeleteOnLeft(rowsOnLeft),
        rowsToDeleteOnRight(rowsOnRight),
        m_deleteOnBothSides(deleteOnBothSides),
        m_useRecycleBin(useRecycleBin)
{
    m_checkBoxDeleteBothSides->SetValue(deleteOnBothSides);
    m_checkBoxUseRecycler->SetValue(useRecycleBin);
    updateTexts();

    m_buttonOK->SetFocus();
}


void DeleteDialog::updateTexts()
{
    wxString headerText;
    if (m_checkBoxUseRecycler->GetValue())
    {
        headerText = _("Do you really want to move the following objects(s) to the Recycle Bin?");
        m_bitmap12->SetBitmap(*globalResource.bitmapRecycler);
    }
    else
    {
        headerText = _("Do you really want to delete the following objects(s)?");
        m_bitmap12->SetBitmap(*globalResource.bitmapDeleteFile);
    }
    m_staticTextHeader->SetLabel(headerText);

    wxString filesToDelete = FreeFileSync::deleteFromGridAndHDPreview(mainGrid,
                             rowsToDeleteOnLeft,
                             rowsToDeleteOnRight,
                             m_checkBoxDeleteBothSides->GetValue());
    m_textCtrlMessage->SetValue(filesToDelete);

    Layout();
}


void DeleteDialog::OnOK(wxCommandEvent& event)
{
    EndModal(BUTTON_OKAY);
}

void DeleteDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(BUTTON_CANCEL);
}

void DeleteDialog::OnClose(wxCloseEvent& event)
{
    EndModal(BUTTON_CANCEL);
}

void DeleteDialog::OnDelOnBothSides(wxCommandEvent& event)
{
    m_deleteOnBothSides = m_checkBoxDeleteBothSides->GetValue();
    updateTexts();
}

void DeleteDialog::OnUseRecycler(wxCommandEvent& event)
{
    if (m_checkBoxUseRecycler->GetValue())
    {
        if (!FreeFileSync::recycleBinExists())
        {
            wxMessageBox(_("It was not possible to initialize the Recycle Bin!\n\nIt's likely that you are not using Windows.\nIf you want this feature included, please contact the author. :)"), _("Error") , wxOK | wxICON_ERROR);
            m_checkBoxUseRecycler->SetValue(false);
        }
    }

    m_useRecycleBin = m_checkBoxUseRecycler->GetValue();
    updateTexts();
}
//########################################################################################


ErrorDlg::ErrorDlg(wxWindow* parentWindow, const int activeButtons, const wxString messageText, bool& ignoreNextErrors) :
        ErrorDlgGenerated(parentWindow),
        ignoreErrors(ignoreNextErrors)
{
    m_bitmap10->SetBitmap(*globalResource.bitmapError);
    m_textCtrl8->SetValue(messageText);

    if (~activeButtons & BUTTON_IGNORE)
    {
        m_buttonIgnore->Hide();
        m_checkBoxIgnoreErrors->Hide();
    }

    if (~activeButtons & BUTTON_RETRY)
        m_buttonRetry->Hide();

    if (~activeButtons & BUTTON_ABORT)
        m_buttonAbort->Hide();

    //set button focus precedence
    if (activeButtons & BUTTON_RETRY)
        m_buttonRetry->SetFocus();
    else if (activeButtons & BUTTON_IGNORE)
        m_buttonIgnore->SetFocus();
    else if (activeButtons & BUTTON_ABORT)
        m_buttonAbort->SetFocus();
}

ErrorDlg::~ErrorDlg() {}


void ErrorDlg::OnClose(wxCloseEvent& event)
{
    ignoreErrors = m_checkBoxIgnoreErrors->GetValue();
    EndModal(BUTTON_ABORT);
}


void ErrorDlg::OnIgnore(wxCommandEvent& event)
{
    ignoreErrors = m_checkBoxIgnoreErrors->GetValue();
    EndModal(BUTTON_IGNORE);
}


void ErrorDlg::OnRetry(wxCommandEvent& event)
{
    ignoreErrors = m_checkBoxIgnoreErrors->GetValue();
    EndModal(BUTTON_RETRY);
}


void ErrorDlg::OnAbort(wxCommandEvent& event)
{
    ignoreErrors = m_checkBoxIgnoreErrors->GetValue();
    EndModal(BUTTON_ABORT);
}
//########################################################################################


WarningDlg::WarningDlg(wxWindow* parentWindow,  int activeButtons, const wxString messageText, bool& dontShowDlgAgain) :
        WarningDlgGenerated(parentWindow),
        dontShowAgain(dontShowDlgAgain)
{
    m_bitmap10->SetBitmap(*globalResource.bitmapWarning);
    m_textCtrl8->SetValue(messageText);

    if (~activeButtons & BUTTON_IGNORE)
    {
        m_buttonIgnore->Hide();
        m_checkBoxDontShowAgain->Hide();
    }

    if (~activeButtons & BUTTON_ABORT)
        m_buttonAbort->Hide();

    //set button focus precedence
    else if (activeButtons & BUTTON_IGNORE)
        m_buttonIgnore->SetFocus();
    else if (activeButtons & BUTTON_ABORT)
        m_buttonAbort->SetFocus();
}

WarningDlg::~WarningDlg() {}


void WarningDlg::OnClose(wxCloseEvent& event)
{
    dontShowAgain = m_checkBoxDontShowAgain->GetValue();
    EndModal(BUTTON_ABORT);
}


void WarningDlg::OnIgnore(wxCommandEvent& event)
{
    dontShowAgain = m_checkBoxDontShowAgain->GetValue();
    EndModal(BUTTON_IGNORE);
}


void WarningDlg::OnAbort(wxCommandEvent& event)
{
    dontShowAgain = m_checkBoxDontShowAgain->GetValue();
    EndModal(BUTTON_ABORT);
}

//########################################################################################
CustomizeColsDlg::CustomizeColsDlg(wxWindow* window, xmlAccess::ColumnAttributes& attr) :
        CustomizeColsDlgGenerated(window),
        output(attr)
{
    m_bpButton29->SetBitmapLabel(*globalResource.bitmapMoveUp);
    m_bpButton30->SetBitmapLabel(*globalResource.bitmapMoveDown);

    xmlAccess::ColumnAttributes columnSettings = attr;

    sort(columnSettings.begin(), columnSettings.end(), xmlAccess::sortByPositionOnly);

    for (xmlAccess::ColumnAttributes::const_iterator i = columnSettings.begin(); i != columnSettings.end(); ++i) //love these iterators!
    {
        m_checkListColumns->Append(CustomGrid::getTypeName(i->type));
        m_checkListColumns->Check(i - columnSettings.begin(), i->visible);
    }

    m_checkListColumns->SetSelection(0);
    Fit();
}


void CustomizeColsDlg::OnOkay(wxCommandEvent& event)
{
    for (int i = 0; i < int(m_checkListColumns->GetCount()); ++i)
    {
        const wxString label = m_checkListColumns->GetString(i);
        for (xmlAccess::ColumnAttributes::iterator j = output.begin(); j != output.end(); ++j)
        {
            if (CustomGrid::getTypeName(j->type) == label) //not nice but short and no performance issue
            {
                j->position = i;
                j->visible  = m_checkListColumns->IsChecked(i);;
                break;
            }
        }
    }

    EndModal(BUTTON_OKAY);
}


void CustomizeColsDlg::OnDefault(wxCommandEvent& event)
{
    xmlAccess::ColumnAttributes defaultColumnAttr = CustomGrid::getDefaultColumnAttributes();

    m_checkListColumns->Clear();
    for (xmlAccess::ColumnAttributes::const_iterator i = defaultColumnAttr.begin(); i != defaultColumnAttr.end(); ++i)
    {
        m_checkListColumns->Append(CustomGrid::getTypeName(i->type));
        m_checkListColumns->Check(i - defaultColumnAttr.begin(), i->visible);
    }
}


void CustomizeColsDlg::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}


void CustomizeColsDlg::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}


void CustomizeColsDlg::OnMoveUp(wxCommandEvent& event)
{
    const int pos = m_checkListColumns->GetSelection();
    if (1 <= pos && pos < int(m_checkListColumns->GetCount()))
    {
        const bool checked    = m_checkListColumns->IsChecked(pos);
        const wxString label  = m_checkListColumns->GetString(pos);

        m_checkListColumns->SetString(pos, m_checkListColumns->GetString(pos - 1));
        m_checkListColumns->Check(pos, m_checkListColumns->IsChecked(pos - 1));
        m_checkListColumns->SetString(pos - 1, label);
        m_checkListColumns->Check(pos - 1, checked);
        m_checkListColumns->Select(pos - 1);
    }
}


void CustomizeColsDlg::OnMoveDown(wxCommandEvent& event)
{
    const int pos = m_checkListColumns->GetSelection();
    if (0 <= pos && pos < int(m_checkListColumns->GetCount()) - 1)
    {
        const bool checked    = m_checkListColumns->IsChecked(pos);
        const wxString label  = m_checkListColumns->GetString(pos);

        m_checkListColumns->SetString(pos, m_checkListColumns->GetString(pos + 1));
        m_checkListColumns->Check(pos, m_checkListColumns->IsChecked(pos + 1));
        m_checkListColumns->SetString(pos + 1, label);
        m_checkListColumns->Check(pos + 1, checked);
        m_checkListColumns->Select(pos + 1);
    }
}

//########################################################################################
GlobalSettingsDlg::GlobalSettingsDlg(wxWindow* window, xmlAccess::XmlGlobalSettings& globalSettings) :
        GlobalSettingsDlgGenerated(window),
        settings(globalSettings)
{
    m_bitmapSettings->SetBitmap(*globalResource.bitmapSettings);
    m_buttonResetWarnings->setBitmapFront(*globalResource.bitmapWarningSmall, 5);

    m_spinCtrlFileTimeTolerance->SetValue(globalSettings.shared.fileTimeTolerance);
    m_textCtrlFileManager->SetValue(globalSettings.gui.commandLineFileManager);

    Fit();
}


void GlobalSettingsDlg::OnOkay(wxCommandEvent& event)
{
    //write global settings only when okay-button is pressed!

    settings.shared.fileTimeTolerance = m_spinCtrlFileTimeTolerance->GetValue();
    settings.gui.commandLineFileManager = m_textCtrlFileManager->GetValue();

    EndModal(BUTTON_OKAY);
}


void GlobalSettingsDlg::OnResetWarnings(wxCommandEvent& event)
{
    wxMessageDialog* messageDlg = new wxMessageDialog(this, _("Reset all warning messages?"), _("Warning") , wxOK | wxCANCEL);
    if (messageDlg->ShowModal() == wxID_OK)
        settings.shared.resetWarnings();
}


void GlobalSettingsDlg::OnDefault(wxCommandEvent& event)
{
    m_spinCtrlFileTimeTolerance->SetValue(2);
#ifdef FFS_WIN
    m_textCtrlFileManager->SetValue(wxT("explorer /select, %name"));
#elif defined FFS_LINUX
    m_textCtrlFileManager->SetValue(wxT("konqueror \"%path\""));
#endif
}


void GlobalSettingsDlg::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}


void GlobalSettingsDlg::OnClose(wxCloseEvent& event)
{
    EndModal(0);
}


//########################################################################################
/*
class for calculation of remaining time:
----------------------------------------
"filesize |-> time" is an affine linear function f(x) = z_1 + z_2 x

For given n measurements, sizes x_0, ..., x_n and times f_0, ..., f_n, the function f (as a polynom of degree 1) can be lineary approximated by

z_1 = (r - s * q / p) / ((n + 1) - s * s / p)
z_2 = (q - s * z_1) / p = (r - (n + 1) z_1) / s

with
p := x_0^2 + ... + x_n^2
q := f_0 x_0 + ... + f_n x_n
r := f_0 + ... + f_n
s := x_0 + ... + x_n

=> the time to process N files with amount of data D is:    N * z_1 + D * z_2

Problem:
--------
Times f_0, ..., f_n can be very small so that precision of the PC clock is poor.
=> Times have to be accumulated to enhance precision:
Copying of m files with sizes x_i and times f_i (i = 1, ..., m) takes sum_i f(x_i) := m * z_1 + z_2 * sum x_i = sum f_i
With X defined as the accumulated sizes and F the accumulated times this gives: (in theory...)
m * z_1 + z_2 * X = F   <=>
z_1 + z_2 * X / m = F / m

=> we optain a new (artificial) measurement with size X / m and time F / m to be used in the linear approximation above


RemainingTime::RemainingTime() : n(0), m(0), X(0), F(0), p(0), q(0), r(0), s(0), z_1(0), z_2(0), lastExec(0) {}

RemainingTime::~RemainingTime()
{
    ofstream output("test.txt");
    for (unsigned i = 0; i < x.size(); ++i)
    {
        output<<x[i]<<" "<<f[i]<<'\n';
    }
    output<<'\n'<<z_1<<" "<<z_2<<'\n';
    output.close();
}


wxLongLong RemainingTime::getRemainingTime(double processedDataSinceLastCall, int remainingFiles, double remainingData) //returns the remaining time in seconds
{
    wxLongLong newExec = wxGetLocalTimeMillis();

    if (lastExec != 0)
    {
        X+= processedDataSinceLastCall;
        F = (newExec - lastExec).ToDouble();
        ++m;

        if (F > 1000)  //add new measurement only if F is accumulated to a certain degree
        {
            lastExec = newExec;
            ++n;

            double x_i = X / m;
            double f_i = F / m;
            X = 0;
            F = 0;
            m = 0;

            x.push_back(x_i);
            f.push_back(f_i);

            p+= x_i * x_i;
            q+= f_i * x_i;
            r+= f_i;
            s+= x_i;

            if (p != 0)
            {
                double tmp = (n - s * s / p);
                if (tmp != 0 && s != 0)
                {   //recalculate coefficients for affine-linear function
                    z_1 = (r - s * q / p) / tmp;
                    z_2 = (r - n * z_1) / s;    //not (n + 1) here, since n already is the number of measurements
                }
            }
        }

        return int(remainingFiles * z_1 + remainingData * z_2);
    }
    //else
    lastExec = newExec;
    return 0;
}*/

//########################################################################################

SyncStatus::SyncStatus(StatusHandler* updater, wxWindow* parentWindow) :
        SyncStatusDlgGenerated(parentWindow),
        currentStatusHandler(updater),
        windowToDis(parentWindow),
        currentProcessIsRunning(true),
        totalData(0),
        currentData(0),
        scalingFactor(0),
        currentObjects(0),
        totalObjects(0),
        processPaused(false),
        currentStatus(SyncStatus::ABORTED)
{
    m_animationControl1->SetAnimation(*globalResource.animationSync);
    m_animationControl1->Play();

    //initialize gauge
    m_gauge1->SetRange(50000);
    m_gauge1->SetValue(0);

    m_buttonAbort->SetFocus();

    if (windowToDis)    //disable (main) window while this status dialog is shown
        windowToDis->Disable();

    timeElapsed.Start(); //measure total time
}


SyncStatus::~SyncStatus()
{
    if (windowToDis)
    {
        windowToDis->Enable();
        windowToDis->Raise();
        windowToDis->SetFocus();
    }
}


void SyncStatus::resetGauge(int totalObjectsToProcess, double totalDataToProcess)
{
    currentData = 0;
    totalData = totalDataToProcess;

    currentObjects = 0;
    totalObjects   = totalObjectsToProcess;

    if (totalData != 0)
        scalingFactor = 50000 / totalData; //let's normalize to 50000
    else
        scalingFactor = 0;
}


void SyncStatus::incProgressIndicator_NoUpdate(int objectsProcessed, double dataProcessed)
{
    currentData+=    dataProcessed;
    currentObjects+= objectsProcessed;
}


void SyncStatus::setStatusText_NoUpdate(const Zstring& text)
{
    currentStatusText = text;
}


void SyncStatus::updateStatusDialogNow()
{
    bool screenChanged = false; //avoid screen flicker by calling layout() only if necessary

    //progress indicator
    m_gauge1->SetValue(globalFunctions::round(currentData * scalingFactor));

    //status text
    if (m_textCtrlInfo->GetValue() != wxString(currentStatusText.c_str()) && (screenChanged = true)) //avoid screen flicker
        m_textCtrlInfo->SetValue(currentStatusText.c_str());

    //remaining objects
    const wxString remainingObjTmp = globalFunctions::numberToWxString(totalObjects - currentObjects);
    if (m_staticTextRemainingObj->GetLabel() != remainingObjTmp && (screenChanged = true)) //avoid screen flicker
        m_staticTextRemainingObj->SetLabel(remainingObjTmp);

    //remaining bytes left for copy
    const wxString remainingBytesTmp = FreeFileSync::formatFilesizeToShortString(totalData - currentData);
    if (m_staticTextDataRemaining->GetLabel() != remainingBytesTmp && (screenChanged = true)) //avoid screen flicker
        m_staticTextDataRemaining->SetLabel(remainingBytesTmp);

    //time elapsed
    const wxString timeElapsedTmp = (wxTimeSpan::Milliseconds(timeElapsed.Time())).Format();
    if (m_staticTextTimeElapsed->GetLabel() != timeElapsedTmp && (screenChanged = true)) //avoid screen flicker
        m_staticTextTimeElapsed->SetLabel(timeElapsedTmp);

    //do the ui update
    if (screenChanged)
    {
        bSizer28->Layout();
        bSizer31->Layout();
    }
    updateUiNow();

    //support for pause button
    while (processPaused && currentProcessIsRunning)
    {
        wxMilliSleep(UI_UPDATE_INTERVAL);
        updateUiNow();
    }
}


void SyncStatus::setCurrentStatus(SyncStatusID id)
{
    switch (id)
    {
    case ABORTED:
        m_bitmapStatus->SetBitmap(*globalResource.bitmapStatusError);
        m_staticTextStatus->SetLabel(_("Aborted"));
        break;

    case FINISHED_WITH_SUCCESS:
        m_bitmapStatus->SetBitmap(*globalResource.bitmapStatusSuccess);
        m_staticTextStatus->SetLabel(_("Completed"));
        break;

    case FINISHED_WITH_ERROR:
        m_bitmapStatus->SetBitmap(*globalResource.bitmapStatusWarning);
        m_staticTextStatus->SetLabel(_("Completed"));
        break;

    case PAUSE:
        m_bitmapStatus->SetBitmap(*globalResource.bitmapStatusPause);
        m_staticTextStatus->SetLabel(_("Pause"));
        break;

    case SCANNING:
        m_bitmapStatus->SetBitmap(*globalResource.bitmapStatusComparing);
        m_staticTextStatus->SetLabel(_("Scanning..."));
        break;

    case COMPARING:
        m_bitmapStatus->SetBitmap(*globalResource.bitmapStatusComparing);
        m_staticTextStatus->SetLabel(_("Comparing..."));
        break;

    case SYNCHRONIZING:
        m_bitmapStatus->SetBitmap(*globalResource.bitmapStatusSyncing);
        m_staticTextStatus->SetLabel(_("Synchronizing..."));
        break;
    }

    currentStatus = id;
    Layout();
}


void SyncStatus::processHasFinished(SyncStatusID id) //essential to call this in StatusHandler derived class destructor
{                                                    //at the LATEST(!) to prevent access to currentStatusHandler
    currentProcessIsRunning = false; //enable okay and close events; may be set ONLY in this method

    setCurrentStatus(id);

    m_buttonAbort->Disable();
    m_buttonAbort->Hide();
    m_buttonPause->Disable();
    m_buttonPause->Hide();
    m_buttonOK->Show();
    m_buttonOK->SetFocus();

    m_animationControl1->Stop();
    //m_animationControl1->SetInactiveBitmap(*globalResource.bitmapFinished);
    m_animationControl1->Hide();

    updateStatusDialogNow(); //keep this sequence to avoid display distortion, if e.g. only 1 item is sync'ed
    Layout();                //
}


void SyncStatus::OnOkay(wxCommandEvent& event)
{
    if (!currentProcessIsRunning) Destroy();
}


void SyncStatus::OnPause(wxCommandEvent& event)
{
    static SyncStatusID previousStatus = SyncStatus::ABORTED;

    if (processPaused)
    {
        setCurrentStatus(previousStatus);
        processPaused = false;
        m_buttonPause->SetLabel(_("Pause"));
        m_animationControl1->Play();
        timeElapsed.Resume();
    }
    else
    {
        previousStatus = currentStatus; //save current status

        setCurrentStatus(SyncStatus::PAUSE);
        processPaused = true;
        m_buttonPause->SetLabel(_("Continue"));
        m_animationControl1->Stop();
        timeElapsed.Pause();
    }
}


void SyncStatus::OnAbort(wxCommandEvent& event)
{
    processPaused = false;
    if (currentProcessIsRunning)
    {
        m_buttonAbort->Disable();
        m_buttonAbort->Hide();
        m_buttonPause->Disable();
        m_buttonPause->Hide();

        setStatusText_NoUpdate(_("Abort requested: Waiting for current operation to finish..."));
        //no Layout() or UI-update here to avoid cascaded Yield()-call

        currentStatusHandler->requestAbortion();
    }
}


void SyncStatus::OnClose(wxCloseEvent& event)
{
    processPaused = false;
    if (currentProcessIsRunning)
        currentStatusHandler->requestAbortion();
    else
        Destroy();
}
//########################################################################################


CompareStatus::CompareStatus(wxWindow* parentWindow) :
        CompareStatusGenerated(parentWindow),
        scannedObjects(0),
        scalingFactorCmp(0),
        totalCmpData(0),
        processedCmpData(0),
        totalCmpObjects(0),
        processedCmpObjects(0)
        /*timeRemaining(0),
        timeRemainingTimeStamp(0)*/
{
    init();
}


CompareStatus::~CompareStatus() {}


void CompareStatus::init()
{
    //initialize gauge
    m_gauge2->SetRange(50000);
    m_gauge2->SetValue(0);

    //initially hide status that's relevant for comparing bytewise only
    bSizer42->Hide(sbSizer13);
    m_gauge2->Hide();
    bSizer42->Layout();

    scannedObjects      = 0;
    scalingFactorCmp    = 0;

    totalCmpData        = 0;
    processedCmpData    = 0;
    totalCmpObjects     = 0;
    processedCmpObjects = 0;

    timeElapsed.Start(); //measure total time

    updateStatusPanelNow();
}


void CompareStatus::switchToCompareBytewise(int totalCmpObjectsToProcess, double totalCmpDataToProcess)
{
    processedCmpData = 0;
    totalCmpData = totalCmpDataToProcess;

    processedCmpObjects = 0;
    totalCmpObjects   = totalCmpObjectsToProcess;

    if (totalCmpData != 0)
        scalingFactorCmp = 50000 / totalCmpData; //let's normalize to 50000
    else
        scalingFactorCmp = 0;

    //show status for comparing bytewise
    bSizer42->Show(sbSizer13);
    m_gauge2->Show();
    bSizer42->Layout();
}


void CompareStatus::incScannedObjects_NoUpdate(int number)
{
    scannedObjects+= number;
}


void CompareStatus::incProcessedCmpData_NoUpdate(int objectsProcessed, double dataProcessed)
{
    processedCmpData+=    dataProcessed;
    processedCmpObjects+= objectsProcessed;

    /*    timeRemaining = calcTimeLeft.getRemainingTime(dataProcessed, totalCmpObjects - processedCmpObjects, totalCmpData - processedCmpData);
        timeRemainingTimeStamp = wxGetLocalTimeMillis();*/
}


void CompareStatus::setStatusText_NoUpdate(const Zstring& text)
{
    currentStatusText = text;
}


void CompareStatus::updateStatusPanelNow()
{
    bool screenChanged = false; //avoid screen flicker by calling layout() only if necessary

    //remove linebreaks from currentStatusText
    wxString formattedStatusText = currentStatusText.c_str();
    for (wxString::iterator i = formattedStatusText.begin(); i != formattedStatusText.end(); ++i)
        if (*i == wxChar('\n'))
            *i = wxChar(' ');

    //status texts
    if (m_textCtrlFilename->GetValue() != formattedStatusText && (screenChanged = true)) //avoid screen flicker
        m_textCtrlFilename->SetValue(formattedStatusText);

    //nr of scanned objects
    const wxString scannedObjTmp = globalFunctions::numberToWxString(scannedObjects);
    if (m_staticTextScanned->GetLabel() != scannedObjTmp && (screenChanged = true)) //avoid screen flicker
        m_staticTextScanned->SetLabel(scannedObjTmp);

    //progress indicator for "compare file content"
    m_gauge2->SetValue(int(processedCmpData * scalingFactorCmp));

    //remaining files left for file comparison
    const wxString filesToCompareTmp = globalFunctions::numberToWxString(totalCmpObjects - processedCmpObjects);
    if (m_staticTextFilesToCompare->GetLabel() != filesToCompareTmp && (screenChanged = true)) //avoid screen flicker
        m_staticTextFilesToCompare->SetLabel(filesToCompareTmp);

    //remaining bytes left for file comparison
    const wxString remainingBytesTmp = FreeFileSync::formatFilesizeToShortString(totalCmpData - processedCmpData);
    if (m_staticTextDataToCompare->GetLabel() != remainingBytesTmp && (screenChanged = true)) //avoid screen flicker
        m_staticTextDataToCompare->SetLabel(remainingBytesTmp);

    /*
        //remaining time in seconds
        if (timeRemaining != 0)
        {
            int time = ((timeRemaining - (wxGetLocalTimeMillis() - timeRemainingTimeStamp)) / 1000).GetLo();
            m_staticTextRemainingTime->SetLabel(numberToWxString(time) + " s");
        }
    */

    //time elapsed
    const wxString timeElapsedTmp = (wxTimeSpan::Milliseconds(timeElapsed.Time())).Format();
    if (m_staticTextTimeElapsed->GetLabel() != timeElapsedTmp && (screenChanged = true)) //avoid screen flicker
        m_staticTextTimeElapsed->SetLabel(timeElapsedTmp);

    //do the ui update
    if (screenChanged)
        bSizer42->Layout();

    updateUiNow();
}
