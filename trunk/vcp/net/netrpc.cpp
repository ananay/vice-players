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
//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
//----------------------------------------------------

#include "../main.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;
extern CPlayerPed	 *pLocalPlayer;
extern CScripts		 *pScripts;

using namespace RakNet;
extern CNetGame* pNetGame;

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerJoin(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	CHAR szPlayerName[MAX_PLAYER_NAME];
	BYTE byteSystemAddress;
	UINT uiNameLength;

	memset(szPlayerName,0,MAX_PLAYER_NAME);

	bitStream->Read(byteSystemAddress);
	bitStream->Read(uiNameLength);
	bitStream->Read(szPlayerName,uiNameLength);
	szPlayerName[uiNameLength] = '\0';

	// Add this client to the player pool.
	pPlayerPool->New(byteSystemAddress, szPlayerName);
}

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerQuit(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE byteSystemAddress;
	BYTE byteReason;

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteReason);

	// Delete this client from the player pool.
	pPlayerPool->Delete(byteSystemAddress,byteReason);
}


//----------------------------------------------------
// Server is giving us basic init information.

void InitGame(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE byteMySystemAddress, showOnRadar;

	bitStream->Read((char*)&pNetGame->m_vecInitPlayerPos, sizeof(VECTOR));
	bitStream->Read((char*)&pNetGame->m_vecInitCameraPos, sizeof(VECTOR));
	bitStream->Read((char*)&pNetGame->m_vecInitCameraLook, sizeof(VECTOR));
	bitStream->Read(pNetGame->m_WorldBounds[0]);
	bitStream->Read(pNetGame->m_WorldBounds[1]);
	bitStream->Read(pNetGame->m_WorldBounds[2]);
	bitStream->Read(pNetGame->m_WorldBounds[3]);
	bitStream->Read(pNetGame->m_byteFriendlyFire);
	bitStream->Read(showOnRadar);
	bitStream->Read(byteMySystemAddress);

	pPlayerPool->SetLocalSystemAddress(byteMySystemAddress);

	pGame->FadeScreen(1, 0);
	pNetGame->InitGameLogic();
	pNetGame->SetGameState(GAMESTATE_CONNECTED);
}

//----------------------------------------------------
// Remote player has sent a chat message.

void Chat(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress;
	BYTE byteTextLen;
	CHAR szText[256];

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteTextLen);
	bitStream->Read(szText,byteTextLen);

	szText[byteTextLen] = '\0';

	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	if(byteSystemAddress == pPlayerPool->GetLocalSystemAddress()) {
		pChatWindow->AddChatMessage(pNetGame->GetPlayerPool()->GetLocalPlayerName(),
			pPlayerPool->GetLocalPlayer()->GetTeamColorAsARGB(),szText);
	}
	else
	{
		CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
		if(pRemotePlayer) {
			pRemotePlayer->Say(szText);	
		}
	}
}

//----------------------------------------------------
// This should be rewritten as a packet instead of
// an RPC.

void Passenger(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress;
	BYTE byteVehicleID;
	UINT uiSeat;

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteVehicleID);
	bitStream->Read(uiSeat);
	
	CRemotePlayer * pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);

	if(pRemotePlayer) {
		pRemotePlayer->StorePassengerData(byteVehicleID,uiSeat);
	}	
}

//----------------------------------------------------
// Reply to our class request from the server.

void RequestClass(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteOutcome;
	int iRequestedClass;
	PLAYER_SPAWN_INFO SpawnInfo;
	CLocalPlayer * pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	CGameModeGeneric * pGameLogic = pNetGame->GetGameLogic();

	bitStream->Read(byteOutcome);
	if(byteOutcome) {
		bitStream->Read(iRequestedClass);
		bitStream->Read(SpawnInfo.byteTeam);
		bitStream->Read(SpawnInfo.byteSkin);
		bitStream->Read((char *)&SpawnInfo.vecPos, sizeof(VECTOR));
		bitStream->Read(SpawnInfo.fRotation);
		bitStream->Read(SpawnInfo.iSpawnWeapons[0]);
		bitStream->Read(SpawnInfo.iSpawnWeaponsAmmo[0]);
		bitStream->Read(SpawnInfo.iSpawnWeapons[1]);
		bitStream->Read(SpawnInfo.iSpawnWeaponsAmmo[1]);
		bitStream->Read(SpawnInfo.iSpawnWeapons[2]);
		bitStream->Read(SpawnInfo.iSpawnWeaponsAmmo[2]);

		pGameLogic->SetSelectedClass(iRequestedClass);
		pPlayer->SetSpawnInfo(SpawnInfo.byteTeam,SpawnInfo.byteSkin,&SpawnInfo.vecPos,
			SpawnInfo.fRotation,
			SpawnInfo.iSpawnWeapons[0],SpawnInfo.iSpawnWeaponsAmmo[0],
			SpawnInfo.iSpawnWeapons[1],SpawnInfo.iSpawnWeaponsAmmo[1],
			SpawnInfo.iSpawnWeapons[2],SpawnInfo.iSpawnWeaponsAmmo[2]);
		pGameLogic->HandleClassSelectionOutcome(&SpawnInfo, byteOutcome);
	}
}

