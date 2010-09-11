//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

//----------------------------------------------------

#include "main.h"
#include "player.h"
#include "playerpool.h"
#include "vehicle.h"
#include "vehiclepool.h"
#include "object.h"
#include "objectpool.h"
#include "timerpool.h"
#include "text.h"
#include "textpool.h"
#include "../RakNet/MessageIdentifiers.h"
#include "netrpc.h"
#include "spawnselection.h"

//----------------------------------------------------

#define UPDATE_TYPE_NONE	0
#define UPDATE_TYPE_FULL	1
#define UPDATE_TYPE_MINIMAL 2

#define GAMESTATE_STOPPED	0
#define GAMESTATE_RUNNING	1

#define INVALID_ID			0xFF

using namespace RakNet;

//----------------------------------------------------

class CNetGame
{
private:

	CPlayerPool					*m_pPlayerPool;
	CVehiclePool				*m_pVehiclePool;
	CTimerPool					*m_pTimerPool;
	CObjectPool					*m_pObjectPool;
	CTextPool					*m_pTextPool;
	RakPeerInterface			*m_pRakPeer;
	static RPC4					*m_pRPC4;
	CSpawnSelection				*m_pGameLogic;
	int							m_iGameState;
	int							m_iMaxPlayers;
	std::string					m_szHostname;
	bool						m_bPassworded;

	// TODO: Make private
public:
	Vector3						m_vecInitPlayerPos;
	Vector3						m_vecInitCameraPos;
	Vector3						m_vecInitCameraLook;
	float						m_WorldBounds[4]; // pos_x neg_x pos_y neg_y
	BYTE						m_byteFriendlyFire;
	BYTE						m_byteShowOnRadar;
private:
		
	void UpdateNetwork();
	void SetupInitPositions();

public:

	CNetGame(int iMaxPlayers, int iPort, char * szPassword, char * szHostname, BYTE byteFriendlyFire, BYTE byteShowOnRadar);
	~CNetGame();

	int GetGameState() { return m_iGameState; };
	CPlayerPool * GetPlayerPool() { return m_pPlayerPool; };
	CVehiclePool * GetVehiclePool() { return m_pVehiclePool; };
	CObjectPool * GetObjectPool() { return m_pObjectPool; };
	CTextPool * GetTextPool() { return m_pTextPool; };
	CTimerPool * GetTimerPool() { return m_pTimerPool; };
	RakPeerInterface * GetRakPeer() { return m_pRakPeer; };
	RPC4 * GetRPC4() { return m_pRPC4; }
	CSpawnSelection * GetGameLogic() { return m_pGameLogic; };
	int	GetMaxPlayers() { return m_iMaxPlayers; };
	std::string GetHostname() { return m_szHostname; };
	bool IsPassworded() { return m_bPassworded; };
	
	void Process();
	void BroadcastData( BitStream *bitStream, PacketPriority priority,
						PacketReliability reliability,
						char orderingStream,
						EntityId excludedPlayer );

	void PlayerSync(Packet *p);
	void VehicleSync(Packet *p);
	void PassengerSync(Packet *p);
	void KickPlayer(EntityId playerID);
	void AddBan(char * ip_mask);
	void LoadBanList();
};

//----------------------------------------------------
