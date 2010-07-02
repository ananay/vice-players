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
	
	BOOL m_bPlayerSlotState[MAX_PLAYERS];
	CLocalPlayer  *m_pLocalPlayer;
	BYTE	m_byteLocalSystemAddress;
	CRemotePlayer *m_pPlayers[MAX_PLAYERS];
	CHAR	m_szLocalPlayerName[MAX_PLAYER_NAME+1];
	CHAR	m_szPlayerNames[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	ULONG	m_ulIPAddress[MAX_PLAYERS]; // used by admin
	
	int		m_iLocalPlayerScore;
	int		m_iLocalPlayerPing;
	int		m_iScore[MAX_PLAYERS];
	int		m_iPing[MAX_PLAYERS];

public:
	BOOL Process();

	void SetLocalPlayerName(PCHAR szName) { strcpy(m_szLocalPlayerName,szName); };
	PCHAR GetLocalPlayerName() { return m_szLocalPlayerName; };
	PCHAR GetPlayerName(BYTE byteSystemAddress) { return m_szPlayerNames[byteSystemAddress]; };

	CLocalPlayer * GetLocalPlayer() { return m_pLocalPlayer; };
	BYTE FindRemoteSystemAddressFromGtaPtr(PED_TYPE * pActor);

	BOOL New(BYTE byteSystemAddress, PCHAR szPlayerName);
	BOOL Delete(BYTE byteSystemAddress, BYTE byteReason);

	CRemotePlayer* GetAt(BYTE byteSystemAddress) {
		if(byteSystemAddress > MAX_PLAYERS) { return NULL; }
		return m_pPlayers[byteSystemAddress];
	};

	BOOL GetSlotState(BYTE byteSystemAddress) {
		if(byteSystemAddress > MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[byteSystemAddress];
	};
	
	void SetLocalSystemAddress(BYTE byteID) {
		strcpy(m_szPlayerNames[byteID],m_szLocalPlayerName);
		m_byteLocalSystemAddress = byteID;
	};

	void UpdatePing(BYTE byteSystemAddress,int iPing) { 
		if(byteSystemAddress == m_byteLocalSystemAddress) {
			m_iLocalPlayerPing = iPing;
		} else {
			m_iPing[byteSystemAddress] = iPing;
		}
	};

	void UpdateScore(BYTE byteSystemAddress,int iScore) { 
		if(byteSystemAddress == m_byteLocalSystemAddress) {
			m_iLocalPlayerScore = iScore;
		} else {
			m_iScore[byteSystemAddress] = iScore;
		}
	};

	void UpdateIPAddress(BYTE byteSystemAddress, ULONG ip) {
		m_ulIPAddress[byteSystemAddress] = ip;
	}

	int GetPing(BYTE byteSystemAddress) { return m_iPing[byteSystemAddress]; };
	int GetScore(BYTE byteSystemAddress) { return m_iScore[byteSystemAddress]; };
	int GetLocalPlayerPing() { return m_iLocalPlayerPing; };
	int GetLocalPlayerScore() { return m_iLocalPlayerScore; };
	BYTE GetLocalSystemAddress() { return m_byteLocalSystemAddress; };
	ULONG GetIPAddress(BYTE byteSystemAddress) { return m_ulIPAddress[byteSystemAddress]; };

	CPlayerPool();
	~CPlayerPool();
};

//----------------------------------------------------