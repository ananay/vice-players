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

#include "../main.h"
#include "../game/pools.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CChatWindow *pChatWindow;

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
	for(BYTE i = 0; i < MAX_VEHICLES; i++) {
		if(m_bVehicleSlotState[i]) {
			Delete(i);
		}
	}
}

//----------------------------------------------------

BOOL CVehiclePool::New( BYTE byteVehicleID, BYTE byteVehicleType,
					    Vector3 * vecPos, float fRotation,
					    int iColor1, int iColor2,
					    Vector3 * vecSpawnPos, float fSpawnRotation )
{

	// Setup the spawninfo for the next respawn.
	m_SpawnInfo[byteVehicleID].byteVehicleType = byteVehicleType;
	m_SpawnInfo[byteVehicleID].vecPos.X = vecSpawnPos->X;
	m_SpawnInfo[byteVehicleID].vecPos.Y = vecSpawnPos->Y;
	m_SpawnInfo[byteVehicleID].vecPos.Z = vecSpawnPos->Z;
	m_SpawnInfo[byteVehicleID].fRotation = fSpawnRotation;
	m_SpawnInfo[byteVehicleID].iColor1 = iColor1;
	m_SpawnInfo[byteVehicleID].iColor2 = iColor2;

	// Now go ahead and spawn it at the location we got passed.
	return Spawn(byteVehicleID,byteVehicleType,vecPos,fRotation,iColor1,iColor2);
}

//----------------------------------------------------

BOOL CVehiclePool::Delete(BYTE byteVehicleID)
{
	if(!GetSlotState(byteVehicleID) || !m_pVehicles[byteVehicleID]) {
		return FALSE;
	}

	m_bVehicleSlotState[byteVehicleID] = FALSE;
	delete m_pVehicles[byteVehicleID];
	m_pVehicles[byteVehicleID] = NULL;

	return TRUE;
}

//----------------------------------------------------

BOOL CVehiclePool::Spawn( BYTE byteVehicleID, BYTE byteVehicleType,
					      Vector3 * vecPos, float fRotation,
					      int iColor1, int iColor2 )
{	
	if(m_pVehicles[byteVehicleID] != NULL) {
		Delete(byteVehicleID);
	}

	m_pVehicles[byteVehicleID] =  new CVehicle(byteVehicleType,
		vecPos->X,vecPos->Y,vecPos->Z,fRotation);

	if(m_pVehicles[byteVehicleID])
	{	
		if(iColor1 != (-1)) {
			m_pVehicles[byteVehicleID]->SetColor(iColor1,iColor2);
		}

		m_bVehicleSlotState[byteVehicleID] = TRUE;

		m_bIsActive[byteVehicleID] = TRUE;
		m_bIsWasted[byteVehicleID] = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------

int CVehiclePool::FindIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle)
{
	for(int i = 1; i < MAX_VEHICLES; i++) {
		if(m_bVehicleSlotState[i]) {
			if(pGtaVehicle == m_pVehicles[i]->GetVehicle()) {
				return i;
			}
		}
	}
	return (-1);
}

//----------------------------------------------------

int CVehiclePool::FindGtaIDFromID(int iID)
{
	if(m_pVehicles[iID]) {
		return CPools::GetIndexFromVehicle(m_pVehicles[iID]->GetVehicle());
	}
	// not too sure about this
	return 0;
}

//----------------------------------------------------

void CVehiclePool::SendVehicleDeath(BYTE byteVehicleId)
{
	BitStream bsVehicleDeath;
	bsVehicleDeath.Write(byteVehicleId);
	pNetGame->GetRPC4()->Call("VehicleDeath", &bsVehicleDeath, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
		UNASSIGNED_SYSTEM_ADDRESS, true);
}

//----------------------------------------------------

void CVehiclePool::Process()
{
	CVehicle *pVehicle;
	DWORD dwThisTime = GetTickCount();

	for(BYTE x = 0; x < MAX_VEHICLES; x++)
	{
		if(GetSlotState(x)) { // It's inuse.
			pVehicle = m_pVehicles[x];

			if(m_bIsActive[x]) {
				if((pVehicle->GetHealth() == 0.0f) || 
					(pVehicle->GetVehicleSubtype() != VEHICLE_SUBTYPE_BOAT &&
					pVehicle->GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE &&
					pVehicle->HasSunk())) { // It's dead or its not a boat and it has sunk
					if(!pVehicle->IsDead()) {
						SendVehicleDeath(x);
						pVehicle->SetDead(TRUE);
					}
				}
				else 
				{
					if(pVehicle->IsDriverLocalPlayer()) {
						pVehicle->SetInvulnerable(FALSE);
					} else {
						pVehicle->SetInvulnerable(TRUE);
					}

					// Lock vehicles beyond given radius.
					// whats the point in this?
					if(pVehicle->GetDistanceFromLocalPlayerPed() > 300.0f) {
						pVehicle->SetLockedState(1);
					} else {
						pVehicle->SetLockedState(0);
					}

					if(pVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_BIKE) {
						pVehicle->VerifyControlState();
					}		
				}
			}
		}
	}
}

//----------------------------------------------------