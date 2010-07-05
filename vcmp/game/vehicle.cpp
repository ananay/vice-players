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

#include "../../raknet/WindowsIncludes.h"
#include <math.h>
#include <stdio.h>

#include "../main.h"
#include "util.h"

extern CGame		*pGame;
extern CChatWindow  *pChatWindow;

//-----------------------------------------------------------

CVehicle::CVehicle(int iType, float fPosX, float fPosY,
					float fPosZ, float fRotation)
{	
	DWORD dwRetID=0;

	m_dwGTAId = 0;

	if(!pGame->IsModelLoaded(iType)) {
		pGame->RequestModel(iType);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iType)) Sleep(2);
	}

	ScriptCommand(&create_car,iType,fPosX,fPosY,fPosZ,&dwRetID);
	ScriptCommand(&set_car_z_angle,dwRetID,fRotation);

	SetEntity((ENTITY_TYPE*)GamePool_Vehicle_GetAt(dwRetID));
	m_dwGTAId = dwRetID;
	
	VECTOR vPos;
	GetPosition(&vPos);
	vPos.Z = fPosZ;
	SetPosition(vPos);
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	pVehicle->dwDoorsLocked = 0;
	//m_pEntity->byteLockedFlags = 1;
	
	ScriptCommand(&set_car_immunities,m_dwGTAId,1,1,1,1,1);
	m_bIsInvulnerable = TRUE;

	m_bHasBeenDriven = FALSE;
	m_dwTimeSinceLastDriven = GetTickCount();
}

//-----------------------------------------------------------

CVehicle::~CVehicle() 
{
	ScriptCommand(&destroy_car,m_dwGTAId);
}

//-----------------------------------------------------------

VEHICLE_TYPE *CVehicle::GetVehicle()
{
	return (VEHICLE_TYPE *)GetEntity();
}

//-----------------------------------------------------------

void CVehicle::UpdateLastDrivenTime()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) {
		if(pVehicle->pDriver) {
			m_bHasBeenDriven = TRUE;
			m_dwTimeSinceLastDriven = GetTickCount();
		}
	}
}

//-----------------------------------------------------------

void CVehicle::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	VECTOR vPos;
	VECTOR vecMoveSpeed;

	if(!GetEntity()) return;

	GetPosition(&vPos);
	GetMoveSpeed(&vecMoveSpeed);

	if(vPos.X > fPX) // greatest X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = -0.3f;
		}
		SetMoveSpeed(vecMoveSpeed);
		return;
	}
	else if(vPos.X < fZX)  // least X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = 0.3f;
		}
		SetMoveSpeed(vecMoveSpeed);
		return;
	}
	else if(vPos.Y > fPY) // Y coord check
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = -0.3f;
		}

		SetMoveSpeed(vecMoveSpeed);
		return;
	}
	else if(vPos.Y < fNY)
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = 0.3f;
		}

		SetMoveSpeed(vecMoveSpeed);
		return;
	}
}

//-----------------------------------------------------------

BOOL CVehicle::HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	VECTOR vPos;

	if(!GetEntity()) return FALSE;

	GetPosition(&vPos);

	if(vPos.X > fPX) {
		return TRUE;
	}
	else if(vPos.X < fZX) {
		return TRUE;
	}
	else if(vPos.Y > fPY) {
		return TRUE;
	}
	else if(vPos.Y < fNY) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float CVehicle::GetDistanceFromLocalPlayerPed()
{
	VECTOR vThisVehicle;
	VECTOR vFromPlayer;
	float  fSX,fSY;

	CPlayerPed *pLocalPlayerPed = pGame->FindPlayerPed();

	if(!GetEntity()) return 10000.0f; // very far away
	if(!pLocalPlayerPed) return 10000.0f; // very far away
	
	GetPosition(&vThisVehicle);
	pLocalPlayerPed->GetPosition(&vFromPlayer);
	
	fSX = (vThisVehicle.X - vFromPlayer.X) * (vThisVehicle.X - vFromPlayer.X);
	fSY = (vThisVehicle.Y - vFromPlayer.Y) * (vThisVehicle.Y - vFromPlayer.Y);
	
	return (float)sqrt(fSX + fSY);
}

//-----------------------------------------------------------

