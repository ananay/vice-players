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
//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author: kyeman
//
//----------------------------------------------------

#include "../netgame.h"
#include "../player.h"

extern CNetGame *pNetGame;
extern CConfig *pServerConfig;

//----------------------------------------------------
// RETURNS NUMBER OF SPAWNS

int CGameModeGeneric::Init()
{
	int i = 0;
	while(i<MAX_SPAWNS)
	{
		m_AvailableSpawns[i].loaded = false;
		i++;
	}

	return true;
}

//----------------------------------------------------

int CGameModeGeneric::addPlayerClass(int team, int model, float x, float y, float z, float rot, int weapon1, int ammo1, int weapon2, int ammo2, int weapon3, int ammo3)
{
	for(int i = 0; i < MAX_SPAWNS; i++)
	{
		if(m_AvailableSpawns[i].loaded == false)
		{
			m_AvailableSpawns[i].byteTeam = team;
			m_AvailableSpawns[i].byteSkin = model;
			m_AvailableSpawns[i].vecPos.X = x;
			m_AvailableSpawns[i].vecPos.Y = y;
			m_AvailableSpawns[i].vecPos.Z = z;
			m_AvailableSpawns[i].fRotation = rot;
			m_AvailableSpawns[i].iSpawnWeapons[0] = weapon1;
			m_AvailableSpawns[i].iSpawnWeaponsAmmo[0] = ammo1;
			m_AvailableSpawns[i].iSpawnWeapons[1] = weapon2;
			m_AvailableSpawns[i].iSpawnWeaponsAmmo[1] = ammo2;
			m_AvailableSpawns[i].iSpawnWeapons[2] = weapon3;
			m_AvailableSpawns[i].iSpawnWeaponsAmmo[2] = ammo3;
			m_AvailableSpawns[i].loaded = true;
			return i;
		}
	}
	return false;
}

//----------------------------------------------------

void CGameModeGeneric::HandleClientJoin(BYTE byteSystemAddress) 
{
	

}

//----------------------------------------------------

BOOL CGameModeGeneric::HandleSpawnClassRequest(BYTE byteSystemAddress,
											   int iSpawnType)
{
	CPlayer *pPlayer;
	pPlayer = pNetGame->GetPlayerPool()->GetAt(byteSystemAddress);

	if(pPlayer && (iSpawnType <= (pNetGame->m_iSpawnsAvailable - 1))) {

		pPlayer->SetSpawnInfo(
			m_AvailableSpawns[iSpawnType].byteTeam,
			m_AvailableSpawns[iSpawnType].byteSkin,
			&m_AvailableSpawns[iSpawnType].vecPos,
			m_AvailableSpawns[iSpawnType].fRotation,
			m_AvailableSpawns[iSpawnType].iSpawnWeapons[0],
			m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[0],
			m_AvailableSpawns[iSpawnType].iSpawnWeapons[1],
			m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[1],
			m_AvailableSpawns[iSpawnType].iSpawnWeapons[2],
			m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[2]
			);

		return TRUE;
	}
 
	return FALSE;
}

//----------------------------------------------------
// EOF