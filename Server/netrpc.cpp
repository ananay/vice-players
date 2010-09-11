//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#include "netgame.h"
#include "rcon.h"
#include "scripts.h"

using namespace RakNet;

extern CNetGame *pNetGame;
extern char     *szAdminPass;
extern CRcon    *pRcon;
extern CScripts	*pScripts;
#ifndef WIN32
#	define stricmp strcasecmp
#endif

#define NETGAME_VERSION 6

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2

void FilterInvalidNickChars(PCHAR szString);

DWORD dwKeys[128];
DWORD dwLastKeyEvent[2];

//----------------------------------------------------
// Sent by a client who's wishing to join us in our
// multiplayer-like activities.

void ClientJoin(RakNet::BitStream *bitStream, Packet *packet)
{
	RakNet::BitStream bsReject;
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	CHAR szPlayerName[MAX_PLAYER_NAME];
	EntityId playerID;
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

	playerID = (BYTE)packet->guid.systemIndex;
	
	// Add this client to the player pool.
	pPlayerPool->New(playerID, szPlayerName);

	// Send this client back an 'InitGame' RPC
	RakNet::BitStream bsInitGame;
	bsInitGame.Write((char*)&pNetGame->m_vecInitPlayerPos, sizeof(Vector3));
	bsInitGame.Write((char*)&pNetGame->m_vecInitCameraPos, sizeof(Vector3));
	bsInitGame.Write((char*)&pNetGame->m_vecInitCameraLook, sizeof(Vector3));
	bsInitGame.Write(pNetGame->m_WorldBounds[0]);
	bsInitGame.Write(pNetGame->m_WorldBounds[1]);
	bsInitGame.Write(pNetGame->m_WorldBounds[2]);
	bsInitGame.Write(pNetGame->m_WorldBounds[3]);
	bsInitGame.Write(pNetGame->m_byteFriendlyFire);
	bsInitGame.Write(pNetGame->m_byteShowOnRadar);
	bsInitGame.Write(playerID);
	pNetGame->GetRPC4()->Call("InitGame", &bsInitGame,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->guid,false);

	// Send this client ServerJoins for every existing player
	RakNet::BitStream pbsExistingClient;

	for(EntityId x = 0; x < MAX_PLAYERS; x++)
	{
		if(x != playerID && pPlayerPool->GetSlotState(x))
		{
			pbsExistingClient.Reset();
			pbsExistingClient.Write(x);
			size_t sNameLength = strlen(pPlayerPool->GetPlayerName(x));
			pbsExistingClient.Write(sNameLength);
			pbsExistingClient.Write(pPlayerPool->GetPlayerName(x), sNameLength);
			pNetGame->GetRPC4()->Call("ServerJoin", &pbsExistingClient, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false);

			// Now also spawn the player for them if they're active.
			CPlayer * pSpawnPlayer = pPlayerPool->GetAt(x);

			// Is this player spawned?
			if(pSpawnPlayer->IsActive())
			{
				// Spawn this player for the new player
				pSpawnPlayer->SpawnForPlayer(playerID);
			}
		}
	}

	// Spawn all existing vehicles for player.
	CVehicle * pVehicle;

	for(EntityId x = 0; x < MAX_VEHICLES; x++)
	{
		if(pVehiclePool->GetSlotState(x))
		{
			pVehicle = pVehiclePool->GetAt(x);

			// Is this vehicle spawned?
			if(pVehicle->IsActive())
			{
				// Spawn this for the new player
				pVehicle->SpawnForPlayer(playerID);
			}
		}
	}

	// Spawn all objects for the player
	pNetGame->GetObjectPool()->InitForPlayer(playerID);

	// Create all texts for player
	pNetGame->GetTextPool()->InitForPlayer(playerID);
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

	if(!pScripts->onPlayerText((BYTE)packet->guid.systemIndex, szText))
		return;
	
	logprintf("[%s]: %s",
		pPool->GetPlayerName((BYTE)packet->guid.systemIndex),
		szText);

	pRcon->OutputChatMessage(pPool->GetPlayerName((BYTE)packet->guid.systemIndex),
		szText);

	// Now pass it off to all the other clients.
	RakNet::BitStream bsSend;
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	bsSend.Write(playerID);
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);

	pNetGame->GetRPC4()->Call("Chat", &bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,true);
}

