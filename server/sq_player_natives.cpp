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

// setPlayerArmour
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
SQInteger sq_setPlayerPos(SQVM * pVM)
{
	SQInteger playerSystemAddress;

	VECTOR pVec;

	sq_getinteger(pVM, -4, &playerSystemAddress);
	sq_getfloat(pVM, -3, &pVec.X);
	sq_getfloat(pVM, -2, &pVec.Y);
	sq_getfloat(pVM, -1, &pVec.Z);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		RakNet::BitStream bsSend;
		bsSend.Write((char *)&pVec, sizeof(VECTOR));
		pNetGame->GetRPC4()->Call("Script_SetPos",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

		sq_pushbool(pVM, true);
		return 1;
	}

	sq_pushbool(pVM, false);
	return 1;
}

// getPlayerPos
SQInteger sq_getPlayerPos(SQVM * pVM)
{
	VECTOR pos;
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
		VECTOR vPos;
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
		VECTOR vRot;
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
		VECTOR vPoint;
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

SQInteger sq_banIP(SQVM * pVM)
{
	const char * banmask;
	sq_getstring(pVM, -1, &banmask);

	pNetGame->AddBan((char*)banmask);
	sq_pushbool(pVM, true);
	return 1;
}

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

// addPlayerClass
SQInteger sq_addPlayerClass(SQVM * pVM)
{
	SQInteger team, model, weapon1, weapon2, weapon3, ammo1, ammo2, ammo3;
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
