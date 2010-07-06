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

#include "../main.h"

#define MAX_SPAWNS 100

//----------------------------------------------------

class CGameModeGeneric
{
private:
	PLAYER_SPAWN_INFO	m_AvailableSpawns[MAX_SPAWNS];
	int					m_iAvailableSpawnCount;
public:

	int AddPlayerClass(int team, int model, float x, float y, float z, float rot, int weapon1, int ammo1, int weapon2, int ammo2, int weapon3, int ammo3);
	int HandleSpawnClassRequest(BYTE byteSystemAddress, int iSpawnType);
	
	CGameModeGeneric();
	~CGameModeGeneric();
};


//----------------------------------------------------
// EOF