//----------------------------------------------------
// Remote client is spawning.

void Spawn(RakNet::BitStream *bitStream, Packet *packet)
{
	CRemotePlayer *pRemotePlayer;

	BYTE byteSystemAddress=0;
	BYTE byteTeam=0;
	BYTE byteSkin=0;
	VECTOR vecPos;
	float fRotation=0;
	BYTE byteOnGround=0;
	int iSpawnWeapons1,iSpawnWeapons2,iSpawnWeapons3;
	int iSpawnWeaponsAmmo1,iSpawnWeaponsAmmo2,iSpawnWeaponsAmmo3;

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteTeam);
	bitStream->Read(byteSkin);
	bitStream->Read(vecPos.X);
	bitStream->Read(vecPos.Y);
	bitStream->Read(vecPos.Z);
	bitStream->Read(fRotation);
	bitStream->Read(iSpawnWeapons1);
	bitStream->Read(iSpawnWeaponsAmmo1);
	bitStream->Read(iSpawnWeapons2);
	bitStream->Read(iSpawnWeaponsAmmo2);
	bitStream->Read(iSpawnWeapons3);
	bitStream->Read(iSpawnWeaponsAmmo3);

	pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);

	if(pRemotePlayer) {
		pRemotePlayer->SpawnPlayer(byteTeam,byteSkin,&vecPos,fRotation,
			iSpawnWeapons1,iSpawnWeaponsAmmo1,
			iSpawnWeapons2,iSpawnWeaponsAmmo2,
			iSpawnWeapons3,iSpawnWeaponsAmmo3);
	}		
}

//----------------------------------------------------
// Remote client is dead.

void Death(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress=0;
	BYTE byteReason;
	BYTE byteWhoKilled;
	BYTE byteScoringModifier;

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteReason);
	bitStream->Read(byteWhoKilled);
	bitStream->Read(byteScoringModifier);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	if(pRemotePlayer) {
		pRemotePlayer->HandleDeath(byteReason,byteWhoKilled,byteScoringModifier);
	}
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void EnterVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress=0;
	BYTE byteVehicleID=0;
	BYTE bytePassenger=0;

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteVehicleID);
	bitStream->Read(bytePassenger);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pRemotePlayer) {
		if(!bytePassenger) {
			pRemotePlayer->GetPlayerPed()->EnterVehicleAsDriver(pVehiclePool->FindGtaIDFromID(byteVehicleID));
		} else {
			pRemotePlayer->GetPlayerPed()->EnterVehicleAsPassenger(pVehiclePool->FindGtaIDFromID(byteVehicleID));
		}
	}
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void ExitVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress=0;
	BYTE byteVehicleID=0;

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteVehicleID);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pRemotePlayer) {
		pRemotePlayer->GetPlayerPed()->ExitCurrentVehicle();
	}
}

//----------------------------------------------------

