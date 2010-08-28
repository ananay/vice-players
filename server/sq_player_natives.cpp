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

#include "sq_player_natives.h"
#include "netgame.h"

extern CNetGame *pNetGame;

using namespace RakNet;

// Player funcs //

// setPlayerHealth
SQInteger sq_setPlayerHealth(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float newHealthValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newHealthValue);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newHealthValue);
		pNetGame->GetRPC4()->Call("Script_SetHealth",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_forceClassSelection(SQVM * pVM)
{
	SQInteger playerSystemAddress;		

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		pNetGame->GetRPC4()->Call("Script_forceClassSelection",NULL,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerArmourr
SQInteger sq_setPlayerArmour(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float newArmourValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newArmourValue);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newArmourValue);
		pNetGame->GetRPC4()->Call("Script_SetArmour",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerPos
SQInteger sq_setPlayerPosition(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	Vector3 pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((char *)&pVec, sizeof(Vector3));
		pNetGame->GetRPC4()->Call("Script_SetPos",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerTurnSpeed
SQInteger sq_setPlayerTurnSpeed(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	Vector3 pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((char *)&pVec, sizeof(Vector3));
		pNetGame->GetRPC4()->Call("Script_SetTurnSpeed",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerMoveSpeed
SQInteger sq_setPlayerMoveSpeed(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	Vector3 pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((char *)&pVec, sizeof(Vector3));
		pNetGame->GetRPC4()->Call("Script_SetMoveSpeed",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerPos
SQInteger sq_getPlayerPosition(SQVM * pVM)
{
	Vector3 pos;
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
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

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerZAngle
SQInteger sq_setPlayerZAngle(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	float fZAngle;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getfloat(pVM, -1, &fZAngle);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(fZAngle);
		pNetGame->GetRPC4()->Call("Script_SetPlayerZAngle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerSkin
SQInteger sq_getPlayerSkin(SQVM *pVM)
{
	SQInteger playerSystemAddress;
	int iSkin;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		iSkin = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetSkin();

		sq_pushinteger(pVM, iSkin);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerSkin
SQInteger sq_setPlayerSkin(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newSkinID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newSkinID);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newSkinID);
		pNetGame->GetRPC4()->Call("Script_SetPlayerSkin",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);
		pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->SetSpawnSkin(newSkinID);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerAction
SQInteger sq_setPlayerAction(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newActionID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newActionID);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newActionID);
		pNetGame->GetRPC4()->Call("Script_SetPlayerAction",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerRotation
SQInteger sq_setPlayerRotation(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float newRot;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newRot);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newRot);
		pNetGame->GetRPC4()->Call("Script_SetPlayerRotation",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerRotation
SQInteger sq_getPlayerRotation(SQVM *pVM)
{
	SQInteger playerSystemAddress;
	float fRot;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		fRot = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetRotation();

		sq_pushfloat(pVM, fRot);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// resetPlayerWeapons
SQInteger sq_resetPlayerWeapons(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		pNetGame->GetRPC4()->Call("Script_ResetWeapons",NULL,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
// setPlayerArmedWeapon
SQInteger sq_setPlayerArmedWeapon(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newWeaponID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getinteger(pVM, -1, &newWeaponID);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newWeaponID);
		pNetGame->GetRPC4()->Call("Script_SetArmedWeapon",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// givePlayerWeapon
SQInteger sq_givePlayerWeapon(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger newWeaponID;
	SQInteger Ammo;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &Ammo);
    sq_getinteger(pVM, -1, &newWeaponID);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(newWeaponID);
		bsSend.Write(Ammo);
		pNetGame->GetRPC4()->Call("Script_GivePlayerWeapon",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// removePlayerFromVehicle
SQInteger sq_removePlayerFromVehicle(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		pNetGame->GetRPC4()->Call("Script_RemoveFromVehicle",NULL,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// putPlayerInVehicle
SQInteger sq_putPlayerInVehicle(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger vehID;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getinteger(pVM, -1, &vehID);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(vehID);

		pNetGame->GetRPC4()->Call("Script_PutInVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// togglePlayerControls
SQInteger sq_togglePlayerControls(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger toggleValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getinteger(pVM, -1, &toggleValue);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(toggleValue);
		pNetGame->GetRPC4()->Call("Script_ToggleControls",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// sendPlayerMessage
SQInteger sq_sendPlayerMessage(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger colourMessage;
	const char * messageValue;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &colourMessage);
	sq_getstring(pVM, -1, &messageValue);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((DWORD)colourMessage);
		bsSend.Write(strlen(messageValue));
		bsSend.Write(messageValue,strlen(messageValue));
		pNetGame->GetRPC4()->Call("Script_ClientMessage",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}
// sendMessageAsPlayer
SQInteger sq_sendMessageAsPlayer(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	const char * Message;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getstring(pVM, -1, &Message);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(Message);
		pNetGame->GetRPC4()->Call("Script_sendMessageAsPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// sendPlayerMessageToAll
SQInteger sq_sendPlayerMessageToAll(SQVM * pVM)
{
	SQInteger colourMessage;
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
SQInteger sq_setPlayerWorldBounds(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	float LowX;
	float LowY;
	float HighX;
	float HighY;

	sq_getinteger(pVM, -5, &playerSystemAddress);
	sq_getfloat(pVM, -1, &HighY);
	sq_getfloat(pVM, -2, &HighX);
	sq_getfloat(pVM, -3, &LowY);
	sq_getfloat(pVM, -4, &LowX);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(LowX);
		bsSend.Write(LowY);
		bsSend.Write(HighX);
		bsSend.Write(HighY);
		pNetGame->GetRPC4()->Call("Script_WorldBounds",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerTime
SQInteger sq_setPlayerTime(SQVM * pVM)
{
	SQInteger h, m;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &h);
	sq_getinteger(pVM, -1, &m);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->SetGameTime(h, m);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerHealth
SQInteger sq_getPlayerHealth(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		int health = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetHealth();

		sq_pushinteger(pVM, health);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerArmour
SQInteger sq_getPlayerArmour(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		int armour = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetArmour();

		sq_pushinteger(pVM, armour);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerName
SQInteger sq_getPlayerName(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		const char *pName = pNetGame->GetPlayerPool()->GetPlayerName(playerSystemAddress);

		sq_pushstring(pVM, pName, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerIP
SQInteger sq_getPlayerIP(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		const char *ip = pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress).ToString(false);

		sq_pushstring(pVM, ip, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// isConnected
SQInteger sq_isConnected(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	BOOL connected = pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress);
	sq_pushbool(pVM, connected);
	return 1;
}

// isPlayerInVehicle
SQInteger sq_isPlayerInVehicle(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		BOOL inVehicle = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->IsInVehicle();
		sq_pushbool(pVM, inVehicle);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerVehicleID
SQInteger sq_getPlayerVehicleID(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);
	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
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
	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraPos
SQInteger sq_setPlayerCameraPos(SQVM * pVM)
{
	SQFloat	X, Y, Z;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		Vector3 vPos;
		vPos.X = X;
		vPos.Y = Y;
		vPos.Z = Z;
		pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->SetCameraPos(vPos);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraRot
SQInteger sq_setPlayerCameraRot(SQVM * pVM)
{
	SQFloat	X, Y, Z;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		Vector3 vRot;
		vRot.X = X;
		vRot.Y = Y;
		vRot.Z = Z;
		pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->SetCameraRot(vRot);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraLookAt
SQInteger sq_setPlayerCameraLookAt(SQVM * pVM)
{
	SQFloat	X, Y, Z;
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		Vector3 vPoint;
		vPoint.X = X;
		vPoint.Y = Y;
		vPoint.Z = Z;
		pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->SetCameraLookAt(vPoint);
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// setCameraBehindPlayer
SQInteger sq_setCameraBehindPlayer(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->SetCameraBehindPlayer();
		sq_pushbool(pVM, true);
	}

	sq_pushbool(pVM, false);
	return 1;
}

// playSound
SQInteger sq_playSound(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger sound;
	SQFloat	  X, Y, Z;

	sq_getinteger(pVM, -5, &playerSystemAddress);
	sq_getinteger(pVM, -4, &sound);
	sq_getfloat(pVM, -3, &X);
	sq_getfloat(pVM, -2, &Y);
	sq_getfloat(pVM, -1, &Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(sound);
		bsSend.Write(X);
		bsSend.Write(Y);
		bsSend.Write(Z);
		pNetGame->GetRPC4()->Call("Script_PlaySound",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// fadeScreen
SQInteger sq_fadeScreen(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	SQInteger type, time;

	sq_getinteger(pVM, -3, &playerSystemAddress);
	sq_getinteger(pVM, -2, &type);
	sq_getinteger(pVM, -1, &time);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write(type);
		bsSend.Write(time);
		pNetGame->GetRPC4()->Call("Script_FadeScreen",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// kickPlayer
SQInteger sq_kickPlayer(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		pNetGame->KickPlayer(playerSystemAddress);
		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// banIP
SQInteger sq_banIP(SQVM * pVM)
{
	const char * banmask;
	sq_getstring(pVM, -1, &banmask);

	pNetGame->AddBan((char*)banmask);
	sq_pushbool(pVM, true);
	return 1;
}


// getPlayerTeam
SQInteger sq_getPlayerTeam(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		sq_pushbool(pVM, pNetGame->GetPlayerPool()->GetAt(playerSystemAddress)->GetTeam());
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerTurnSpeed
SQInteger sq_getPlayerTurnSpeed(SQVM * pVM)
{
	Vector3 speed;
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		CPlayer *pPlayer = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress);
		pPlayer->GetTurnSpeed(&speed);

		sq_newarray(pVM, 0);
		sq_pushfloat(pVM, speed.X);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Y);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Z);
		sq_arrayappend(pVM, -2);
		sq_push(pVM, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerMoveSpeed
SQInteger sq_getPlayerMoveSpeed(SQVM * pVM)
{
	Vector3 speed;
	SQInteger playerSystemAddress;
	sq_getinteger(pVM, -1, &playerSystemAddress);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		CPlayer *pPlayer = pNetGame->GetPlayerPool()->GetAt(playerSystemAddress);
		pPlayer->GetMoveSpeed(&speed);

		sq_newarray(pVM, 0);
		sq_pushfloat(pVM, speed.X);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Y);
		sq_arrayappend(pVM, -2);
		sq_pushfloat(pVM, speed.Z);
		sq_arrayappend(pVM, -2);
		sq_push(pVM, -1);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// addPlayerClass
SQInteger sq_addPlayerClass(SQVM * pVM)
{
	int iTeam;
	int iModel;
	int iWeapons[3];
	int iAmmo[3];
	Vector3 vecPosition;
	float fRotation;

	sq_getinteger(pVM, -12, &iTeam);
	sq_getinteger(pVM, -11, &iModel);
	sq_getfloat(pVM, -10, &vecPosition.X);
	sq_getfloat(pVM, -9, &vecPosition.Y);
	sq_getfloat(pVM, -8, &vecPosition.Z);
	sq_getfloat(pVM, -7, &fRotation);
	sq_getinteger(pVM, -6, &iWeapons[0]);
	sq_getinteger(pVM, -5, &iAmmo[0]);
	sq_getinteger(pVM, -4, &iWeapons[1]);
	sq_getinteger(pVM, -3, &iAmmo[1]);
	sq_getinteger(pVM, -2, &iWeapons[2]);
	sq_getinteger(pVM, -1, &iAmmo[2]);

	int classid = pNetGame->GetGameLogic()->AddPlayerClass(iTeam, iModel, vecPosition.X, vecPosition.Y, vecPosition.Z, 
		fRotation, iWeapons[0], iAmmo[0], iWeapons[1], iAmmo[1], iWeapons[2], iAmmo[2]);

	sq_pushinteger(pVM, classid);
	return 1;
}
