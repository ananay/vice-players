//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): adamix
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CChatWindow *pChatWindow;

CObjectPool::CObjectPool()
{
	for(EntityId ObjectID = 0; ObjectID < MAX_OBJECTS; ObjectID++) 
	{
		m_bObjectSlotState[ObjectID] = FALSE;
		m_pObjects[ObjectID] = NULL;
	}
}

CObjectPool::~CObjectPool()
{
	for(EntityId i = 0; i < MAX_OBJECTS; i++)
	{
		Delete(i);
	}
}

BOOL CObjectPool::New(EntityId ObjectID, int iModel, Vector3 vecPos, Vector3 vecRot)
{
	if(m_pObjects[ObjectID])
	{
		Delete(ObjectID);
	}

	m_pObjects[ObjectID] = new CObject(iModel, &vecPos, &vecRot);

	if (m_pObjects[ObjectID])
	{
		m_bObjectSlotState[ObjectID] = TRUE;

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectPool::Delete(EntityId ObjectID)
{
	if(!m_pObjects[ObjectID])
	{
		return FALSE;
	}

	m_bObjectSlotState[ObjectID] = FALSE;
	delete m_pObjects[ObjectID];
	m_pObjects[ObjectID] = NULL;

	return TRUE;
}