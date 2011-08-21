// **************************************************************************
// * This file is part of the FreeFileSync project. It is distributed under *
// * GNU General Public License: http://www.gnu.org/licenses/gpl.html       *
// * Copyright (C) 2008-2011 ZenJu (zhnmju123 AT gmx.de)                    *
// **************************************************************************

#ifndef UPDATEVERSION_H_INCLUDED
#define UPDATEVERSION_H_INCLUDED


namespace zen
{
void checkForUpdateNow();

void checkForUpdatePeriodically(long& lastUpdateCheck);
}

#endif // UPDATEVERSION_H_INCLUDED
