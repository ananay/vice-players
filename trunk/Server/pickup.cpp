//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): bpeterson
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "netgame.h"
extern CNetGame *pNetGame;

CPickup::CPickup(int iModel, int iType, Vector3 * vecPos)
{
	m_iModel = iModel;
	m_iType = iType;
	memcpy(&m_vecPos, vecPos, sizeof(Vector3));
}

CPickup::~CPickup()
{
	DestroyForWorld();
}

void CPickup::SpawnForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_PickupID);
	bsSend.Write(m_iModel);
	bsSend.Write(m_iType);
	bsSend.Write((char *)&m_vecPos, sizeof(Vector3));
	
	pNetGame->GetRPC4()->Call("PickupSpawn", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
}

void CPickup::DestroyForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_PickupID);

	pNetGame->GetRPC4()->Call("PickupDestroy", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
}

void CPickup::SpawnForWorld()
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(pPlayerPool->GetSlotState(i))
		{
			SpawnForPlayer(i);
		}
	}
}

void CPickup::DestroyForWorld()
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(pPlayerPool->GetSlotState(i))
		{
			DestroyForPlayer(i);
		}
	}
}