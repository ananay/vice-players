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

#include "netgame.h"
#include "rcon.h"
#include "scripts.h"

using namespace RakNet;


RakNet::RakPeerInterface		*pRak=0;
extern CNetGame			*pNetGame;
extern char				*szAdminPass;
extern CRcon			*pRcon;
extern CScripts	*pScripts;
#ifndef WIN32
#	define stricmp strcasecmp
#endif

#define NETGAME_VERSION 6

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2

void FilterInvalidNickChars(PCHAR szString);

//----------------------------------------------------
// Sent by a client who's wishing to join us in our
// multiplayer-like activities.

void ClientJoin(RakNet::BitStream *bitStream, Packet *packet)
{
	RakNet::BitStream bsReject;
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	CHAR szPlayerName[MAX_PLAYER_NAME];
	BYTE byteSystemAddress;
	BYTE byteVersion;
	BYTE byteNickLen;
	BYTE byteRejectReason;

	bitStream->Read(byteVersion);

	if(byteVersion != NETGAME_VERSION) {
		byteRejectReason = REJECT_REASON_BAD_VERSION;
		bsReject.Write(byteRejectReason);
		pNetGame->GetRPC4()->Call("ConnectionRejected", &bsReject,HIGH_PRIORITY,RELIABLE,0,packet->guid,false);
		return;
	}

	bitStream->Read(byteNickLen);
	bitStream->Read(szPlayerName,byteNickLen);
	szPlayerName[byteNickLen] = '\0';

	FilterInvalidNickChars(szPlayerName);
	byteNickLen = strlen(szPlayerName);

	if(byteNickLen==0 || byteNickLen > 16 || pPlayerPool->IsNickInUse(szPlayerName)) {
		byteRejectReason = REJECT_REASON_BAD_NICKNAME;
		bsReject.Write(byteRejectReason);
		pNetGame->GetRPC4()->Call("ConnectionRejected", &bsReject,HIGH_PRIORITY,RELIABLE,0,packet->guid,false);
		return;
	}

	byteSystemAddress = (BYTE)packet->guid.systemIndex;
	
	// Add this client to the player pool.
	pPlayerPool->New(byteSystemAddress, szPlayerName);

	// Send this client back an 'InitGame' RPC
	RakNet::BitStream bsInitGame;
	bsInitGame.Write((char*)&pNetGame->m_vecInitPlayerPos, sizeof(VECTOR));
	bsInitGame.Write((char*)&pNetGame->m_vecInitCameraPos, sizeof(VECTOR));
	bsInitGame.Write((char*)&pNetGame->m_vecInitCameraLook, sizeof(VECTOR));
	bsInitGame.Write(pNetGame->m_WorldBounds[0]);
	bsInitGame.Write(pNetGame->m_WorldBounds[1]);
	bsInitGame.Write(pNetGame->m_WorldBounds[2]);
	bsInitGame.Write(pNetGame->m_WorldBounds[3]);
	bsInitGame.Write(pNetGame->m_iSpawnsAvailable);
	bsInitGame.Write(pNetGame->m_byteFriendlyFire);
	bsInitGame.Write(pNetGame->m_byteShowOnRadar);
	bsInitGame.Write(byteSystemAddress);
	pNetGame->GetRPC4()->Call("InitGame", &bsInitGame,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->guid,false);

	// Send this client ServerJoins for every existing player
	RakNet::BitStream pbsExistingClient;

	for(BYTE x = 0; x < MAX_PLAYERS; x++) {
		if((pPlayerPool->GetSlotState(x) == TRUE) && (x != byteSystemAddress)) {
			pbsExistingClient.Reset();
			pbsExistingClient.Write(x);
			pbsExistingClient.Write(strlen(pPlayerPool->GetPlayerName(x)));
			pbsExistingClient.Write(pPlayerPool->GetPlayerName(x),strlen(pPlayerPool->GetPlayerName(x)));
			pNetGame->GetRPC4()->Call("ServerJoin", &pbsExistingClient,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->guid,false);

			// Now also spawn the player for them if they're active.
			CPlayer *pSpawnPlayer = pPlayerPool->GetAt(x);
			if(pSpawnPlayer->IsActive()) {
				pSpawnPlayer->SpawnForPlayer(byteSystemAddress);
			}
		}
	}

	// Spawn all existing vehicles for player.
	CVehicle *pVehicle;

	for(BYTE x = 0; x < MAX_VEHICLES; x++) {
		if(pVehiclePool->GetSlotState(x) == TRUE) {
			pVehicle = pVehiclePool->GetAt(x);
			if(pVehicle->IsActive()) pVehicle->SpawnForPlayer(byteSystemAddress);
		}
	}

	pNetGame->GetGameLogic()->HandleClientJoin(byteSystemAddress);
}