void VehicleSpawn(RakNet::BitStream *bitStream, Packet *packet)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	BYTE byteVehicleID=0;
	BYTE byteVehicleType;
	VECTOR vecPos;
	VECTOR vecSpawnPos;
	float fRotation;
	float fSpawnRotation;
	float fHealth;
	int iColor1, iColor2;

	bitStream->Read(byteVehicleID);
	bitStream->Read(byteVehicleType);
	bitStream->Read(vecPos.X);
	bitStream->Read(vecPos.Y);
	bitStream->Read(vecPos.Z);
	bitStream->Read(fRotation);
	bitStream->Read(iColor1);
	bitStream->Read(iColor2);
	bitStream->Read(fHealth);
	bitStream->Read(vecSpawnPos.X);
	bitStream->Read(vecSpawnPos.Y);
	bitStream->Read(vecSpawnPos.Z);
	bitStream->Read(fSpawnRotation);

	pVehiclePool->New(byteVehicleID,byteVehicleType,
		&vecPos,fRotation,iColor1,iColor2,&vecSpawnPos,fSpawnRotation);
	//pVehiclePool->GetAt(byteVehicleID)->SetHealth(fHealth);
}

//----------------------------------------------------

void VehicleDestroy(RakNet::BitStream *bitStream, Packet *packet)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	BYTE byteVehicleID=0;

	bitStream->Read(byteVehicleID);

	pVehiclePool->Delete(byteVehicleID);
}

//----------------------------------------------------

void UpdateScoreAndPing(RakNet::BitStream *bitStream, Packet *packet)
{	
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	int iPlayers = (packet->bitSize/8) / 9;

	BYTE byteSystemAddress;
	int iPlayerScore;
	int iPlayerPing;
	ULONG ip;

	for(BYTE i = 0; i < iPlayers; i++) {
		bitStream->Read(byteSystemAddress);
		bitStream->Read(iPlayerScore);
		bitStream->Read(iPlayerPing);
		bitStream->Read(ip);

		if(pPlayerPool->GetSlotState(byteSystemAddress) || byteSystemAddress == pPlayerPool->GetLocalSystemAddress()) {
			pPlayerPool->UpdateScore(byteSystemAddress,iPlayerScore);
			pPlayerPool->UpdatePing(byteSystemAddress,iPlayerPing);
			pPlayerPool->UpdateIPAddress(byteSystemAddress,ip);
		}
	}
}

//----------------------------------------------------

void ConnectionRejected(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteRejectReason;

	bitStream->Read(byteRejectReason);

	if(byteRejectReason==REJECT_REASON_BAD_VERSION) {
		pChatWindow->AddInfoMessage("CONNECTION REJECTED");
		pChatWindow->AddInfoMessage("YOU'RE USING AN INCORRECT VERSION!");
	}
	else if(byteRejectReason==REJECT_REASON_BAD_NICKNAME)
	{
		pChatWindow->AddInfoMessage("CONNECTION REJECTED");
		pChatWindow->AddInfoMessage("YOUR NICKNAME IS INVALID");
	}

	if(pNetGame) {
		pNetGame->Shutdown();
	}
}

//----------------------------------------------------

void SetGameTime(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE h, m;
	bitStream->Read(h);
	bitStream->Read(m);

	pGame->SetGameTime(h, m);
}

//----------------------------------------------------

void SetCameraPosition(RakNet::BitStream *bitStream, Packet *packet)
{
	VECTOR vPos;
	bitStream->Read(vPos.X);
	bitStream->Read(vPos.Y);
	bitStream->Read(vPos.Z);

	pGame->GetCamera()->SetPosition(vPos);
}

//----------------------------------------------------

void SetCameraRotation(RakNet::BitStream *bitStream, Packet *packet)
{
	VECTOR vRot;
	bitStream->Read(vRot.X);
	bitStream->Read(vRot.Y);
	bitStream->Read(vRot.Z);

	pGame->GetCamera()->SetRotation(vRot);
}

//----------------------------------------------------

void SetCameraLookAt(RakNet::BitStream *bitStream, Packet *packet)
{
	VECTOR vPoint;
	bitStream->Read(vPoint.X);
	bitStream->Read(vPoint.Y);
	bitStream->Read(vPoint.Z);

	pGame->GetCamera()->LookAtPoint(vPoint, 1);
}

//----------------------------------------------------

void SetCameraBehindPlayer(RakNet::BitStream *bitStream, Packet *packet)
{
	pGame->GetCamera()->SetBehindPlayer();
}

//----------------------------------------------------

void LoadClientScript(RakNet::BitStream *bitStream, Packet *packet)
{
	char szScriptName[256];
	UINT uiLength;
	bitStream->Read(uiLength);
	bitStream->Read(szScriptName,uiLength);
	szScriptName[uiLength] = '\0';
	int iSlot = pScripts->LoadScript((const char*)&szScriptName);
	pScripts->onInit(iSlot);
}

