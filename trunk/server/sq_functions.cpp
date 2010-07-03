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
// Support From: Jenksta
//-----------------------------------------------------

#include "sq_functions.h"
#include "main.h"
#include "netgame.h"


extern CNetGame		 *pNetGame;

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

	VECTOR pZVec;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getfloat(pVM, -1, &pZVec.Z);

	RakNet::BitStream bsSend;
	bsSend.Write(pZVec.Z);
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
	pNetGame->GetRPC4()->Call("Script_RemoveFromVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

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