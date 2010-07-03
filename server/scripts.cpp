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
//-----------------------------------------------------

#include "../raknet/WindowsIncludes.h"
#include <stdio.h>
#include "scripts.h"
#include "squirrel/sqstdaux.h"
#include "squirrel/sqstdblob.h"
#include "squirrel/sqstdio.h"
#include "squirrel/sqstdmath.h"
#include "squirrel/sqstdstring.h"
#include "squirrel/sqstdsystem.h"
#include "sq_functions.h"

void logprintf(char * format, ...);
void fatal_exit(char * szError);

void printfunc(SQVM * pVM, const char * szFormat, ...)
{
	va_list vl;
	char szBuffer[512];
	va_start(vl, szFormat);
	vsprintf(szBuffer, szFormat, vl);
	va_end(vl);
	logprintf(szBuffer);
}

CScripts::CScripts()
{
	// reset all script slots
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		m_pScripts[i] = NULL;
	}
}

bool CScripts::LoadScript(const char * szScriptName)
{
	// make sure a script with the same name isn't already loaded
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(!strcmp(m_szScriptNames[i], szScriptName)) {
			// a script with the same name already exists
			return false;
		}
	}

	// get the script path
	char szScriptPath[512];
	sprintf(szScriptPath, "scripts/%s", szScriptName);

	// make sure the script exists
	FILE * fp = fopen(szScriptPath, "rb");

	if(!fp) {
		// script does not exist
		return false;
	}

	fclose(fp);

	// find a free script slot
	int iSlot = -1;

	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(!m_pScripts[i]) {
			// found a free slot
			iSlot = i;
			break;
		}
	}

	if(iSlot == -1) {
		// no free script slot found
		return false;
	}

	// set the script name
	strcpy(m_szScriptNames[iSlot], szScriptName);

	// create the squirrel VM with an initial stack size of 1024 bytes
	m_pScripts[iSlot] = sq_open(1024);

	// get the script vm pointer
	SQVM * pVM = m_pScripts[iSlot];

	// register the default error handles
	sqstd_seterrorhandlers(pVM);

	// set the print and error functions
	sq_setprintfunc(pVM, printfunc, printfunc);

	// push the root table onto the stack
	sq_pushroottable(pVM);

	// register the blob library
	sqstd_register_bloblib(pVM);

	// register the input/out library
	sqstd_register_iolib(pVM);

	// register the math library
	sqstd_register_mathlib(pVM);

	// register the string library
	sqstd_register_stringlib(pVM);

	// register the system library
	sqstd_register_systemlib(pVM);

	// register the vcmp functions
	sq_register_vcmp(pVM);

	// load and compile the script
	if(SQ_FAILED(sqstd_dofile(pVM, szScriptPath, SQFalse, SQTrue))) {
		// script compilation failed/script does not exist
		return false;
	}

	// pop the root table from the stack
	sq_pop(pVM, 1);

	// script loaded successfully
	return true;
}

bool CScripts::UnloadScript(const char * szScriptName)
{
	// find the script slot
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(!strcmp(m_szScriptNames[i], szScriptName)) {
			// found the script slot, unload the script

			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// close the script vm
			sq_close(pVM);

			// reset the script vm pointer
			m_pScripts[i] = NULL;

			// script unloaded successfully
			return true;
		}
	}

	// script not found
	return false;
}

bool CScripts::LoadFromConfig(CConfig * pConfig)
{
	int iScriptCount = pConfig->GetConfigArrayCount("SCRIPT");
	int iScriptsLoaded = 0;
	
	for(int i = 0; i < iScriptCount; i++) {
		char * szScriptName = pConfig->GetConfigEntryAsString("SCRIPT", i);

		if(!LoadScript(szScriptName)) {
			logprintf("Failed to load script %s.", szScriptName);
		}
		else {
			iScriptsLoaded++;
		}
	}

	return (iScriptsLoaded > 0);
}

void CScripts::onServerInit()
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerInit", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerExit()
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerExit", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerConnect(int playerId, const char *name)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerConnect", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the player name onto the stack
				sq_pushstring(pVM, name, -1);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerDisconnect(int playerId, int reason)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerDisconnect", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the reason onto the stack
				sq_pushinteger(pVM, reason);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerText(int playerId, const char *text)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerText", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerSpawn(int playerId)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerSpawn", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Call the function
				sq_call(pVM, 2, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerRequestClass(int playerId, int classid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerRequestClass", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the class onto the stack
				sq_pushinteger(pVM, classid);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerDeath(int playerId, int reason)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerDeath", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the death reason onto the stack
				sq_pushinteger(pVM, reason);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerEnterVehicle(int playerId, int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerEnterVehicle", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerExitVehicle(int playerId, int vehicleid)
{
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerExitVehicle", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the vehicle id onto the stack
				sq_pushinteger(pVM, vehicleid);

				// Call the function
				sq_call(pVM, 3, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

bool CScripts::onRconCommand(const char *command, const char *arg)
{
	bool found = false;
	for(int i = 0; i < MAX_SCRIPTS; i++) {
		if(m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i];

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onRconCommand", -1);

			// Get the closure for the function
			if(SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the command onto the stack
				sq_pushstring(pVM, command, -1);

				// Push the command arguments onto the stack
				sq_pushstring(pVM, arg, -1);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, true, true)))
				{
					SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if(result) found = true;
				}

			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
	return found;
}