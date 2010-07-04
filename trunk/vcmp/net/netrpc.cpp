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
	BYTE byteMySystemAddress;

	bitStream->Read((char*)&pNetGame->m_vecInitPlayerPos, sizeof(VECTOR));
	bitStream->Read((char*)&pNetGame->m_vecInitCameraPos, sizeof(VECTOR));
	bitStream->Read((char*)&pNetGame->m_vecInitCameraLook, sizeof(VECTOR));
	bitStream->Read(pNetGame->m_WorldBounds[0]);
	bitStream->Read(pNetGame->m_WorldBounds[1]);
	bitStream->Read(pNetGame->m_WorldBounds[2]);
	bitStream->Read(pNetGame->m_WorldBounds[3]);
	bitStream->Read(pNetGame->m_iSpawnsAvailable);
	bitStream->Read(pNetGame->m_byteFriendlyFire);
	bitStream->Read(byteMySystemAddress);

	pPlayerPool->SetLocalSystemAddress(byteMySystemAddress);

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

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	if(pRemotePlayer) {
		pRemotePlayer->Say(szText);	
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
	BYTE byteRequestOutcome=0;
	PLAYER_SPAWN_INFO SpawnInfo;
	CLocalPlayer *pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	bitStream->Read(byteRequestOutcome);
	bitStream->Read(SpawnInfo.byteTeam);
	bitStream->Read(SpawnInfo.byteSkin);
	bitStream->Read((char*)&SpawnInfo.vecPos, sizeof(VECTOR));
	bitStream->Read(SpawnInfo.fRotation);
	bitStream->Read(SpawnInfo.iSpawnWeapons[0]);
	bitStream->Read(SpawnInfo.iSpawnWeaponsAmmo[0]);
	bitStream->Read(SpawnInfo.iSpawnWeapons[1]);
	bitStream->Read(SpawnInfo.iSpawnWeaponsAmmo[1]);
	bitStream->Read(SpawnInfo.iSpawnWeapons[2]);
	bitStream->Read(SpawnInfo.iSpawnWeaponsAmmo[2]);


	if(byteRequestOutcome) {
		pPlayer->SetSpawnInfo(SpawnInfo.byteTeam,SpawnInfo.byteSkin,&SpawnInfo.vecPos,
			SpawnInfo.fRotation,
			SpawnInfo.iSpawnWeapons[0],SpawnInfo.iSpawnWeaponsAmmo[0],
			SpawnInfo.iSpawnWeapons[1],SpawnInfo.iSpawnWeaponsAmmo[1],
			SpawnInfo.iSpawnWeapons[2],SpawnInfo.iSpawnWeaponsAmmo[2]);

		pNetGame->GetGameLogic()->HandleClassSelectionOutcome(&SpawnInfo, TRUE);
	}
	else {
		pNetGame->GetGameLogic()->HandleClassSelectionOutcome(NULL,FALSE);
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
// Server responding to our Death message.

void OwnDeath(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress=0;
	BYTE byteReason;
	BYTE byteWhoKilled;
	BYTE byteScoringModifier;

	bitStream->Read(byteSystemAddress);
	bitStream->Read(byteReason);
	bitStream->Read(byteWhoKilled);
	bitStream->Read(byteScoringModifier);

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	pLocalPlayer->HandleDeath(byteReason,byteWhoKilled,byteScoringModifier);
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

void UpdateScPing(RakNet::BitStream *bitStream, Packet *packet)
{	
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	int iPlayers = (packet->bitSize/8) / 9;
	int x=0;

	BYTE byteSystemAddress;
	int iPlayerScore;
	int iPlayerPing;
	ULONG ip;

	while(x!=iPlayers) {
		bitStream->Read(byteSystemAddress);
		bitStream->Read(iPlayerScore);
		bitStream->Read(iPlayerPing);
		bitStream->Read(ip);

		if( pPlayerPool->GetSlotState(byteSystemAddress) == TRUE ||
			byteSystemAddress == pPlayerPool->GetLocalSystemAddress() ) {

			pPlayerPool->UpdateScore(byteSystemAddress,iPlayerScore);
			pPlayerPool->UpdatePing(byteSystemAddress,iPlayerPing);
			pPlayerPool->UpdateIPAddress(byteSystemAddress,ip);
		}

		x++;
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

	pNetGame->GetRakPeer()->Shutdown(0);
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

void Script_showMarkersForPlayer(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	CLocalPlayer *pLocalPed = pNetGame->GetPlayerPool()->GetLocalPlayer();

	pPlayer->ShowMarker(pLocalPed->GetTeamColorAsRGBA());

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

// SetVehicleColor
void Script_DestroyVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE vehicle;
	bitStream->Read(vehicle);
	
	pNetGame->GetVehiclePool()->Delete(vehicle);
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
	pNetGame->GetRPC4()->RegisterFunction("OwnDeath",OwnDeath);
	pNetGame->GetRPC4()->RegisterFunction("EnterVehicle",EnterVehicle);
	pNetGame->GetRPC4()->RegisterFunction("ExitVehicle",ExitVehicle);
	pNetGame->GetRPC4()->RegisterFunction("VehicleSpawn",VehicleSpawn);
	pNetGame->GetRPC4()->RegisterFunction("UpdateScPing",UpdateScPing);
	pNetGame->GetRPC4()->RegisterFunction("ConnectionRejected",ConnectionRejected);
	pNetGame->GetRPC4()->RegisterFunction("Passenger",Passenger);

	pNetGame->GetRPC4()->RegisterFunction("Script_SetHealth",Script_SetHealth);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetArmour",Script_SetArmour);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetPos",Script_SetPos);
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
	pNetGame->GetRPC4()->RegisterFunction("Script_showMarkersForPlayer",Script_showMarkersForPlayer);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetVehicleHealth",Script_SetVehicleHealth);
	pNetGame->GetRPC4()->RegisterFunction("Script_SetVehicleColor",Script_SetVehicleColor);
	pNetGame->GetRPC4()->RegisterFunction("Script_DestroyVehicle",Script_DestroyVehicle);

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
	pNetGame->GetRPC4()->UnregisterFunction("OwnDeath");
	pNetGame->GetRPC4()->UnregisterFunction("EnterVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("ExitVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("VehicleSpawn");
	pNetGame->GetRPC4()->UnregisterFunction("UpdateScPing");
	pNetGame->GetRPC4()->UnregisterFunction("ConnectionRejected");
	pNetGame->GetRPC4()->UnregisterFunction("Passenger");

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
	pNetGame->GetRPC4()->UnregisterFunction("Script_showMarkersForPlayer");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetVehicleHealth");
	pNetGame->GetRPC4()->UnregisterFunction("Script_SetVehicleColor");
	pNetGame->GetRPC4()->UnregisterFunction("Script_DestroyVehicle");
}

//----------------------------------------------------
