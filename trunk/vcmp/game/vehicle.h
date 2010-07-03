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
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
//----------------------------------------------------------

#pragma once

#include "game.h"
#include "entity.h"

//-----------------------------------------------------------

class CVehicle : public CEntity
{
private:
	VEHICLE_TYPE	*m_pVehicle;
	DWORD			m_dwGTAId;
	BOOL			m_bIsInvulnerable;
	DWORD			m_dwTimeSinceLastDriven;
	BOOL			m_bHasBeenDriven;

public:
	CVehicle(int iType, float fPosX, float fPosY,
			 float fPosZ, float fRotation = 0.0f);

	~CVehicle();

	VEHICLE_TYPE *GetVehicle();
	void SetVehicle(VEHICLE_TYPE *pVehicle);

	float GetDistanceFromLocalPlayerPed();
	void  SetLockedState(int iLocked);
	void  UpdateLastDrivenTime();

	BYTE  GetVehicleSubtype();

	float GetHealth();
	void  SetHealth(float fHealth);
	void  SetColor(int iColor1, int iColor2);
	void  VerifyControlState();

	BOOL  HasSunk();
	BOOL  IsDriverLocalPlayer();
	void  EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY);
	BOOL  HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY);

	BOOL  GetHandbrakeState();
	void  SetHandbrakeState(BOOL bState);
	BYTE  GetMaxPassengers();

	BYTE  GetEntityFlags();
	void  SetEnitityFlags(BYTE byteEntityFlags);

	void  SetInvulnerable(BOOL bInv);
	BOOL  IsInvulnerable() { return m_bIsInvulnerable; };

	PED_TYPE *GetDriver();

	DWORD GetTimeSinceLastDriven();
	void  SetTimeSinceLastDriven(DWORD dwTime);

	BOOL  HasBeenDriven();
	void  SetHasBeenDriven(BOOL bDriven);

	BOOL IsOkToRespawn();
};

//-----------------------------------------------------------