//----------------------------------------------------

// ============= Scripting RPC's ====================//

// SetHealth
void Script_SetHealth(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	float playerHealth;

	bitStream->Read(playerHealth);

	pPlayer->SetHealth(playerHealth);
}

// SetVehicleHealth
void Script_SetVehicleHealth(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	float newHealth;
	bitStream->Read(vehicle);
	bitStream->Read(newHealth);
	
	CVehicle *pVehicle = pNetGame->GetVehiclePool()->GetAt(vehicle);
	pVehicle->SetHealth(newHealth);
}

// SetArmour
void Script_SetArmour(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	float playerArmour;

	bitStream->Read(playerArmour);

	pPlayer->SetArmour(playerArmour);
}

// SetPlayerPos
void Script_SetPos(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	VECTOR playerPos;

	bitStream->Read((char *)&playerPos, sizeof(VECTOR));

	pPlayer->Teleport(playerPos.X, playerPos.Y, playerPos.Z);
}

// SetPlayerTurnSpeed
void Script_SetTurnSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	VECTOR speed;

	bitStream->Read((char *)&speed, sizeof(VECTOR));

	pPlayer->SetTurnSpeed(speed);
}

// SetPlayerTurnSpeed
void Script_SetMoveSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	VECTOR speed;

	bitStream->Read((char *)&speed, sizeof(VECTOR));

	pPlayer->SetMoveSpeed(speed);
}

// PutPlayerInVehicle
void Script_PutInVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int vehID;

	bitStream->Read(vehID);

	pPlayer->PutDirectlyInVehicle(pNetGame->GetVehiclePool()->FindGtaIDFromID(vehID));
}

// GiveWeapon
void Script_GivePlayerWeapon(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int weaponID;
	int ammoAMT;

	bitStream->Read(weaponID);
	bitStream->Read(ammoAMT);

	pPlayer->GiveWeapon(weaponID, ammoAMT);
}

// SetSkin
void Script_SetPlayerSkin(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int skinID;

	bitStream->Read(skinID);

	pPlayer->SetModel(skinID);
}

// SetZAngle
void Script_SetPlayerZAngle(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	float fRotation;

	bitStream->Read(fRotation);
	pPlayer->SetRotation(fRotation);
}

// setAction
void Script_SetPlayerAction(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	BYTE Action;

	bitStream->Read(Action);
	pPlayer->SetAction(Action);
}

// setPlayerRotation
void Script_SetPlayerRotation(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	float playerRot;

	bitStream->Read(playerRot);
	pPlayer->SetRotation(playerRot);
}

// resetPlayerWeapons
void Script_ResetWeapons(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	pPlayer->ClearAllWeapons();
}

// setArmedWeapon
void Script_SetArmedWeapon(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int playerWeapon;

	bitStream->Read(playerWeapon);

	pPlayer->SetArmedWeapon(playerWeapon);
}

// removePlayerFromVehicle
void Script_RemoveFromVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	pPlayer->ExitCurrentVehicle();
}

// togglecontrols
void Script_ToggleControls(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	int ControlValue;

	bitStream->Read(ControlValue);

	pPlayer->TogglePlayerControllable(ControlValue);
}

// send message to client
void Script_ClientMessage(RakNet::BitStream *bitStream, Packet *packet)
{
	DWORD dwColor;
	CHAR szMessage[256];
	UINT uiLength;

	bitStream->Read(dwColor);
	bitStream->Read(uiLength);
	bitStream->Read(szMessage,uiLength);
	szMessage[uiLength] = '\0';

	pChatWindow->AddClientMessage(dwColor,szMessage);
}

// setworldbounds
void Script_WorldBounds(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	float LowX;
	float LowY;
	float HighX;
	float HighY;

	bitStream->Read(LowX);
	bitStream->Read(LowY);
	bitStream->Read(HighX);
	bitStream->Read(HighY);

	pPlayer->EnforceWorldBoundries(HighX, LowX, HighY, LowY);
}

// SetVehicleColor
void Script_SetVehicleColor(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	int color1, color2;
	bitStream->Read(vehicle);
	bitStream->Read(color1);
	bitStream->Read(color2);
	
	CVehicle *pVehicle = pNetGame->GetVehiclePool()->GetAt(vehicle);
	pVehicle->SetColor(color1, color2);
}

