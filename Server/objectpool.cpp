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


CObjectPool::CObjectPool()
{
	for(EntityId objectID = 0; objectID < MAX_OBJECTS; objectID++) 
	{
		m_bObjectSlotState[objectID] = FALSE;
		m_pObjects[objectID] = NULL;
	}
}

CObjectPool::~CObjectPool()
{

}


EntityId CObjectPool::New(int iModel, Vector3 * vecPos, Vector3 * vecRot)
{
	EntityId ObjectID;

	for(ObjectID=1; ObjectID <= MAX_OBJECTS; ObjectID++)
	{
		if(m_bObjectSlotState[ObjectID] == FALSE) break;
	}

	if(ObjectID == MAX_OBJECTS) return -1;		

	m_pObjects[ObjectID] = new CObject(iModel,vecPos,vecRot);

	if(m_pObjects[ObjectID])
	{
		m_pObjects[ObjectID]->SetID(ObjectID);
		m_bObjectSlotState[ObjectID] = TRUE;

		return ObjectID;
	}
	return -1;
}

EntityId CObjectPool::GetFreeSlot()
{
	for(EntityId id = 0; id < MAX_OBJECTS; id++)
	{
		if(m_pObjects[id] == NULL)
			return id;
	}
	return false;
}

void CObjectPool::InitForPlayer(EntityId playerId)
{
	for(EntityId ObjectID = 0; ObjectID < MAX_OBJECTS; ObjectID++)
	{
		if(GetSlotState(ObjectID))
		{
			m_pObjects[ObjectID]->SpawnForPlayer(playerId);
		}
	}
}