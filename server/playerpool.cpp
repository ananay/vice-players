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

#include "netgame.h"
#include "rcon.h"
#include "scripts.h"

extern CNetGame *pNetGame;
extern CRcon	*pRcon;
extern CScripts	*pScripts;

#ifndef WIN32
#	define stricmp strcasecmp
#endif

//----------------------------------------------------

using namespace RakNet;

//----------------------------------------------------

CPlayerPool::CPlayerPool()
{
	BYTE byteSystemAddress = 0;
	while(byteSystemAddress != MAX_PLAYERS) {
		m_bPlayerSlotState[byteSystemAddress] = FALSE;
		m_pPlayers[byteSystemAddress] = NULL;
		byteSystemAddress++;
	}
}

//----------------------------------------------------

CPlayerPool::~CPlayerPool()
{	
	BYTE byteSystemAddress = 0;
	while(byteSystemAddress != MAX_PLAYERS) {
		Delete(byteSystemAddress,0);
		byteSystemAddress++;
	}
}

//----------------------------------------------------

BOOL CPlayerPool::New(BYTE byteSystemAddress, PCHAR szPlayerName)
{
	m_pPlayers[byteSystemAddress] = new CPlayer();

	if(m_pPlayers[byteSystemAddress])
	{
		strcpy(m_szPlayerName[byteSystemAddress],szPlayerName);
		m_pPlayers[byteSystemAddress]->SetID(byteSystemAddress);
		m_bPlayerSlotState[byteSystemAddress] = TRUE;
		m_iPlayerScore[byteSystemAddress] = 0;
		m_bIsAnAdmin[byteSystemAddress] = FALSE;

		// Notify all the other players of a newcommer with
		// a 'ServerJoin' join RPC 
		RakNet::BitStream bsSend;
		bsSend.Write(byteSystemAddress);
		bsSend.Write(strlen(szPlayerName));
		bsSend.Write(szPlayerName,strlen(szPlayerName));
		pNetGame->GetRPC4()->Call("ServerJoin", &bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(byteSystemAddress),true);

		pRcon->ConsolePrintf("[join] %u %s",byteSystemAddress,szPlayerName);

		logprintf("[join] %u %s",byteSystemAddress,szPlayerName);

		pScripts->onPlayerConnect(byteSystemAddress);

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
	if(!GetSlotState(byteSystemAddress) || !m_pPlayers[byteSystemAddress])
	{
		return FALSE;
	}

	m_bPlayerSlotState[byteSystemAddress] = FALSE;
	delete m_pPlayers[byteSystemAddress];
	m_pPlayers[byteSystemAddress] = NULL;
	
	// Notify all the other players that this client is quiting.
	RakNet::BitStream bsSend;
	bsSend.Write(byteSystemAddress);
	bsSend.Write(byteReason);
	pNetGame->GetRPC4()->Call("ServerQuit", &bsSend,HIGH_PRIORITY,RELIABLE_ORDERED,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(byteSystemAddress),true);

	pRcon->ConsolePrintf("[part] %u %s %u",byteSystemAddress,m_szPlayerName[byteSystemAddress],byteReason);

	pScripts->onPlayerDisconnect(byteSystemAddress, byteReason);

	logprintf("[part] %u %s %u",byteSystemAddress,m_szPlayerName[byteSystemAddress],byteReason);
	
	return TRUE;
}

//----------------------------------------------------

void CPlayerPool::Process()
{
	BYTE byteSystemAddress = 0;

	while(byteSystemAddress != MAX_PLAYERS) {
		if(TRUE == m_bPlayerSlotState[byteSystemAddress]) {
			m_pPlayers[byteSystemAddress]->Process();
		}
		byteSystemAddress++;
	}
}

//----------------------------------------------------
// Add information to the pool about a kill.
// Return constant describing the scoring.

BYTE CPlayerPool::AddResponsibleDeath(BYTE byteWhoKilled, BYTE byteWhoDied)
{
	CPlayer *pWhoKilled;
	CPlayer *pWhoDied;

	if( byteWhoKilled != INVALID_PLAYER_ID &&
		byteWhoKilled < MAX_PLAYERS &&
		byteWhoDied < MAX_PLAYERS ) {

		if(m_bPlayerSlotState[byteWhoKilled] && m_bPlayerSlotState[byteWhoDied]) {

			pWhoKilled = m_pPlayers[byteWhoKilled];
			pWhoDied = m_pPlayers[byteWhoDied];

			if( (pWhoKilled->GetTeam() == 255) ||
				(pWhoKilled->GetTeam() != pWhoDied->GetTeam()) )
			{
				m_iPlayerScore[byteWhoKilled]++;

				pRcon->OutputDeathMessage(VALID_KILL,byteWhoDied,byteWhoKilled);

				return VALID_KILL;
			}
			else {
				m_iPlayerScore[byteWhoKilled]--;

				pRcon->OutputDeathMessage(TEAM_KILL,byteWhoDied,byteWhoKilled);

				return TEAM_KILL;
			}
		}
		return SELF_KILL;
	}

	if(byteWhoKilled == INVALID_PLAYER_ID && byteWhoDied < MAX_PLAYERS)
	{
		if(m_bPlayerSlotState[byteWhoDied]) {

			pRcon->OutputDeathMessage(SELF_KILL,byteWhoDied,0);

			return SELF_KILL;
		}
	}

	pRcon->OutputDeathMessage(SELF_KILL,byteWhoDied,0);
	
	return SELF_KILL;						
}

//----------------------------------------------------

float CPlayerPool::GetDistanceFromPlayerToPlayer(BYTE bytePlayer1, BYTE bytePlayer2)
{
	Vector3	*vecFromPlayer;
	Vector3	*vecThisPlayer;
	float	fSX,fSY;

	CPlayer * pPlayer1 = GetAt(bytePlayer1);
	CPlayer * pPlayer2 = GetAt(bytePlayer2);

	vecFromPlayer = &pPlayer1->m_vecPos;
	vecThisPlayer = &pPlayer2->m_vecPos;
	
	fSX = (vecThisPlayer->X - vecFromPlayer->X) * (vecThisPlayer->X - vecFromPlayer->X);
	fSY = (vecThisPlayer->Y - vecFromPlayer->Y) * (vecThisPlayer->Y - vecFromPlayer->Y);
	
	return (float)sqrt(fSX + fSY);	
}

//----------------------------------------------------

BOOL CPlayerPool::IsNickInUse(PCHAR szNick)
{
	int x=0;
	while(x!=MAX_PLAYERS) {
		if(GetSlotState((BYTE)x)) {
#pragma warning(disable:4996)
			if(!stricmp(GetPlayerName((BYTE)x),szNick)) {
				return TRUE;
			}
		}
		x++;
	}
	return FALSE;
}

//----------------------------------------------------

BOOL CPlayerPool::IsConnected(BYTE byteSystemAddress)
{
	if(!GetSlotState(byteSystemAddress) || !m_pPlayers[byteSystemAddress])
	{
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------

void CPlayerPool::SetGameTime(BYTE hours, BYTE minutes)
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			pPlayerPool->GetAt(i)->SetGameTime(hours, minutes);
		}
	}
}

//----------------------------------------------------