// SetVehiclePos
void Script_SetVehiclePos(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	VECTOR pos;
	bitStream->Read(vehicle);
	bitStream->Read((char *)&pos, sizeof(VECTOR));

	CVehiclePool *pPool = pNetGame->GetVehiclePool();
	if(pPool->GetSlotState(vehicle))
	{
		pPool->GetAt(vehicle)->SetPosition(pos);
	}
}

// SetVehicleTurnSpeed
void Script_SetVehicleTurnSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	VECTOR speed;
	bitStream->Read(vehicle);
	bitStream->Read((char *)&speed, sizeof(VECTOR));

	CVehiclePool *pPool = pNetGame->GetVehiclePool();
	if(pPool->GetSlotState(vehicle))
	{
		pPool->GetAt(vehicle)->SetTurnSpeed(speed);
	}
}

// SetVehicleMoveSpeed
void Script_SetVehicleMoveSpeed(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	VECTOR speed;
	bitStream->Read(vehicle);
	bitStream->Read((char *)&speed, sizeof(VECTOR));

	CVehiclePool *pPool = pNetGame->GetVehiclePool();
	if(pPool->GetSlotState(vehicle))
	{
		pPool->GetAt(vehicle)->SetMoveSpeed(speed);
	}
}

// SetVehicleColor
void Script_DestroyVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	bitStream->Read(vehicle);
	
	pNetGame->GetVehiclePool()->Delete(vehicle);
}

// Play sound
void Script_PlaySound(RakNet::BitStream *bitStream, Packet *packet)
{
	int sound;
	VECTOR vPos;

	bitStream->Read(sound);
	bitStream->Read(vPos.X);
	bitStream->Read(vPos.Y);
	bitStream->Read(vPos.Z);

	pGame->PlaySound(sound, vPos);
}

// Fade
void Script_FadeScreen(RakNet::BitStream *bitStream, Packet *packet)
{
	int type, time;

	bitStream->Read(type);
	bitStream->Read(time);

	pGame->FadeScreen(type, time);
}

void RegisterRPCs()
{
	pNetGame->GetRPC4()->RegisterFunction("ServerJoin",ServerJoin);
	pNetGame->GetRPC4()->RegisterFunction("ServerQuit",ServerQuit);	
	pNetGame->GetRPC4()->RegisterFunction("InitGame",InitGame);
	pNetGame->GetRPC4()->RegisterFunction("Chat",Chat);
	pNetGame->GetRPC4()->RegisterFunction("RequestClass",RequestClass);
	pNetGame->GetRPC4()->RegisterFunction("Spawn",Spawn);
	pNetGame->GetRPC4()->RegisterFunction("Death",Death);
	pNetGame->GetRPC4()->RegisterFunction("EnterVehicle",EnterVehicle);
	pNetGame->GetRPC4()->RegisterFunction("ExitVehicle",ExitVehicle);
	pNetGame->GetRPC4()->RegisterFunction("VehicleSpawn",VehicleSpawn);
	pNetGame->GetRPC4()->RegisterFunction("VehicleDestroy",VehicleDestroy);
	pNetGame->GetRPC4()->RegisterFunction("UpdateScoreAndPing",UpdateScoreAndPing);
	pNetGame->GetRPC4()->RegisterFunction("ConnectionRejected",ConnectionRejected);
	pNetGame->GetRPC4()->RegisterFunction("Passenger",Passenger);
	pNetGame->GetRPC4()->RegisterFunction("SetGameTime",SetGameTime);
	pNetGame->GetRPC4()->RegisterFunction("SetCameraPosition",SetCameraPosition);
	pNetGame->GetRPC4()->RegisterFunction("SetCameraRotation",SetCameraRotation);
	pNetGame->GetRPC4()->RegisterFunction("SetCameraLookAt",SetCameraLookAt);
	pNetGame->GetRPC4()->RegisterFunction("SetCameraBehindPlayer",SetCameraBehindPlayer);
	pNetGame->GetRPC4()->RegisterFunction("LoadClientScript",LoadClientScript);

	pNetGame->GetRPC4()->RegisterFunction("Script_SetHealth",Script_SetHealth);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetArmour",Script_SetArmour);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetPos",Script_SetPos);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetTurnSpeed",Script_SetTurnSpeed);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetMoveSpeed",Script_SetMoveSpeed);
	pNetGame->GetRPC4()->RegisterFunction("Script_PutInVehicle",Script_PutInVehicle);
	pNetGame->GetRPC4()->RegisterFunction("Script_GivePlayerWeapon",Script_GivePlayerWeapon);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetPlayerSkin",Script_SetPlayerSkin);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetPlayerZAngle",Script_SetPlayerZAngle);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetPlayerAction",Script_SetPlayerAction);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetPlayerRotation",Script_SetPlayerRotation);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetArmedWeapon",Script_SetArmedWeapon);
	pNetGame->GetRPC4()->RegisterFunction("Script_RemoveFromVehicle",Script_RemoveFromVehicle);
	pNetGame->GetRPC4()->RegisterFunction("Script_ToggleControls",Script_ToggleControls);
	pNetGame->GetRPC4()->RegisterFunction("Script_ClientMessage",Script_ClientMessage);
	pNetGame->GetRPC4()->RegisterFunction("Script_WorldBounds",Script_WorldBounds);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetVehicleHealth",Script_SetVehicleHealth);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetVehicleColor",Script_SetVehicleColor);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetVehiclePos",Script_SetVehiclePos);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetVehicleTurnSpeed",Script_SetVehicleTurnSpeed);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetVehicleMoveSpeed",Script_SetVehicleMoveSpeed);
	pNetGame->GetRPC4()->RegisterFunction("Script_DestroyVehicle",Script_DestroyVehicle);
	pNetGame->GetRPC4()->RegisterFunction("Script_PlaySound",Script_PlaySound);
	pNetGame->GetRPC4()->RegisterFunction("Script_FadeScreen",Script_FadeScreen);

}

