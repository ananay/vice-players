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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write(newHealthValue);
	pNetGame->GetRPC4()->Call("Script_SetHealth",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// setVehicleHealth
int sq_setVehicleHealth(SQVM * pVM)
{
	int vehicle;
	float newHealthValue;

	sq_getinteger(pVM, -2, &vehicle);
    sq_getfloat(pVM, -1, &newHealthValue);

	pNetGame->GetVehiclePool()->GetAt(vehicle)->SetHealth(newHealthValue);

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write((char *)&pVec, sizeof(VECTOR));
	pNetGame->GetRPC4()->Call("Script_SetPos",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// getPlayerPos
int sq_getPlayerPos(SQVM * pVM)
{
	VECTOR pos;
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	CPlayer *pPlayer = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress);
	pPlayer->GetPosition(&pos);

	sq_newarray(pVM, 0);
	sq_pushfloat(pVM, pos.X);
	sq_arrayappend(pVM, -2);
	sq_pushfloat(pVM, pos.Y);
	sq_arrayappend(pVM, -2);
	sq_pushfloat(pVM, pos.Z);
	sq_arrayappend(pVM, -2);
	sq_push(pVM, -1);
	return 1;
}

// setPlayerZAngle
int sq_setPlayerZAngle(SQVM * pVM)
{
	int playerSystemAddress;

	float fZAngle;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getfloat(pVM, -1, &fZAngle);

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write(newRot);
	pNetGame->GetRPC4()->Call("Script_SetPlayerRotation",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// getPlayerRotation
int sq_getPlayerRotation(SQVM *pVM)
{
	int playerSystemAddress;
	float fRot;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}
	
	fRot = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetRotation();

	sq_pushfloat(pVM, fRot);
	return 1;
}

// resetPlayerWeapons
int sq_resetPlayerWeapons(SQVM * pVM)
{
	int playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write(newWeaponID);
	pNetGame->GetRPC4()->Call("Script_SetArmedWeapon",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// givePlayerWeapon
int sq_givePlayerWeapon(SQVM * pVM)
{
	int playerSystemAddress;
	int newWeaponID;
	int Ammo;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &Ammo);
    sq_getinteger(pVM, -1, &newWeaponID);

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write(newWeaponID);
	bsSend.Write(Ammo);
	pNetGame->GetRPC4()->Call("Script_GivePlayerWeapon",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// removePlayerFromVehicle
int sq_removePlayerFromVehicle(SQVM * pVM)
{
	int playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	pNetGame->GetRPC4()->Call("Script_RemoveFromVehicle",NULL,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// putPlayerInVehicle
int sq_putPlayerInVehicle(SQVM * pVM)
{
	int playerSystemAddress;
	int vehID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getinteger(pVM, -1, &vehID);

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write(vehID);

	pNetGame->GetRPC4()->Call("Script_PutInVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write((DWORD)colourMessage);
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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

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

	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	RakNet::BitStream bsSend;
	bsSend.Write(LowX);
	bsSend.Write(LowY);
	bsSend.Write(HighX);
	bsSend.Write(HighY);
	pNetGame->GetRPC4()->Call("Script_WorldBounds",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

	sq_pushbool(pVM, true);
	return 1;
}

// getPlayerHealth
int sq_getPlayerHealth(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	int health = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetHealth();

	sq_pushinteger(pVM, health);
	return 1;
}

// getPlayerArmour
int sq_getPlayerArmour(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	int armour = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetArmour();

	sq_pushinteger(pVM, armour);
	return 1;
}

// getPlayerName
int sq_getPlayerName(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	const char *pName = pNetGame->GetPlayerPool()->GetPlayerName(playerSystemAddress);

	sq_pushstring(pVM, pName, -1);
	return 1;
}

// getPlayerIP
int sq_getPlayerIP(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress) == false)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	const char *ip = pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress).ToString(false);

	sq_pushstring(pVM, ip, -1);
	return 1;
}

// isConnected
int sq_isConnected(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	BOOL connected = pNetGame->GetPlayerPool()->IsConnected(playerSystemAddress);
	sq_pushbool(pVM, connected);
	return 1;
}

// isPlayerInVehicle
int sq_isPlayerInVehicle(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	BOOL inVehicle = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->IsInVehicle();
	sq_pushbool(pVM, inVehicle);
	return 1;
}

// getPlayerVehicleID
int sq_getPlayerVehicleID(SQVM * pVM)
{
	int playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	CPlayer *pPlayer = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress);
	if(pPlayer->IsInVehicle())
	{
		BYTE vehID = pPlayer->GetVehicleID();
		sq_pushinteger(pVM, vehID);
	}
	else
		sq_pushbool(pVM, false);

	return 1;
}

// setVehicleColor
int sq_setVehicleColor(SQVM * pVM)
{
	int vehicle, color1, color2;

	sq_getinteger(pVM, -3, &vehicle);
    sq_getinteger(pVM, -2, &color1);
	sq_getinteger(pVM, -1, &color2);

	pNetGame->GetVehiclePool()->GetAt(vehicle)->SetColor(color1, color2);

	sq_pushbool(pVM, true);
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

int sq_destroyVehicle(SQVM * pVM)
{
	int byteVehicle;
	sq_getinteger(pVM, -1, &byteVehicle);

	pNetGame->GetVehiclePool()->Delete(byteVehicle);
	sq_pushbool(pVM, true);

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
	_DECL_FUNC(destroyVehicle, 2, _SC(".n")),
	_DECL_FUNC(addPlayerClass, 13, _SC(".nnnnnnnnnnnn")),
	_DECL_FUNC(setPlayerHealth, 3, _SC(".nn")),
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
	_DECL_FUNC(setPlayerRotation, 3, _SC(".nn")),
	_DECL_FUNC(setPlayerSkin, 3, _SC(".nn")),
	_DECL_FUNC(setPlayerPos, 5, _SC(".nnnn")),
	_DECL_FUNC(getPlayerPos, 2, _SC(".n")),
	_DECL_FUNC(getPlayerRotation, 2, _SC(".n")),
	_DECL_FUNC(getPlayerHealth, 2, _SC(".n")),
	_DECL_FUNC(getPlayerArmour, 2, _SC(".n")),
	_DECL_FUNC(isConnected, 2, _SC(".n")),
	_DECL_FUNC(isPlayerInVehicle, 2, _SC(".n")),
	_DECL_FUNC(setVehicleHealth, 3, _SC(".nn")),
	_DECL_FUNC(setVehicleColor, 4, _SC(".nnn")),
	_DECL_FUNC(getPlayerVehicleID, 2, _SC(".n")),
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