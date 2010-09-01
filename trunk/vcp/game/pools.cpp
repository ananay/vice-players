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
// File Author(s): jenksta
//
//----------------------------------------------------------

#include "pools.h"

PED_TYPE * VCMP_SAFECALL CPools::GetPedFromIndex(int iIndex)
{
	PED_TYPE * pPed;

	DWORD dwFunc = FUNC_CPool_CPed_AtHandle;
	_asm
	{
		mov ebx, VAR_PedPool
		mov ecx, [ebx]
		push iIndex
		call dwFunc
		mov pPed, eax
	}

	return pPed;	
}

int VCMP_SAFECALL CPools::GetIndexFromPed(PED_TYPE * pPed)
{
	int iIndex;

	DWORD dwFunc = FUNC_CPool_CPed__HandleOf;
	_asm
	{
		mov ebx, VAR_PedPool
		mov ecx, [ebx]
		push pPed
		call dwFunc
		mov iIndex, eax
	}

	return iIndex;
}

VEHICLE_TYPE * VCMP_SAFECALL CPools::GetVehicleFromIndex(int iIndex)
{
	VEHICLE_TYPE * pVehicle;

	DWORD dwFunc = FUNC_CPool_CVehicle_AtHandle;
	_asm
	{
		mov ebx, VAR_VehiclePool
		mov ecx, [ebx]
		push iIndex
		call dwFunc
		mov pVehicle, eax
	}

	return pVehicle;
}

int VCMP_SAFECALL CPools::GetIndexFromVehicle(VEHICLE_TYPE * pVehicle)
{
	int iIndex;

	DWORD dwFunc = FUNC_CPool_CVehicle__HandleOf;
	_asm
	{
		mov ebx, VAR_VehiclePool
		mov ecx, [ebx]
		push pVehicle
		call dwFunc
		mov iIndex, eax
	}

	return iIndex;	
}

ENTITY_TYPE * VCMP_SAFECALL CPools::GetObjectFromIndex(int iIndex)
{	
	ENTITY_TYPE *pObject;

	DWORD dwFunc = FUNC_CPool_CObject__AtHandle;

	_asm
	{
		mov ebx, VAR_ObjectPool
		mov ecx, [ebx]
		push iIndex
		call dwFunc
		mov pObject, eax
	}

	return pObject;
}