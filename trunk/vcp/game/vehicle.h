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
#include "physical.h"

//-----------------------------------------------------------

class CVehicle : public CPhysical
{
private:
	DWORD m_dwGTAId;
	BOOL  m_bIsInvulnerable;
	BOOL  m_bDead;

public:
	CVehicle(int iType, float fPosX, float fPosY,
			 float fPosZ, float fRotation = 0.0f);

	~CVehicle();

	VEHICLE_TYPE *GetVehicle();

	float GetDistanceFromLocalPlayerPed();
	void  SetLockedState(int iLocked);

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

	// Added by the VC-Players team.
	void toggleVehicleSiren(int iToggle);


	BYTE  GetEntityFlags();
	void  SetEnitityFlags(BYTE byteEntityFlags);

	void  SetInvulnerable(BOOL bInv);
	BOOL  IsInvulnerable() { return m_bIsInvulnerable; };

	PED_TYPE *GetDriver();

	void  SetImmunities(int iIm1, int iIm2, int iIm3, int iIm4, int iIm5);

	void  SetDead(BOOL bDead);
	BOOL  IsDead();
};

//-----------------------------------------------------------
