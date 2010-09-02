//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#define VCMP_SAFECALL __stdcall

#include "common.h"
#include "address.h"

class CPools
{
public:
	static PED_TYPE *     VCMP_SAFECALL GetPedFromIndex(int iIndex);
	static int            VCMP_SAFECALL GetIndexFromPed(PED_TYPE * pPed);
	static VEHICLE_TYPE * VCMP_SAFECALL GetVehicleFromIndex(int iIndex);
	static int            VCMP_SAFECALL GetIndexFromVehicle(VEHICLE_TYPE * pVehicle);
	static ENTITY_TYPE  * VCMP_SAFECALL GetObjectFromIndex(int iIndex);
};