/***************************************************************
 * Name:      FreeFileSyncApp.h
 * Purpose:   Defines Application Class
 * Author:    ZenJu (zhnmju123@gmx.de)
 * Created:   2008-07-16
 **************************************************************/

#ifndef FREEFILESYNCAPP_H
#define FREEFILESYNCAPP_H

#include <wx/app.h>
#include "library/processXml.h"
#include <wx/help.h>


class Application : public wxApp
{
public:
    bool OnInit();
    int  OnRun();
    int  OnExit();
    bool OnExceptionInMainLoop();
    void OnStartApplication(wxIdleEvent& event);

private:
    void runGuiMode(const wxString& cfgFileName, xmlAccess::XmlGlobalSettings& settings);
    void runBatchMode(const wxString& filename, xmlAccess::XmlGlobalSettings& globSettings);

    xmlAccess::XmlGlobalSettings globalSettings; //settings used by GUI, batch mode or both

    boost::shared_ptr<wxHelpController> helpController; //global help controller
    int returnValue;
};

#endif // FREEFILESYNCAPP_H
