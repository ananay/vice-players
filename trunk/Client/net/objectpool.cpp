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
//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): adamix
//
//----------------------------------------------------------

#include "../main.h"
#include "../game/pools.h"

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