//----------------------------------------------------
// Sent by client with command text

void ChatCommand(RakNet::BitStream *bitStream, Packet *packet)
{
	CHAR szText[256];
	BYTE byteTextLen;
	CPlayerPool *pPool = pNetGame->GetPlayerPool();

	EntityId playerID = (BYTE)packet->guid.systemIndex;

	bitStream->Read(byteTextLen);
	bitStream->Read(szText,byteTextLen);
	szText[byteTextLen] = '\0';

	if(!pPool->GetSlotState((BYTE)packet->guid.systemIndex)) return;

	pScripts->onPlayerCommand(playerID, szText);
}


//----------------------------------------------------
// Sent by client who wishes to request a class from
// the gamelogic handler.

void RequestClass(RakNet::BitStream *bitStream, Packet *packet)
{
	// TODO: Make this cancelable sometime
	BYTE byteOutcome = 1;
	int iRequestedClass;
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	bitStream->Read(iRequestedClass);

	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;
	
	iRequestedClass = pNetGame->GetGameLogic()->HandleSpawnClassRequest(playerID, iRequestedClass);

	RakNet::BitStream bsSpawnRequestReply;
	CPlayer * pPlayer = pNetGame->GetPlayerPool()->GetAt(playerID);
	PLAYER_SPAWN_INFO * SpawnInfo = pPlayer->GetSpawnInfo();

	// TODO: This call should decide the outcome
	pScripts->onPlayerRequestClass(playerID, iRequestedClass);

	bsSpawnRequestReply.Write(byteOutcome);
	if(byteOutcome) {
		bsSpawnRequestReply.Write(iRequestedClass);
		bsSpawnRequestReply.Write((char *)SpawnInfo, sizeof(PLAYER_SPAWN_INFO));

		pNetGame->GetRPC4()->Call("RequestClass", &bsSpawnRequestReply ,HIGH_PRIORITY, RELIABLE, 0, packet->guid, false);
	}
}

//----------------------------------------------------
// Sent by client when they're spawning/respawning.

void Spawn(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(playerID);
	pPlayer->Spawn();

	pScripts->onPlayerSpawn(playerID);
}

//----------------------------------------------------
// Sent by client when they die.

void Death(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(playerID);

	BYTE byteDeathReason;
	BYTE byteWhoWasResponsible;

	bitStream->Read(byteDeathReason);
	bitStream->Read(byteWhoWasResponsible);

	if(pPlayer) {
		pPlayer->HandleDeath(byteDeathReason,byteWhoWasResponsible);
		pScripts->onPlayerDeath(playerID, byteWhoWasResponsible, byteDeathReason);
	}
}

//----------------------------------------------------
// Sent by client when a vehicle is dead.

void VehicleDeath(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;

	EntityId vehicleID;

	bitStream->Read(vehicleID);

	CVehiclePool * pVehiclePool = pNetGame->GetVehiclePool();
	CVehicle * pVehicle = pVehiclePool->GetAt(vehicleID);
	if(pVehicle) {
		pVehiclePool->FlagForRespawn(vehicleID);

		pScripts->onVehicleDeath(vehicleID);
	}
}

//----------------------------------------------------
// Sent by client when they want to enter a
// vehicle gracefully.

void EnterVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(playerID);
	EntityId vehicleID;
	BYTE bytePassenger;

	bitStream->Read(vehicleID);
	bitStream->Read(bytePassenger);
	pPlayer->EnterVehicle(vehicleID,bytePassenger);
	
	pScripts->onPlayerEnterVehicle(playerID, vehicleID);
	logprintf("%u enters vehicle %u",playerID,vehicleID);
}

//----------------------------------------------------
// Sent by client when they want to exit a
// vehicle gracefully.

void ExitVehicle(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;
	CPlayer	*pPlayer = pNetGame->GetPlayerPool()->GetAt(playerID);
	EntityId vehicleID;

	bitStream->Read(vehicleID);
	pPlayer->ExitVehicle(vehicleID);

	pScripts->onPlayerExitVehicle(playerID, vehicleID);
	//logprintf("%u exits vehicle %u",playerID,vehicleID);
}

