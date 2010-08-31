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

#pragma once

#define INVALID_PLAYER_ID 255

//----------------------------------------------------

class CPlayerPool
{
private:
	
	BOOL			m_bPlayerSlotState[MAX_PLAYERS];
	CLocalPlayer  *	m_pLocalPlayer;
	EntityId		m_localPlayerID;
	CRemotePlayer * m_pPlayers[MAX_PLAYERS];
	char			m_szLocalPlayerName[MAX_PLAYER_NAME+1];
	char			m_szPlayerNames[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	ULONG			m_ulIPAddress[MAX_PLAYERS]; // used by admin
	
	int				m_iLocalPlayerScore;
	int				m_iLocalPlayerPing;
	int				m_iScore[MAX_PLAYERS];
	int				m_iPing[MAX_PLAYERS];

public:
	BOOL Process();

	void SetLocalPlayerName(char * szName) { strcpy(m_szLocalPlayerName,szName); };
	char * GetLocalPlayerName() { return m_szLocalPlayerName; };
	char * GetPlayerName(EntityId playerID) { return m_szPlayerNames[playerID]; };

	CLocalPlayer * GetLocalPlayer() { return m_pLocalPlayer; };
	EntityId FindPlayerIDFromGtaPtr(PED_TYPE * pActor);

	BOOL New(EntityId playerID, PCHAR szPlayerName);
	BOOL Delete(EntityId playerID, BYTE byteReason);

	CRemotePlayer* GetAt(EntityId playerID) {
		if(playerID > MAX_PLAYERS) { return NULL; }
		return m_pPlayers[playerID];
	};

	BOOL GetSlotState(EntityId playerID) {
		if(playerID > MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[playerID];
	};
	
	void SetLocalPlayerID(EntityId playerID) {
		strcpy(m_szPlayerNames[playerID],m_szLocalPlayerName);
		m_localPlayerID = playerID;
	};

	void UpdatePing(EntityId playerID, int iPing) { 
		if(playerID == m_localPlayerID) {
			m_iLocalPlayerPing = iPing;
		} else {
			m_iPing[playerID] = iPing;
		}
	};

	void UpdateScore(EntityId playerID, int iScore) { 
		if(playerID == m_localPlayerID) {
			m_iLocalPlayerScore = iScore;
		} else {
			m_iScore[playerID] = iScore;
		}
	};

	void UpdateIPAddress(EntityId playerID, ULONG ip) {
		m_ulIPAddress[playerID] = ip;
	}

	int GetPing(EntityId playerID) { return m_iPing[playerID]; };
	int GetScore(EntityId playerID) { return m_iScore[playerID]; };
	int GetLocalPlayerPing() { return m_iLocalPlayerPing; };
	int GetLocalPlayerScore() { return m_iLocalPlayerScore; };
	EntityId GetLocalPlayerID() { return m_localPlayerID; };
	ULONG GetIPAddress(EntityId playerID) { return m_ulIPAddress[playerID]; };

	CPlayerPool();
	~CPlayerPool();
};

//----------------------------------------------------