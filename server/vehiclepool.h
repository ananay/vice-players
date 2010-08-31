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

//----------------------------------------------------

class CVehiclePool
{
private:
	
	BOOL m_bVehicleSlotState[MAX_VEHICLES];
	BYTE GetFreeSlot();
	CVehicle *m_pVehicles[MAX_VEHICLES];
	int m_iRespawnDelay[MAX_VEHICLES];

public:
	
	CVehiclePool();
	~CVehiclePool();

	BOOL New(EntityId vehicleID, BYTE byteVehicleType, Vector3 * vecPos, float fRotation, int iColor1, int iColor2);
	BYTE New(BYTE byteVehicleType, Vector3 * vecPos, float fRotation, int iColor1, int iColor2);
	BOOL Delete(EntityId vehicleID);	
		
	// Retrieve a vehicle by id
	CVehicle* GetAt(EntityId vehicleID) {
		if(vehicleID > MAX_VEHICLES) { return NULL; }
		return m_pVehicles[vehicleID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(EntityId vehicleID) {
		if(vehicleID > MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[vehicleID];
	};

	void FlagForRespawn(EntityId vehicleID);
	void Process();
};

//----------------------------------------------------