BOOL CVehicle::IsOkToRespawn()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) {
		if(pVehicle->pDriver) return FALSE;
		if(pVehicle->pPassengers[0]) return FALSE;
		if(pVehicle->pPassengers[1]) return FALSE;
		if(pVehicle->pPassengers[2]) return FALSE;
		if(pVehicle->pPassengers[3]) return FALSE;
		if(pVehicle->pPassengers[4]) return FALSE;
		if(pVehicle->pPassengers[5]) return FALSE;
		if(pVehicle->pPassengers[6]) return FALSE;
		if(pVehicle->pPassengers[7]) return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------

void CVehicle::SetInvulnerable(BOOL bInv)
{
	if(!GetEntity()) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	if(bInv && m_bIsInvulnerable == FALSE) {
		ScriptCommand(&set_car_immunities,m_dwGTAId,1,1,1,1,1);
		m_bIsInvulnerable = TRUE;
	}
	else if(!bInv && m_bIsInvulnerable == TRUE)	{
		ScriptCommand(&set_car_immunities,m_dwGTAId,0,0,0,0,0);
		m_bIsInvulnerable = FALSE;
	}
}
//-----------------------------------------------------------

void CVehicle::SetLockedState(int iLocked)
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();
	if(pPhysical) {
		pPhysical->byteLockedFlags = iLocked;
	}
}

//-----------------------------------------------------------
// Can be used to fix control state of motorbikes
// which have passengers without drivers.

void CVehicle::VerifyControlState()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) {
		if( (!pVehicle->pDriver) &&
			(pVehicle->physical.entity.nControlFlags == 0x2) )
		{
			pVehicle->physical.entity.nControlFlags = 0x22;
		}
	}
}

//-----------------------------------------------------------

float CVehicle::GetHealth()
{	
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) return pVehicle->fHealth;
	else return 0.0f;
}

//-----------------------------------------------------------

void CVehicle::SetHealth(float fHealth)
{	
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) {
		pVehicle->fHealth = fHealth;
	}
}	

//-----------------------------------------------------------

void CVehicle::SetColor(int iColor1, int iColor2)
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle)  {
		pVehicle->byteColors[0] = (BYTE)iColor1;
		pVehicle->byteColors[1] = (BYTE)iColor2;
	}
}

//-----------------------------------------------------------
 
BYTE CVehicle::GetVehicleSubtype()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(!pVehicle) return 0;
	DWORD dwVehicle = (DWORD)pVehicle;

	_asm mov ecx, dwVehicle
	_asm mov edx, [ecx+288]
	_asm mov eax, [edx+204]

	_asm and eax, 0F0000h
	_asm jz ret_car
	_asm sub eax, 10000h
	_asm jz ret_bike
	_asm sub eax, 10000h
	_asm jz ret_heli
	_asm sub eax, 20000h
	_asm jz ret_boat
	_asm sub eax, 40000h
	_asm jz ret_plane

	return 0;

ret_car:	return	VEHICLE_SUBTYPE_CAR;
ret_bike:	return	VEHICLE_SUBTYPE_BIKE;
ret_heli:	return	VEHICLE_SUBTYPE_HELI;
ret_boat:	return	VEHICLE_SUBTYPE_BOAT;
ret_plane:	return	VEHICLE_SUBTYPE_PLANE;
}

//-----------------------------------------------------------

BOOL CVehicle::HasSunk()
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();
	if(pPhysical) {
		if(pPhysical->byteSunkFlags & 0x10) {
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

BOOL CVehicle::IsDriverLocalPlayer()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) {
		if(pVehicle->pDriver == GamePool_FindPlayerPed()) {
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

BYTE CVehicle::GetMaxPassengers()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) {
		return pVehicle->byteMaxPassengers;
	}	
	return 0;
}

//-----------------------------------------------------------

PED_TYPE *CVehicle::GetDriver()
{
	VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)GetEntity();
	if(pVehicle) {
		return pVehicle->pDriver;
	}
	return NULL;
}

//-----------------------------------------------------------

DWORD CVehicle::GetTimeSinceLastDriven()
{
	return m_dwTimeSinceLastDriven;
}

//-----------------------------------------------------------

void CVehicle::SetTimeSinceLastDriven(DWORD dwTime)
{
	m_dwTimeSinceLastDriven = dwTime;
}

//-----------------------------------------------------------

BOOL CVehicle::HasBeenDriven()
{
	return m_bHasBeenDriven;
}

//-----------------------------------------------------------

void CVehicle::SetHasBeenDriven(BOOL bDriven)
{
	m_bHasBeenDriven = bDriven;
}

//-----------------------------------------------------------