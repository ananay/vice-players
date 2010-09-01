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
// File Authors: Christopher, adamix
//
//-----------------------------------------------------

#include "sq_functions.h"
#include "../main.h"

extern CNetGame *pNetGame;

using namespace RakNet;

#define _DECL_FUNC(name,nparams,pmask) {_SC(#name),sq_##name,nparams,pmask}
static SQRegFunction vcmp_funcs[]={
	// put functions here
	//_DECL_FUNC(func_name,func_params,_SC(func_param_template)),
	_DECL_FUNC(setScriptAuthor, 2, _SC(".s")),
	_DECL_FUNC(setScriptVersion, 2, _SC(".s")),
	_DECL_FUNC(addChatMessage, 2, _SC(".s")),
	_DECL_FUNC(toggleCheatCodes, 2, _SC(".b")),
	{0,0}
};

int sq_register_vcmp(SQVM * pVM)
{
	SQInteger i=0;
	while(vcmp_funcs[i].name!=0)
	{
		sq_pushstring(pVM,vcmp_funcs[i].name,-1);
		sq_newclosure(pVM,vcmp_funcs[i].f,0);
		sq_setparamscheck(pVM,vcmp_funcs[i].nparamscheck,vcmp_funcs[i].typemask);
		sq_setnativeclosurename(pVM,-1,vcmp_funcs[i].name);
		sq_createslot(pVM,-3);
		i++;
	}
	return 1;
}

static SQRegFunction timer_funcs[]={
	_DECL_FUNC(setTimer, 0, NULL),
	_DECL_FUNC(killTimer, 0, NULL),
	_DECL_FUNC(isTimerActive, 0, NULL),
	{0,0}
};

int sq_register_timer(SQVM * pVM)
{
	SQInteger i=0;
	while(timer_funcs[i].name!=0)
	{
		sq_pushstring(pVM,timer_funcs[i].name,-1);
		sq_newclosure(pVM,timer_funcs[i].f,0);
		sq_setnativeclosurename(pVM,-1,timer_funcs[i].name);
		sq_createslot(pVM,-3);
		i++;
	}
	return 1;
}