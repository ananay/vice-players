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
// File Author: kyeman
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
	BYTE byteSystemAddress = 0;

	m_pLocalPlayer = new CLocalPlayer();

	while(byteSystemAddress < MAX_PLAYERS) {
		m_bPlayerSlotState[byteSystemAddress] = FALSE;
		m_pPlayers[byteSystemAddress] = NULL;
		byteSystemAddress++;
	}
}

//----------------------------------------------------

CPlayerPool::~CPlayerPool()
{	
	BYTE byteSystemAddress = 0;

	delete m_pLocalPlayer;

	while(byteSystemAddress < MAX_PLAYERS) {
		Delete(byteSystemAddress,0);
		byteSystemAddress++;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(BYTE byteSystemAddress, PCHAR szPlayerName)
{
	m_pPlayers[byteSystemAddress] = new CRemotePlayer();

	if(m_pPlayers[byteSystemAddress])
	{
		strcpy(m_szPlayerNames[byteSystemAddress],szPlayerName);
		m_pPlayers[byteSystemAddress]->SetID(byteSystemAddress);
		m_bPlayerSlotState[byteSystemAddress] = TRUE;

		m_iScore[byteSystemAddress] = 0;
		m_iPing[byteSystemAddress] = 0;
		m_ulIPAddress[byteSystemAddress] = 0;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::Delete(BYTE byteSystemAddress, BYTE byteReason)
{
	if(!GetSlotState(byteSystemAddress) || !m_pPlayers[byteSystemAddress]) {
		return FALSE;
	}

	m_bPlayerSlotState[byteSystemAddress] = FALSE;
	delete m_pPlayers[byteSystemAddress];
	m_pPlayers[byteSystemAddress] = NULL;


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

BYTE CPlayerPool::FindRemoteSystemAddressFromGtaPtr(PED_TYPE * pActor)
{
	CPlayerPed *pPlayerPed;

	BYTE byteSystemAddress = 0;

	while(byteSystemAddress < MAX_PLAYERS)  {

		if(TRUE == m_bPlayerSlotState[byteSystemAddress]) {
			pPlayerPed = m_pPlayers[byteSystemAddress]->GetPlayerPed();

			if(pPlayerPed) {
				PED_TYPE *pTestActor = pPlayerPed->GetPed();
				if((pTestActor != NULL) && (pActor == pTestActor)) // found it
					return (BYTE)m_pPlayers[byteSystemAddress]->GetID();
			}
		}

		byteSystemAddress++;
	}

	return INVALID_PLAYER_ID;	
}
	
//----------------------------------------------------