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
// File Author(s): kyeman
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
	RakPeerInterface			*m_pRakPeer;
	static RPC4					*m_pRPC4;
	CSpawnSelection			*m_pGameLogic;
	int							m_iGameState;
	int							m_iMaxPlayers;

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

	CNetGame(int iMaxPlayers,int iPort,int iGameType,
		char *szPassword,char *szGameFile,
		BYTE byteFriendlyFire,BYTE byteShowOnRadar);

	~CNetGame();

	int GetGameState() { return m_iGameState; };
	CPlayerPool * GetPlayerPool() { return m_pPlayerPool; };
	CVehiclePool * GetVehiclePool() { return m_pVehiclePool; };
	CObjectPool * GetObjectPool() { return m_pObjectPool; };
	CTimerPool * GetTimerPool() { return m_pTimerPool; };
	RakPeerInterface * GetRakPeer() { return m_pRakPeer; };
	RPC4 * GetRPC4() { return m_pRPC4; }
	CSpawnSelection * GetGameLogic() { return m_pGameLogic; };
	int	GetMaxPlayers() { return m_iMaxPlayers; };
	
	void Process();
	void BroadcastData( BitStream *bitStream, PacketPriority priority,
						PacketReliability reliability,
						char orderingStream,
						BYTE byteExcludedPlayer );

	void PlayerSync(Packet *p);
	void VehicleSync(Packet *p);
	void PassengerSync(Packet *p);
	void KickPlayer(BYTE byteKickPlayer);
	void AddBan(char * ip_mask);
	void LoadBanList();
};

//----------------------------------------------------
