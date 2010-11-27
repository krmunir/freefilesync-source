// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) 2008-2010 ZenJu (zhnmju123 AT gmx.de)                    *
// **************************************************************************
//
#ifndef NOTIFY_H_INCLUDED
#define NOTIFY_H_INCLUDED

#ifndef FFS_WIN
use in windows build only
#endif

#include <wx/msw/wrapwin.h> //includes "windows.h"
#include "../shared/file_error.h"
#include <vector>
#include <memory>

//handle (user-) request for device removal via template method pattern
//evaluate directly after processing window messages
class NotifyRequestDeviceRemoval
{
public:
    NotifyRequestDeviceRemoval(const std::vector<HANDLE>& openHandles); //throw (FileError)
    virtual ~NotifyRequestDeviceRemoval();

private:
    virtual void onRequestRemoval(HANDLE hnd) = 0; //throw()!
    //NOTE: onRemovalFinished is NOT guaranteed to execute after onRequestRemoval()! but most likely will
    virtual void onRemovalFinished(HANDLE hnd, bool successful) = 0; //throw()!

    NotifyRequestDeviceRemoval(NotifyRequestDeviceRemoval&); //no copying
    void operator=(NotifyRequestDeviceRemoval&);             //

    class Pimpl;
    std::auto_ptr<Pimpl> pimpl;
};


#endif // NOTIFY_H_INCLUDED