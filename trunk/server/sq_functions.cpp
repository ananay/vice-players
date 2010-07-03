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
// File Author: Christopher
//
//-----------------------------------------------------

#include "sq_functions.h"
#include "netgame.h"

extern CNetGame *pNetGame;

using namespace RakNet;

// setPlayerHealth
int sq_setPlayerHealth(SQVM * pVM)
{
	int playerSystemAddress;
	float newHealthValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newHealthValue);

	RakNet::BitStream bsSend;
	bsSend.Write(newHealthValue);
	pNetGame->GetRPC4()->Call("Script_SetHealth",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerArmour
int sq_setPlayerArmour(SQVM * pVM)
{
	int playerSystemAddress;
	float newArmourValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newArmourValue);

	RakNet::BitStream bsSend;
	bsSend.Write(newArmourValue);
	pNetGame->GetRPC4()->Call("Script_SetArmour",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerPos
int sq_setPlayerPos(SQVM * pVM)
{
	int playerSystemAddress;

	VECTOR pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	RakNet::BitStream bsSend;
	bsSend.Write((char *)&pVec, sizeof(VECTOR));
	pNetGame->GetRPC4()->Call("Script_SetPos",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerZAngle
int sq_setPlayerZAngle(SQVM * pVM)
{
	int playerSystemAddress;

	float fZAngle;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getfloat(pVM, -1, &fZAngle);

	RakNet::BitStream bsSend;
	bsSend.Write(fZAngle);
	pNetGame->GetRPC4()->Call("Script_SetPlayerZAngle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerSkin
int sq_setPlayerSkin(SQVM * pVM)
{
	int playerSystemAddress;
	int newSkinID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newSkinID);

	RakNet::BitStream bsSend;
	bsSend.Write(newSkinID);
	pNetGame->GetRPC4()->Call("Script_SetPlayerSkin",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerAction
int sq_setPlayerAction(SQVM * pVM)
{
	int playerSystemAddress;
	int newActionID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newActionID);

	RakNet::BitStream bsSend;
	bsSend.Write(newActionID);
	pNetGame->GetRPC4()->Call("Script_SetPlayerAction",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerRotation
int sq_setPlayerRotation(SQVM * pVM)
{
	int playerSystemAddress;
	float newRot;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newRot);

	RakNet::BitStream bsSend;
	bsSend.Write(newRot);
	pNetGame->GetRPC4()->Call("Script_SetPlayerRotation",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerRotation
int sq_resetPlayerWeapons(SQVM * pVM)
{
	int playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	pNetGame->GetRPC4()->Call("Script_ResetWeapons",NULL,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerArmedWeapon
int sq_setPlayerArmedWeapon(SQVM * pVM)
{
	int playerSystemAddress;
	int newWeaponID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newWeaponID);

	RakNet::BitStream bsSend;
	bsSend.Write(newWeaponID);
	pNetGame->GetRPC4()->Call("Script_SetArmedWeapon",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// removePlayerFromVehicle
int sq_removePlayerFromVehicle(SQVM * pVM)
{
	int playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	pNetGame->GetRPC4()->Call("Script_RemoveFromVehicle",NULL,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// togglePlayerControls
int sq_togglePlayerControls(SQVM * pVM)
{
	int playerSystemAddress;
	int toggleValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getinteger(pVM, -1, &toggleValue);

	RakNet::BitStream bsSend;
	bsSend.Write(toggleValue);
	pNetGame->GetRPC4()->Call("Script_ToggleControls",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// sendPlayerMessage
int sq_sendPlayerMessage(SQVM * pVM)
{
	int playerSystemAddress;
	int colourMessage;
	const char * messageValue;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &colourMessage);
	sq_getstring(pVM, -1, &messageValue);

	RakNet::BitStream bsSend;
	bsSend.Write(colourMessage);
	bsSend.Write(strlen(messageValue));
	bsSend.Write(messageValue,strlen(messageValue));
	pNetGame->GetRPC4()->Call("Script_ClientMessage",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}
// sendMessageAsPlayer
int sq_sendMessageAsPlayer(SQVM * pVM)
{
	int playerSystemAddress;
	const char * Message;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getstring(pVM, -1, &Message);

	RakNet::BitStream bsSend;
	bsSend.Write(Message);
	pNetGame->GetRPC4()->Call("Script_sendMessageAsPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// sendPlayerMessageToAll
int sq_sendPlayerMessageToAll(SQVM * pVM)
{
	int colourMessage;
	const char * messageValue;

	sq_getinteger(pVM, -2, &colourMessage);
	sq_getstring(pVM, -1, &messageValue);
	
	RakNet::BitStream bsSend;
	bsSend.Write(colourMessage);
	bsSend.Write(strlen(messageValue));
	bsSend.Write(messageValue,strlen(messageValue));

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	for(BYTE i = 0; i < MAX_PLAYERS; i++)
	{
		if(pPlayerPool->GetSlotState(i)) 
		{
			pNetGame->GetRPC4()->Call("Script_ClientMessage",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}
	sq_pushbool(pVM, true);
	return 1;
}

// setPlayerWorldBounds
int sq_setPlayerWorldBounds(SQVM * pVM)
{
	int playerSystemAddress;
	float LowX;
	float LowY;
	float HighX;
	float HighY;

	sq_getinteger(pVM, -5, &playerSystemAddress);
	sq_getfloat(pVM, -1, &HighY);
	sq_getfloat(pVM, -2, &HighX);
	sq_getfloat(pVM, -3, &LowY);
	sq_getfloat(pVM, -4, &LowX);

	RakNet::BitStream bsSend;
	bsSend.Write(LowX);
	bsSend.Write(LowY);
	bsSend.Write(HighX);
	bsSend.Write(HighY);
	pNetGame->GetRPC4()->Call("Script_WorldBounds",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// getPlayerName
int sq_getPlayerName(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	const char *pName = pNetGame->GetPlayerPool()->GetPlayerName(playerSystemAddress);

	sq_pushstring(pVM, pName, -1);
	return 1;
}

// getPlayerIP
int sq_getPlayerIP(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	const char *ip = pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress).ToString(false);

	sq_pushstring(pVM, ip, -1);
	return 1;
}

// createVehicle
int sq_createVehicle(SQVM * pVM)
{
	int byteVehicleType;
	VECTOR pos;
	float fRotation;
	int iColor1, iColor2;
	sq_getinteger(pVM, -7, &byteVehicleType);
	sq_getfloat(pVM, -6, &pos.X);
	sq_getfloat(pVM, -5, &pos.Y);
	sq_getfloat(pVM, -4, &pos.Z);
	sq_getfloat(pVM, -3, &fRotation);
	sq_getinteger(pVM, -2, &iColor1);
	sq_getinteger(pVM, -1, &iColor2);
	int vehID = pNetGame->GetVehiclePool()->New(byteVehicleType, &pos, fRotation, iColor1, iColor2);
	CVehicle *pVehicle = pNetGame->GetVehiclePool()->GetAt(vehID);
	pVehicle->SpawnForWorld();
	sq_pushinteger(pVM, vehID);
	return 1;
}

// addPlayerClass
int sq_addPlayerClass(SQVM * pVM)
{
	int team, model, weapon1, weapon2, weapon3, ammo1, ammo2, ammo3;
	float x, y, z, rot;
	sq_getinteger(pVM, -12, &team);
	sq_getinteger(pVM, -11, &model);
	sq_getfloat(pVM, -10, &x);
	sq_getfloat(pVM, -9, &y);
	sq_getfloat(pVM, -8, &z);
	sq_getfloat(pVM, -7, &rot);
	sq_getinteger(pVM, -6, &weapon1);
	sq_getinteger(pVM, -5, &ammo1);
	sq_getinteger(pVM, -4, &weapon2);
	sq_getinteger(pVM, -3, &ammo2);
	sq_getinteger(pVM, -2, &weapon3);
	sq_getinteger(pVM, -1, &ammo3);
	int classid = pNetGame->GetGameLogic()->addPlayerClass(team, model, x, y, z, rot,
								weapon1, ammo1,
								weapon2, ammo2,
								weapon3, ammo3);

	sq_pushinteger(pVM, classid);
	return 1;
}

#define _DECL_FUNC(name,nparams,pmask) {_SC(#name),sq_##name,nparams,pmask}
static SQRegFunction vcmp_funcs[]={
	// put functions here
	//_DECL_FUNC(func_name,func_params,_SC(func_param_template)),
	_DECL_FUNC(getPlayerName, 2, _SC(".n")),
	_DECL_FUNC(getPlayerIP, 2, _SC(".n")),
	_DECL_FUNC(createVehicle, 8, _SC(".nnnnnnn")),
	_DECL_FUNC(addPlayerClass, 13, _SC(".nnnnnnnnnnnn")),
	_DECL_FUNC(setPlayerHealth, 3, _SC(".nn")),
	_DECL_FUNC(setPlayerArmour, 3, _SC(".nn")),
	_DECL_FUNC(sendPlayerMessage, 4, _SC(".iis")),
	_DECL_FUNC(sendPlayerMessageToAll, 3, _SC(".is")),
	_DECL_FUNC(setPlayerWorldBounds, 5, _SC(".n")),
	_DECL_FUNC(togglePlayerControls, 3, _SC(".n")),
	_DECL_FUNC(removePlayerFromVehicle, 2, _SC(".n")),
	_DECL_FUNC(setPlayerArmedWeapon, 3, _SC(".n")),
	//_DECL_FUNC(sendMessageAsPlayer, 3, _SC(".is")),
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