//----------------------------------------------------

void UnRegisterRPCs()
{
	pNetGame->GetRPC4()->UnregisterFunction("ServerJoin");
	pNetGame->GetRPC4()->UnregisterFunction("ServerQuit");
	pNetGame->GetRPC4()->UnregisterFunction("InitGame");
	pNetGame->GetRPC4()->UnregisterFunction("Chat");
	pNetGame->GetRPC4()->UnregisterFunction("RequestClass");
	pNetGame->GetRPC4()->UnregisterFunction("Spawn");
	pNetGame->GetRPC4()->UnregisterFunction("Death");
	pNetGame->GetRPC4()->UnregisterFunction("EnterVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("ExitVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("VehicleSpawn");
	pNetGame->GetRPC4()->UnregisterFunction("VehicleDestroy");
	pNetGame->GetRPC4()->UnregisterFunction("UpdateScoreAndPing");
	pNetGame->GetRPC4()->UnregisterFunction("ConnectionRejected");
	pNetGame->GetRPC4()->UnregisterFunction("Passenger");
	pNetGame->GetRPC4()->UnregisterFunction("SetGameTime");
	pNetGame->GetRPC4()->UnregisterFunction("SetCameraPosition");
	pNetGame->GetRPC4()->UnregisterFunction("SetCameraRotation");
	pNetGame->GetRPC4()->UnregisterFunction("SetCameraLookAt");
	pNetGame->GetRPC4()->UnregisterFunction("SetCameraBehindPlayer");
	pNetGame->GetRPC4()->UnregisterFunction("LoadClientScript");

	pNetGame->GetRPC4()->UnregisterFunction("Script_SetHealth");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetArmour");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetPos");
	pNetGame->GetRPC4()->UnregisterFunction("Script_PutInVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("Script_GivePlayerWeapon");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetPlayerSkin");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetPlayerZAngle");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetPlayerAction");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetPlayerRotation");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetArmedWeapon");
	pNetGame->GetRPC4()->UnregisterFunction("Script_RemoveFromVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("Script_ToggleControls");
	pNetGame->GetRPC4()->UnregisterFunction("Script_ClientMessage");
	pNetGame->GetRPC4()->UnregisterFunction("Script_WorldBounds");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetVehicleHealth");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetVehicleColor");
	pNetGame->GetRPC4()->UnregisterFunction("Script_DestroyVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("Script_PlaySound");
	pNetGame->GetRPC4()->UnregisterFunction("Script_FadeScreen");
}

//----------------------------------------------------
