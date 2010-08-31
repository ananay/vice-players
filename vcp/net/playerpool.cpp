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

#include "../main.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

char szQuitReasons[][32] = {
"Timeout",
"Leaving",
"Kicked"
};

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	m_pLocalPlayer = new CLocalPlayer();

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		m_bPlayerSlotState[i] = FALSE;
		m_pPlayers[i] = NULL;
	}
}

//----------------------------------------------------

CPlayerPool::~CPlayerPool()
{
	delete m_pLocalPlayer;

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		Delete(i, 0);
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(EntityId playerID, PCHAR szPlayerName)
{
	m_pPlayers[playerID] = new CRemotePlayer();

	if(m_pPlayers[playerID])
	{
		strcpy(m_szPlayerNames[playerID],szPlayerName);
		m_pPlayers[playerID]->SetID(playerID);
		m_bPlayerSlotState[playerID] = TRUE;

		m_iScore[playerID] = 0;
		m_iPing[playerID] = 0;
		m_ulIPAddress[playerID] = 0;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::Delete(EntityId playerID, BYTE byteReason)
{
	if(!GetSlotState(playerID) || !m_pPlayers[playerID]) {
		return FALSE;
	}

	m_bPlayerSlotState[playerID] = FALSE;
	delete m_pPlayers[playerID];
	m_pPlayers[playerID] = NULL;


	return TRUE;
}

//----------------------------------------------------

BOOL CPlayerPool::Process()
{
	m_pLocalPlayer->Process();

	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(m_bPlayerSlotState[i]) {
			m_pPlayers[i]->Process();
		}
	}

	return TRUE;
}

//----------------------------------------------------

EntityId CPlayerPool::FindPlayerIDFromGtaPtr(PED_TYPE * pActor)
{
	CPlayerPed *pPlayerPed;

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(m_bPlayerSlotState[i])
		{
			pPlayerPed = m_pPlayers[i]->GetPlayerPed();

			if(pPlayerPed)
			{
				PED_TYPE * pTestActor = pPlayerPed->GetPed();

				if((pTestActor != NULL) && (pActor == pTestActor)) // found it
				{
					return m_pPlayers[i]->GetID();
				}
			}
		}
	}

	return INVALID_PLAYER_ID;
}
	
//----------------------------------------------------