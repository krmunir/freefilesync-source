#ifndef SMALLDIALOGS_H_INCLUDED
#define SMALLDIALOGS_H_INCLUDED

#include "../fileHierarchy.h"
#include "../library/processXml.h"
#include "guiGenerated.h"
#include <wx/stopwatch.h>
#include <memory>

class Statistics;
class StatusHandler;

namespace FreeFileSync
{
    class SyncStatistics;
}


class AboutDlg : public AboutDlgGenerated
{
public:
    AboutDlg(wxWindow* window);
    ~AboutDlg() {}

private:
    void OnClose(wxCloseEvent& event);
    void OnOK(wxCommandEvent& event);
};


class HelpDlg : public HelpDlgGenerated
{
public:
    HelpDlg(wxWindow* window);
    ~HelpDlg();

private:
    void OnClose(wxCloseEvent& event);
    void OnOK(wxCommandEvent& event);
};


class FilterDlg : public FilterDlgGenerated
{
public:
    FilterDlg(wxWindow* window, wxString& filterIncl, wxString& filterExcl);
    ~FilterDlg() {}

    enum
    {
        BUTTON_APPLY = 1
    };

private:
    void OnHelp(wxCommandEvent& event);
    void OnDefault(wxCommandEvent& event);
    void OnApply(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    wxString& includeFilter;
    wxString& excludeFilter;
};


class DeleteDialog : public DeleteDlgGenerated
{
public:
    DeleteDialog(wxWindow* main,
                 const std::vector<FreeFileSync::FileSystemObject*>& rowsOnLeft,
                 const std::vector<FreeFileSync::FileSystemObject*>& rowsOnRight,
                 bool& deleteOnBothSides,
                 bool& useRecycleBin,
                 int& totalDeleteCount);

    ~DeleteDialog() {}

    enum
    {
        BUTTON_OKAY = 1,
        BUTTON_CANCEL
    };

private:
    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnDelOnBothSides(wxCommandEvent& event);
    void OnUseRecycler(wxCommandEvent& event);

    void updateTexts();

    const std::vector<FreeFileSync::FileSystemObject*>& rowsToDeleteOnLeft;
    const std::vector<FreeFileSync::FileSystemObject*>& rowsToDeleteOnRight;
    bool& m_deleteOnBothSides;
    bool& m_useRecycleBin;
    int&  totalDelCount;
};


class ErrorDlg : public ErrorDlgGenerated
{
public:
    ErrorDlg(wxWindow* parentWindow, const int activeButtons, const wxString messageText, bool& ignoreNextErrors);
    ~ErrorDlg();

    enum ReturnCodes
    {
        BUTTON_IGNORE = 1,
        BUTTON_RETRY  = 2,
        BUTTON_ABORT  = 4
    };

private:
    void OnClose(wxCloseEvent& event);
    void OnIgnore(wxCommandEvent& event);
    void OnRetry(wxCommandEvent& event);
    void OnAbort(wxCommandEvent& event);

    bool& ignoreErrors;
};


class WarningDlg : public WarningDlgGenerated
{
public:
    WarningDlg(wxWindow* parentWindow, int activeButtons, const wxString messageText, bool& dontShowAgain);
    ~WarningDlg();

    enum Response
    {
        BUTTON_IGNORE  = 1,
        BUTTON_ABORT   = 2
    };

private:
    void OnClose(wxCloseEvent& event);
    void OnIgnore(wxCommandEvent& event);
    void OnResolve(wxCommandEvent& event);
    void OnAbort(wxCommandEvent& event);
    void OnOkay(wxCommandEvent& event);

    bool& dontShowAgain;
};


class QuestionDlg : public QuestionDlgGenerated
{
public:
    QuestionDlg(wxWindow* parentWindow, int activeButtons, const wxString messageText, bool& dontShowAgain);

    enum
    {
        BUTTON_YES    = 1,
        BUTTON_NO     = 2,
        BUTTON_CANCEL = 4
    };

private:
    void OnClose(wxCloseEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnYes(wxCommandEvent& event);
    void OnNo(wxCommandEvent& event);

    bool& dontShowAgain;
};


class CustomizeColsDlg : public CustomizeColsDlgGenerated
{
public:
    CustomizeColsDlg(wxWindow* window, xmlAccess::ColumnAttributes& attr, bool& showFileIcons);

    enum
    {
        BUTTON_OKAY = 10
    };

private:
    void OnOkay(wxCommandEvent& event);
    void OnDefault(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    void OnMoveUp(wxCommandEvent& event);
    void OnMoveDown(wxCommandEvent& event);

    xmlAccess::ColumnAttributes& output;
    bool& m_showFileIcons;
};


class SyncPreviewDlg : public SyncPreviewDlgGenerated
{
public:
    SyncPreviewDlg(wxWindow* parentWindow,
                   const wxString& variantName,
                   const FreeFileSync::SyncStatistics& statistics,
                   bool& dontShowAgain);

