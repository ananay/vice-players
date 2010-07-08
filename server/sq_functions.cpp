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
#include "netgame.h"

extern CNetGame *pNetGame;

using namespace RakNet;


#define _DECL_FUNC(name,nparams,pmask) {_SC(#name),sq_##name,nparams,pmask}
static SQRegFunction vcmp_funcs[]={
	// put functions here
	//_DECL_FUNC(func_name,func_params,_SC(func_param_template)),
	_DECL_FUNC(setScriptAuthor, 2, _SC(".s")),
	_DECL_FUNC(setScriptVersion, 2, _SC(".s")),
	_DECL_FUNC(loadClientScript, 3, _SC(".ns")),
	_DECL_FUNC(kickPlayer, 2, _SC(".n")),
	_DECL_FUNC(banIP, 2, _SC(".s")),
	_DECL_FUNC(setGameTime, 3, _SC(".nn")),
	_DECL_FUNC(getMaxPlayers, 1, NULL),
	_DECL_FUNC(getTickCount, 1, NULL),
	_DECL_FUNC(setPlayerTime, 4, _SC(".nnn")),
	_DECL_FUNC(getPlayerName, 2, _SC(".n")),
	_DECL_FUNC(getPlayerIP, 2, _SC(".n")),
	_DECL_FUNC(setPlayerCameraPos, 5, _SC(".nnnn")),
	_DECL_FUNC(setPlayerCameraRot, 5, _SC(".nnnn")),
	_DECL_FUNC(setPlayerCameraLookAt, 5, _SC(".nnnn")),
	_DECL_FUNC(setCameraBehindPlayer, 2, _SC(".n")),
	_DECL_FUNC(playSound, 6, _SC(".nnnnn")),
	_DECL_FUNC(fadeScreen, 3, _SC(".nnn")),
	_DECL_FUNC(addPlayerClass, 13, _SC(".nnnnnnnnnnnn")),
	_DECL_FUNC(getPlayerHealth, 2, _SC(".n")),
	_DECL_FUNC(setPlayerHealth, 3, _SC(".nn")),
	_DECL_FUNC(getPlayerArmour, 2, _SC(".n")),
	_DECL_FUNC(setPlayerArmour, 3, _SC(".nn")),
	_DECL_FUNC(sendPlayerMessage, 4, _SC(".iis")),
	_DECL_FUNC(sendPlayerMessageToAll, 3, _SC(".is")),
	_DECL_FUNC(setPlayerWorldBounds, 5, _SC(".n")),
	_DECL_FUNC(togglePlayerControls, 3, _SC(".n")),
	_DECL_FUNC(removePlayerFromVehicle, 2, _SC(".n")),
	_DECL_FUNC(putPlayerInVehicle, 3, _SC(".nn")),
	_DECL_FUNC(setPlayerArmedWeapon, 3, _SC(".n")),
	//_DECL_FUNC(sendMessageAsPlayer, 3, _SC(".is")),
	_DECL_FUNC(givePlayerWeapon, 4, _SC(".iii")),
	_DECL_FUNC(resetPlayerWeapons, 2, _SC(".n")),
	_DECL_FUNC(getPlayerRotation, 2, _SC(".n")),
	_DECL_FUNC(setPlayerRotation, 3, _SC(".nn")),
	_DECL_FUNC(getPlayerSkin, 2, _SC(".n")),
	_DECL_FUNC(setPlayerSkin, 3, _SC(".nn")),
	_DECL_FUNC(getPlayerPosition, 2, _SC(".n")),
	_DECL_FUNC(setPlayerPosition, 5, _SC(".nnnn")),
	_DECL_FUNC(setPlayerTurnSpeed, 5, _SC(".nnnn")),
	_DECL_FUNC(setPlayerMoveSpeed, 5, _SC(".nnnn")),
	_DECL_FUNC(isConnected, 2, _SC(".n")),
	_DECL_FUNC(getPlayerTeam, 2, _SC(".n")),
	_DECL_FUNC(setPlayerAction, 3, _SC(".nn")),
	_DECL_FUNC(isPlayerInVehicle, 2, _SC(".n")),
	_DECL_FUNC(getPlayerVehicleID, 2, _SC(".n")),
	_DECL_FUNC(getPlayerTurnSpeed, 2, _SC(".n")),
	_DECL_FUNC(getPlayerMoveSpeed, 2, _SC(".n")),
	_DECL_FUNC(createVehicle, 8, _SC(".nnnnnnn")),
	_DECL_FUNC(destroyVehicle, 2, _SC(".n")),
	_DECL_FUNC(setVehicleHealth, 3, _SC(".nn")),
	_DECL_FUNC(setVehicleColor, 4, _SC(".nnn")),
	_DECL_FUNC(getVehicleHealth, 2, _SC(".n")),
	_DECL_FUNC(getVehicleColors, 2, _SC(".n")),
	_DECL_FUNC(getVehiclePosition, 2, _SC(".n")),
	_DECL_FUNC(setVehiclePosition, 5, _SC(".nnnn")),
	_DECL_FUNC(getVehicleTurnSpeed, 2, _SC(".n")),
	_DECL_FUNC(getVehicleMoveSpeed, 2, _SC(".n")),
	_DECL_FUNC(setVehicleTurnSpeed, 5, _SC(".nnnn")),
	_DECL_FUNC(setVehicleMoveSpeed, 5, _SC(".nnnn")),
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