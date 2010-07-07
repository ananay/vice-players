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
//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: Christopher, adamix
//
//-----------------------------------------------------

#include "sq_vehicle_natives.h"
#include "netgame.h"

extern CNetGame *pNetGame;

using namespace RakNet;

// Vehicle natives //

// createVehicle
SQInteger sq_createVehicle(SQVM * pVM)
{
	SQInteger byteVehicleType;
	VECTOR pos;
	float fRotation;
	SQInteger iColor1, iColor2;
	sq_getinteger(pVM, -7, &byteVehicleType);
	sq_getfloat(pVM, -6, &pos.X);
	sq_getfloat(pVM, -5, &pos.Y);
	sq_getfloat(pVM, -4, &pos.Z);
	sq_getfloat(pVM, -3, &fRotation);
	sq_getinteger(pVM, -2, &iColor1);
	sq_getinteger(pVM, -1, &iColor2);
	int vehID = pNetGame->GetVehiclePool()->New(byteVehicleType, &pos, fRotation, iColor1, iColor2);
	CVehicle *pVehicle = pNetGame->GetVehiclePool()->GetAt(vehID);
	pVehicle->SpawnForWorld();
	sq_pushinteger(pVM, vehID);
	return 1;
}

// destroyVehicle
SQInteger sq_destroyVehicle(SQVM * pVM)
{
	SQInteger byteVehicle;
	sq_getinteger(pVM, -1, &byteVehicle);
	if(pNetGame->GetVehiclePool()->GetSlotState(byteVehicle))
	{
		pNetGame->GetVehiclePool()->Delete(byteVehicle);
		sq_pushbool(pVM, true);

		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleHealth
SQInteger sq_setVehicleHealth(SQVM * pVM)
{
	SQInteger vehicle;
	float newHealthValue;

	sq_getinteger(pVM, -2, &vehicle);
    sq_getfloat(pVM, -1, &newHealthValue);
	if(pNetGame->GetVehiclePool()->GetSlotState(vehicle))
	{
		pNetGame->GetVehiclePool()->GetAt(vehicle)->SetHealth(newHealthValue);

		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_getVehicleHealth(SQVM * pVM)
{
	SQInteger vehicle;

	sq_getinteger(pVM, -1, &vehicle);

	if(pNetGame->GetVehiclePool()->GetSlotState(vehicle))
	{
		float fHealth = pNetGame->GetVehiclePool()->GetAt(vehicle)->GetHealth();

		sq_pushfloat(pVM, fHealth);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// setVehicleColor
SQInteger sq_setVehicleColor(SQVM * pVM)
{
	SQInteger vehicle, color1, color2;

	sq_getinteger(pVM, -3, &vehicle);
    sq_getinteger(pVM, -2, &color1);
	sq_getinteger(pVM, -1, &color2);
	if(pNetGame->GetVehiclePool()->GetSlotState(vehicle))
	{
		pNetGame->GetVehiclePool()->GetAt(vehicle)->SetColor(color1, color2);

		sq_pushbool(pVM, true);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_getVehicleColors(SQVM * pVM)
{
	SQInteger vehicle;

	sq_getinteger(pVM, -1, &vehicle);

	if(pNetGame->GetVehiclePool()->GetSlotState(vehicle))
	{
		int iColors[2];
		pNetGame->GetVehiclePool()->GetAt(vehicle)->GetColor(&iColors[0], &iColors[1]);

		sq_newarray(pVM, 0);
		sq_pushinteger(pVM, iColors[0]);
		sq_arrayappend(pVM, -2);
		sq_pushinteger(pVM, iColors[1]);
		sq_arrayappend(pVM, -2);
		sq_push(pVM, -1);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}