    enum
    {
        BUTTON_START  = 1,
        BUTTON_CANCEL = 2
    };

private:
    void OnClose(wxCloseEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnStartSync(wxCommandEvent& event);

    bool& m_dontShowAgain;
};


class CompareCfgDialog : public CmpCfgDlgGenerated
{
public:
    CompareCfgDialog(wxWindow* parentWindow, const wxPoint& position, FreeFileSync::CompareVariant& cmpVar);

    enum
    {
        BUTTON_OKAY = 10
    };

private:
    void OnClose(wxCloseEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnTimeSize(wxCommandEvent& event);
    void OnContent(wxCommandEvent& event);
    void OnShowHelp(wxCommandEvent& event);

    FreeFileSync::CompareVariant& m_cmpVar;
};


class GlobalSettingsDlg : public GlobalSettingsDlgGenerated
{
public:
    GlobalSettingsDlg(wxWindow* window, xmlAccess::XmlGlobalSettings& globalSettings);
    ~GlobalSettingsDlg() {}

    enum
    {
        BUTTON_OKAY = 10
    };

private:
    void OnOkay(wxCommandEvent& event);
    void OnResetDialogs(wxCommandEvent& event);
    void OnDefault(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnAddRow(wxCommandEvent& event);
    void OnRemoveRow(wxCommandEvent& event);

    void set(const xmlAccess::ExternalApps& extApp);
    xmlAccess::ExternalApps getExtApp();

    xmlAccess::XmlGlobalSettings& settings;
};


class CompareStatus : public CompareStatusGenerated
{
public:
    CompareStatus(wxWindow* parentWindow);

    void init(); //initialize all status values

    void switchToCompareBytewise(int totalObjectsToProcess, wxLongLong totalDataToProcess);
    void incScannedObjects_NoUpdate(int number);
    void incProcessedCmpData_NoUpdate(int objectsProcessed, wxLongLong dataProcessed);
    void setStatusText_NoUpdate(const Zstring& text);
    void updateStatusPanelNow();

private:
    //status variables
    unsigned int scannedObjects;
    Zstring currentStatusText;

    wxStopWatch timeElapsed;

    //gauge variables
    int        totalObjects;
    wxLongLong totalData;      //each data element represents one byte for proper progress indicator scaling
    int        currentObjects; //each object represents a file or directory processed
    wxLongLong currentData;
    double     scalingFactor;  //nr of elements has to be normalized to smaller nr. because of range of int limitation

    //remaining time
    std::auto_ptr<Statistics> statistics;
    long lastStatCallSpeed;   //used for calculating intervals between statistics update
    long lastStatCallRemTime; //
};


class SyncStatus : public SyncStatusDlgGenerated
{
public:
    SyncStatus(StatusHandler* updater, wxWindow* parentWindow);
    ~SyncStatus();

    enum SyncStatusID
    {
        ABORTED,
        FINISHED_WITH_SUCCESS,
        FINISHED_WITH_ERROR,
        PAUSE,
        SCANNING,
        COMPARING_CONTENT,
        SYNCHRONIZING
    };

    void resetGauge(int totalObjectsToProcess, wxLongLong totalDataToProcess);
    void incProgressIndicator_NoUpdate(int objectsProcessed, wxLongLong dataProcessed);
    void setStatusText_NoUpdate(const Zstring& text);
    void updateStatusDialogNow();

    void setCurrentStatus(SyncStatusID id);
    void processHasFinished(SyncStatusID id);  //essential to call this in StatusUpdater derived class destructor at the LATEST(!) to prevent access to currentStatusUpdater

private:
    void OnOkay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnAbort(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    wxStopWatch timeElapsed;

    StatusHandler* currentStatusHandler;
    wxWindow* windowToDis;
    bool currentProcessIsRunning;

    //gauge variables
    int        totalObjects;
    wxLongLong totalData;
    int        currentObjects; //each object represents a file or directory processed
    wxLongLong currentData;    //each data element represents one byte for proper progress indicator scaling
    double     scalingFactor;  //nr of elements has to be normalized to smaller nr. because of range of int limitation

    Zstring currentStatusText;
    bool processPaused;
    SyncStatusID currentStatus;

    //remaining time
    std::auto_ptr<Statistics> statistics;
    long lastStatCallSpeed;   //used for calculating intervals between statistics update
    long lastStatCallRemTime; //
};

#endif // SMALLDIALOGS_H_INCLUDED
