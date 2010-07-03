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

#include "netgame.h"

//----------------------------------------------------

CVehiclePool::CVehiclePool()
{
	for(BYTE i = 0; i < MAX_VEHICLES; i++) {
		m_bVehicleSlotState[i] = FALSE;
		m_pVehicles[i] = NULL;
	}
}

//----------------------------------------------------

CVehiclePool::~CVehiclePool()
{	
	BYTE byteVehicleID = 0;
	while(byteVehicleID != MAX_VEHICLES) {
		Delete(byteVehicleID);
		byteVehicleID++;
	}
}

//----------------------------------------------------

BOOL CVehiclePool::New(BYTE byteVehicleID, BYTE byteVehicleType,
					   VECTOR * vecPos, float fRotation,
					   int iColor1, int iColor2)
{
	m_pVehicles[byteVehicleID] = new CVehicle(byteVehicleType,vecPos,fRotation,iColor1,iColor2);

	/*
	logprintf("CVehiclePool::New(%u,%.2f,%.2f,%.2f,%.2f,%d,%d)",
		byteVehicleType,vecPos->X,vecPos->Y,vecPos->Z,fRotation,iColor1,iColor2);*/

	if(m_pVehicles[byteVehicleID])
	{
		m_pVehicles[byteVehicleID]->SetID(byteVehicleID);
		m_bVehicleSlotState[byteVehicleID] = TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

BYTE CVehiclePool::New(BYTE byteVehicleType,
					   VECTOR * vecPos, float fRotation,
					   int iColor1, int iColor2)
{
	BYTE vehicleID = GetFreeSlot();
	if(New(vehicleID, byteVehicleType, vecPos, fRotation, iColor1, iColor2))
		return vehicleID;

	return false;
}

//----------------------------------------------------

BOOL CVehiclePool::Delete(BYTE byteVehicleID)
{
	if(!GetSlotState(byteVehicleID) || !m_pVehicles[byteVehicleID])
	{
		return FALSE;
	}

	m_bVehicleSlotState[byteVehicleID] = FALSE;
	delete m_pVehicles[byteVehicleID];
	m_pVehicles[byteVehicleID] = NULL;

	return TRUE;
}

//----------------------------------------------------

void CVehiclePool::Process()
{
	
}

//----------------------------------------------------

BYTE CVehiclePool::GetFreeSlot()
{
	for(BYTE id = 0; id < MAX_VEHICLES; id++)
	{
		if(m_pVehicles[id] == NULL)
			return id;
	}
	return false;
}

//----------------------------------------------------