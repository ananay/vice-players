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
// File Author(s): adamix
//
//-----------------------------------------------------
#include "netgame.h"
extern CNetGame *pNetGame;

CObject::CObject(int iModel, Vector3 * vecPos, Vector3 * vecRot)
{
	memcpy(&m_vecPos, vecPos, sizeof(Vector3));
	memcpy(&m_vecRot, vecRot, sizeof(Vector3));

	m_iModel = iModel;
}

CObject::~CObject()
{

}

void CObject::SpawnForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_ObjectID);
	bsSend.Write(m_iModel);
	bsSend.Write(m_vecPos);
	bsSend.Write(m_vecRot);
	
	pNetGame->GetRPC4()->Call("ObjectSpawn", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
}

void CObject::SpawnForWorld()
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			SpawnForPlayer(i);
		}
	}
}

