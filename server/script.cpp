//----------------------------------------------------
//
// GPL code license:
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): jenksta
//                 adamix
//
//-----------------------------------------------------

#include "../raknet/WindowsIncludes.h"
#include "netgame.h"
#include "plugins.h"
extern CNetGame *pNetGame;
extern CPlugins *pPlugins;

#include <stdio.h>
#include "script.h"
#include "../squirrel/sqstdaux.h"
#include "../squirrel/sqstdblob.h"
#include "../squirrel/sqstdio.h"
#include "../squirrel/sqstdmath.h"
#include "../squirrel/sqstdstring.h"
#include "../squirrel/sqstdsystem.h"

void printfunc(SQVM * pVM, const char * szFormat, ...)
{
	va_list vl;
	char szBuffer[512];
	va_start(vl, szFormat);
	vsprintf(szBuffer, szFormat, vl);
	va_end(vl);
	logprintf(szBuffer);
}

CScript::CScript(const char *szScriptName)
{
	// get the script path
	char szScriptPath[512];
	sprintf(szScriptPath, "scripts/%s", szScriptName);

	// make sure the script exists
	FILE * fp = fopen(szScriptPath, "rb");

	if(!fp) {
		// script does not exist
		return;
	}

	fclose(fp);

	// set the script name
	strcpy(m_szScriptName, szScriptName);

	// create the squirrel VM with an initial stack size of 1024 bytes
	m_pVM = sq_open(1024);

	// get the script vm pointer
	SQVM * pVM = m_pVM;

	// register the default error handlers
	sqstd_seterrorhandlers(pVM);

	// set the print and error functions
	sq_setprintfunc(pVM, printfunc, printfunc);

	// push the root table onto the stack
	sq_pushroottable(pVM);

	// register the blob library
	sqstd_register_bloblib(pVM);

	// register the input/out library
	// sqstd_register_iolib(pVM); -- disabled for security reasons. need to fix paths and enable again.

	// register the math library
	sqstd_register_mathlib(pVM);

	// register the string library
	sqstd_register_stringlib(pVM);

	// register the system library
	sqstd_register_systemlib(pVM);

	// register the vcmp functions
	sq_register_vcmp(pVM);

	// register the timer functions
	sq_register_timer(pVM);

	// load and compile the script
	if(SQ_FAILED(sqstd_dofile(pVM, szScriptPath, SQFalse, SQTrue))) {
		// script compilation failed
		return;
	}

	// pop the root table from the stack
	sq_pop(pVM, 1);

	// call the plugins
	pPlugins->OnScriptLoad(pVM);

	// script loaded successfully
	return;
}

CScript::~CScript()
{
	// kill all timers
	pNetGame->GetTimerPool()->HandleScriptUnload(m_pVM);

	// close the script vm
	sq_close(m_pVM);

	// reset the script vm pointer
	m_pVM = NULL;
}