//----------------------------------------------------
// Sent by client when they want score and ping information.

void UpdateScoreAndPing(RakNet::BitStream *bitStream, Packet *packet)
{
	RakNet::BitStream bsSend;
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	int write_counter = 0;
	SystemAddress systemAddress;
	RakPeerInterface * pRak = pNetGame->GetRakPeer();

	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			bsSend.Write((BYTE)i);
			bsSend.Write(pPlayerPool->GetScore(i));
			bsSend.Write(pRak->GetLastPing(pRak->GetSystemAddressFromIndex(i)));
			
			if(pPlayerPool->IsAdmin(playerID)) {
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
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;

	char szPassTest[65];
	int iPassLen;

	bitStream->Read(iPassLen);
	if(iPassLen > 64) return;
	bitStream->Read(szPassTest,iPassLen);

	szPassTest[iPassLen] = '\0';
	
	if(!strcmp(szPassTest,szAdminPass)) {
		pPlayerPool->SetAdmin(playerID);
	}
}

//----------------------------------------------------

void KickPlayer(RakNet::BitStream *bitStream, Packet *packet)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;

	BYTE byteKickPlayer;

	if(pPlayerPool->IsAdmin(playerID)) {
		bitStream->Read(byteKickPlayer);	
		pNetGame->KickPlayer(byteKickPlayer);
	}
}

//----------------------------------------------------

void BanIPAddress(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	char ban_ip[256];
	int iBanIpLen;

	if(pPlayerPool->IsAdmin(playerID)) {
		bitStream->Read(iBanIpLen);
		bitStream->Read(ban_ip,iBanIpLen);
		ban_ip[iBanIpLen] = 0;

		pNetGame->AddBan(ban_ip);
	}
}

void KeyEvent(RakNet::BitStream *bitStream, Packet *packet)
{
	EntityId playerID = (BYTE)packet->guid.systemIndex;
	if(!pNetGame->GetPlayerPool()->GetSlotState(playerID)) return;

	BYTE type;

	DWORD dwKey;
	bool state;

	bitStream->Read(dwKey);
	state = bitStream->ReadBit();
	
	if(dwKeys[dwKey] != state)
	{
		if(state == true)
			type = 0;
		else
			type = 2;
	}
	else
	{
		type = 1;
	}

	dwLastKeyEvent[0] = dwKey;
	dwLastKeyEvent[1] = state;
	dwKeys[dwKey] = state;

	pScripts->onPlayerKeyEvent(playerID, type, (char*)&dwKey);
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
	pNetGame->GetRPC4()->RegisterFunction("VehicleDeath", VehicleDeath);
	pNetGame->GetRPC4()->RegisterFunction("EnterVehicle", EnterVehicle);
	pNetGame->GetRPC4()->RegisterFunction("ExitVehicle", ExitVehicle);
	pNetGame->GetRPC4()->RegisterFunction("UpdateScoreAndPing", UpdateScoreAndPing);
	pNetGame->GetRPC4()->RegisterFunction("Admin", Admin);
	pNetGame->GetRPC4()->RegisterFunction("KickPlayer", KickPlayer);
	pNetGame->GetRPC4()->RegisterFunction("BanIPAddress", BanIPAddress);
	pNetGame->GetRPC4()->RegisterFunction("KeyEvent", KeyEvent);
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
	pNetGame->GetRPC4()->UnregisterFunction("VehicleDeath");
	pNetGame->GetRPC4()->UnregisterFunction("EnterVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("ExitVehicle");
	pNetGame->GetRPC4()->UnregisterFunction("UpdateScoreAndPing");
	pNetGame->GetRPC4()->UnregisterFunction("Admin");
	pNetGame->GetRPC4()->UnregisterFunction("KickPlayer");
	pNetGame->GetRPC4()->UnregisterFunction("BanIPAddress");
	pNetGame->GetRPC4()->UnregisterFunction("KeyEvent");
}

//----------------------------------------------------