//----------------------------------------------------
// Sent by client with global chat text

void Chat(RakNet::BitStream *bitStream, Packet *packet)
{
	CHAR szText[256];
	BYTE byteTextLen;
	CPlayerPool *pPool = pNetGame->GetPlayerPool();

	bitStream->Read(byteTextLen);
	bitStream->Read(szText,byteTextLen);
	szText[byteTextLen] = '\0';

	if(!pPool->GetSlotState((BYTE)packet->guid.systemIndex)) return;

	logprintf("[%s]: %s",
		pPool->GetPlayerName((BYTE)packet->guid.systemIndex),
		szText);

	pRcon->OutputChatMessage(pPool->GetPlayerName((BYTE)packet->guid.systemIndex),
		szText);

	// Now pass it off to all the other clients.
	RakNet::BitStream bsSend;
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	bsSend.Write(byteSystemAddress);
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);

	pNetGame->GetRPC4()->Call("Chat", &bsSend,HIGH_PRIORITY,RELIABLE,0,packet->guid,true);

	pScripts->onPlayerText(byteSystemAddress, szText);
}

void ChatCommand(RakNet::BitStream *bitStream, Packet *packet)
{
	CHAR szText[256];
	BYTE byteTextLen;
	CPlayerPool *pPool = pNetGame->GetPlayerPool();

	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;

	bitStream->Read(byteTextLen);
	bitStream->Read(szText,byteTextLen);
	szText[byteTextLen] = '\0';

	if(!pPool->GetSlotState((BYTE)packet->guid.systemIndex)) return;

	pScripts->onPlayerCommand(byteSystemAddress, szText);
}


//----------------------------------------------------
// Sent by client who wishes to request a class from
// the gamelogic handler.

void RequestClass(RakNet::BitStream *bitStream, Packet *packet)
{
	
	BYTE byteRequestedClass;
	BYTE byteRequestOutcome = 0;
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	bitStream->Read(byteRequestedClass);

	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;
	
	if(pNetGame->GetGameLogic()->HandleSpawnClassRequest(byteSystemAddress,byteRequestedClass)) 
	{
		byteRequestOutcome = 1;
	}

	RakNet::BitStream bsSpawnRequestReply;
	CPlayer *pPlayer=pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	PLAYER_SPAWN_INFO *SpawnInfo = pPlayer->GetSpawnInfo();

	bsSpawnRequestReply.Write(byteRequestOutcome);
	bsSpawnRequestReply.Write(SpawnInfo->byteTeam);
	bsSpawnRequestReply.Write(SpawnInfo->byteSkin);
	bsSpawnRequestReply.Write((char*)&SpawnInfo->vecPos, sizeof(VECTOR));
	bsSpawnRequestReply.Write(SpawnInfo->fRotation);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeapons[0]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeaponsAmmo[0]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeapons[1]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeaponsAmmo[1]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeapons[2]);
	bsSpawnRequestReply.Write(SpawnInfo->iSpawnWeaponsAmmo[2]);

	pNetGame->GetRPC4()->Call("RequestClass", &bsSpawnRequestReply,HIGH_PRIORITY,RELIABLE,0,packet->guid,false);

	pScripts->onPlayerRequestClass(byteSystemAddress, byteRequestedClass);
}


//----------------------------------------------------
// Sent by client when they're spawning/respawning.

void Spawn(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	pPlayer->Spawn();

	pScripts->onPlayerSpawn(byteSystemAddress);
}

//----------------------------------------------------
// Sent by client when they die.

void Death(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);

	BYTE byteDeathReason;
	BYTE byteWhoWasResponsible;

	bitStream->Read(byteDeathReason);
	bitStream->Read(byteWhoWasResponsible);

	if(pPlayer) {
		pPlayer->HandleDeath(byteDeathReason,byteWhoWasResponsible);
		pScripts->onPlayerDeath(byteSystemAddress, byteDeathReason);
	}
}

//----------------------------------------------------
// Sent by client when they want to enter a
// vehicle gracefully.

void EnterVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	BYTE byteVehicleID;
	BYTE bytePassenger;

	bitStream->Read(byteVehicleID);
	bitStream->Read(bytePassenger);
	pPlayer->EnterVehicle(byteVehicleID,bytePassenger);
	
	pScripts->onPlayerEnterVehicle(byteSystemAddress, byteVehicleID);
	//logprintf("%u enters vehicle %u",byteSystemAddress,byteVehicleID);
}

//----------------------------------------------------
// Sent by client when they want to exit a
// vehicle gracefully.

void ExitVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);
	BYTE byteVehicleID;

	bitStream->Read(byteVehicleID);
	pPlayer->ExitVehicle(byteVehicleID);

	pScripts->onPlayerExitVehicle(byteSystemAddress, byteVehicleID);
	//logprintf("%u exits vehicle %u",byteSystemAddress,byteVehicleID);
}

//----------------------------------------------------
// Sent by client when they want score and ping information.

void UpdateScoreAndPing(RakNet::BitStream *bitStream, Packet *packet)
{
	RakNet::BitStream bsSend;
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	int write_counter = 0;
	SystemAddress systemAddress;

	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			bsSend.Write((BYTE)i);
			bsSend.Write(pPlayerPool->GetScore(i));
			bsSend.Write(pRak->GetLastPing(pRak->GetSystemAddressFromIndex(i)));
			
			if(pPlayerPool->IsAdmin(byteSystemAddress)) {
				systemAddress = pRak->GetSystemAddressFromIndex(i);
				bsSend.Write(systemAddress.binaryAddress);
			} else {
				bsSend.Write((unsigned long)0UL);
			}

			write_counter++;
		}
		i++;
	}

	pNetGame->GetRPC4()->Call("UpdateScoreAndPing", &bsSend, HIGH_PRIORITY, RELIABLE, 0, packet->guid, false);
}

//----------------------------------------------------

void Admin(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;

	char szPassTest[65];
	int iPassLen;

	bitStream->Read(iPassLen);
	if(iPassLen > 64) return;
	bitStream->Read(szPassTest,iPassLen);

	szPassTest[iPassLen] = '\0';
	
	if(!strcmp(szPassTest,szAdminPass)) {
		pPlayerPool->SetAdmin(byteSystemAddress);
	}
}

//----------------------------------------------------

void KickPlayer(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;

	BYTE byteKickPlayer;

	if(pPlayerPool->IsAdmin(byteSystemAddress)) {
		bitStream->Read(byteKickPlayer);	
		pNetGame->KickPlayer(byteKickPlayer);
	}
}

//----------------------------------------------------

void BanIPAddress(RakNet::BitStream *bitStream, Packet *packet)
{
	BYTE byteSystemAddress = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(byteSystemAddress)) return;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	char ban_ip[256];
	int iBanIpLen;

	if(pPlayerPool->IsAdmin(byteSystemAddress)) {
		bitStream->Read(iBanIpLen);
		bitStream->Read(ban_ip,iBanIpLen);
		ban_ip[iBanIpLen] = 0;

		pNetGame->AddBan(ban_ip);
	}
}
//----------------------------------------------------

void RegisterRPCs()
{

	pNetGame->GetRPC4()->RegisterFunction("ClientJoin", ClientJoin);
	pNetGame->GetRPC4()->RegisterFunction("Chat", Chat);
	pNetGame->GetRPC4()->RegisterFunction("ChatCommand", ChatCommand);
	pNetGame->GetRPC4()->RegisterFunction("RequestClass", RequestClass);
	pNetGame->GetRPC4()->RegisterFunction("Spawn", Spawn);
	pNetGame->GetRPC4()->RegisterFunction("Death", Death);
	pNetGame->GetRPC4()->RegisterFunction("EnterVehicle", EnterVehicle);
	pNetGame->GetRPC4()->RegisterFunction("ExitVehicle", ExitVehicle);
	pNetGame->GetRPC4()->RegisterFunction("UpdateScoreAndPing", UpdateScoreAndPing);
	pNetGame->GetRPC4()->RegisterFunction("Admin", Admin);
	pNetGame->GetRPC4()->RegisterFunction("KickPlayer", KickPlayer);
	pNetGame->GetRPC4()->RegisterFunction("BanIPAddress", BanIPAddress);
}

//----------------------------------------------------

void UnRegisterRPCs()
{
	pNetGame->GetRPC4()->UnregisterFunction("ClientJoin");
	pNetGame->GetRPC4()->UnregisterFunction("Chat");
	pNetGame->GetRPC4()->UnregisterFunction("ChatCommand");
	pNetGame->GetRPC4()->UnregisterFunction("RequestClass");
	pNetGame->GetRPC4()->UnregisterFunction("Spawn");
	pNetGame->GetRPC4()->UnregisterFunction("Death");
	pNetGame->GetRPC4()->UnregisterFunction("EnterVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("ExitVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("UpdateScoreAndPing");
	pNetGame->GetRPC4()->UnregisterFunction("Admin");
	pNetGame->GetRPC4()->UnregisterFunction("KickPlayer");
	pNetGame->GetRPC4()->UnregisterFunction("BanIPAddress");
}

//----------------------------------------------------
