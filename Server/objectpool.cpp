//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
// License: See LICENSE in root directory
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
	// Loop through all object slots
	for(EntityId i = 0; i < MAX_OBJECTS; i++)
	{
		// Is this object slot used?
		if(m_bObjectSlotState[i])
		{
			// Delete this object
			Delete(i);
		}
	}
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