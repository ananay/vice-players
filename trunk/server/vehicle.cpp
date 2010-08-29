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
#include <math.h>

extern CNetGame *pNetGame;
extern CScripts * pScripts;

#define PI 3.14159265

//----------------------------------------------------------

using namespace RakNet;

//----------------------------------------------------------

CVehicle::CVehicle( BYTE byteModel, Vector3 *vecPos,
				    float fRotation, int iColor1,
				    int iColor2)
{


	// Store the spawn info.
	m_SpawnInfo.byteVehicleType = byteModel;
	m_SpawnInfo.fRotation = fRotation;
	m_SpawnInfo.vecPos.X = vecPos->X;
	m_SpawnInfo.vecPos.Y = vecPos->Y;
	m_SpawnInfo.vecPos.Z = vecPos->Z;
	m_SpawnInfo.iColor1 = iColor1;
	m_SpawnInfo.iColor2 = iColor2;

	Reset();
}

CVehicle::~CVehicle()
{
	RakNet::BitStream bsSend;
	bsSend.Write(m_byteVehicleID);
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			pNetGame->GetRPC4()->Call("Script_DestroyVehicle",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}
}

//----------------------------------------------------

void CVehicle::Reset()
{
	// Set the initial pos to spawn pos.
	memset(&m_matWorld,0,sizeof(MATRIX4X4));
	m_matWorld.vPos.X = m_SpawnInfo.vecPos.X;
	m_matWorld.vPos.Y = m_SpawnInfo.vecPos.Y;
	m_matWorld.vPos.Z = m_SpawnInfo.vecPos.Z;

	// Reset the initial turn speed and move speed
	memset(&m_vecMoveSpeed,0,sizeof(Vector3));
	memset(&m_vecTurnSpeed,0,sizeof(Vector3));

	// Set the initial colors to the spawn colors
	m_iColors[0] = m_SpawnInfo.iColor1;
	m_iColors[1] = m_SpawnInfo.iColor2;

	m_bIsActive = TRUE;
	m_bIsWasted = FALSE;
	m_bHasHadUpdate = FALSE;
	m_byteDriverID = INVALID_ID;
	m_fHealth = 1000.0f;
	m_bHasDriver = FALSE;
	m_bHasBeenDriven = FALSE;
	m_dwTimeSinceLastDriven = GetTickCount();
}

//----------------------------------------------------
// Updates our stored data structures for this
// network vehicle.

void CVehicle::Update(BYTE bytePlayerID, MATRIX4X4 * matWorld, Vector3 * vecMoveSpeed, Vector3 * vecTurnSpeed, float fHealth)
{
	m_byteDriverID = bytePlayerID;
	memcpy(&m_matWorld, matWorld, sizeof(MATRIX4X4));
	memcpy(&m_vecMoveSpeed,vecMoveSpeed, sizeof(Vector3));
	memcpy(&m_vecTurnSpeed, vecTurnSpeed, sizeof(Vector3));

	if(m_fHealth != fHealth)
	{
		pScripts->onVehicleDamage(m_byteVehicleID, m_fHealth, fHealth);
	}

	m_fHealth = fHealth;
	m_bHasHadUpdate = TRUE;
	pScripts->onVehicleSync(m_byteVehicleID);
}

//----------------------------------------------------

void CVehicle::SpawnForPlayer(BYTE byteForSystemAddress)
{
	RakNet::BitStream bsVehicleSpawn;

	bsVehicleSpawn.Write(m_byteVehicleID);
	bsVehicleSpawn.Write(m_SpawnInfo.byteVehicleType);
	bsVehicleSpawn.Write(m_matWorld.vPos.X);
	bsVehicleSpawn.Write(m_matWorld.vPos.Y);
	bsVehicleSpawn.Write(m_matWorld.vPos.Z);
	// TODO: This should use the current rotation, not the spawn rotation
	bsVehicleSpawn.Write(m_SpawnInfo.fRotation);
	bsVehicleSpawn.Write(m_iColors[0]);
	bsVehicleSpawn.Write(m_iColors[1]);
	bsVehicleSpawn.Write(m_fHealth);

	// now add spawn co-ords and rotation
	bsVehicleSpawn.Write(m_SpawnInfo.vecPos.X);
	bsVehicleSpawn.Write(m_SpawnInfo.vecPos.Y);
	bsVehicleSpawn.Write(m_SpawnInfo.vecPos.Z);
	bsVehicleSpawn.Write(m_SpawnInfo.fRotation);
	
	pNetGame->GetRPC4()->Call("VehicleSpawn", &bsVehicleSpawn,HIGH_PRIORITY,RELIABLE_ORDERED,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(byteForSystemAddress),false);
}

//----------------------------------------------------------

void CVehicle::DestroyForPlayer(BYTE byteForSystemAddress)
{
	RakNet::BitStream bsVehicleDestroy;

	bsVehicleDestroy.Write(m_byteVehicleID);

	pNetGame->GetRPC4()->Call("VehicleDestroy", &bsVehicleDestroy,HIGH_PRIORITY,RELIABLE_ORDERED,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(byteForSystemAddress),false);
}

//----------------------------------------------------------

void CVehicle::SpawnForWorld()
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			SpawnForPlayer(i);
		}
	}
}

//----------------------------------------------------------

void CVehicle::DestroyForWorld()
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			DestroyForPlayer(i);
		}
	}
}

//----------------------------------------------------------

void CVehicle::Respawn()
{
	DestroyForWorld();
	Reset();
	SpawnForWorld();
}

//----------------------------------------------------------

void CVehicle::UpdateLastDrivenTime()
{
	if(m_byteDriverID) {
		m_bHasBeenDriven = TRUE;
		m_dwTimeSinceLastDriven = GetTickCount();
	}
}

//----------------------------------------------------------

void CVehicle::SetHealth(float newHealth)
{
	RakNet::BitStream bsSend;
	bsSend.Write(m_byteVehicleID);
	bsSend.Write(newHealth);
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			pNetGame->GetRPC4()->Call("Script_SetVehicleHealth",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}
}

//----------------------------------------------------------

void CVehicle::SetColor(int color1, int color2)
{
	RakNet::BitStream bsSend;
	bsSend.Write(m_byteVehicleID);
	bsSend.Write(color1);
	bsSend.Write(color2);
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			pNetGame->GetRPC4()->Call("Script_SetVehicleColor",&bsSend,HIGH_PRIORITY,RELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i),false);
		}
	}
	m_iColors[0] = color1;
	m_iColors[1] = color2;
}

//----------------------------------------------------------

void CVehicle::GetPosition(Vector3 * vecPosition)
{
	vecPosition->X = m_matWorld.vPos.X;
	vecPosition->Y = m_matWorld.vPos.Y;
	vecPosition->Z = m_matWorld.vPos.Z;
}

//----------------------------------------------------

void CVehicle::GetMoveSpeed(Vector3 * vecMoveSpeed)
{
	memcpy(vecMoveSpeed, &m_vecMoveSpeed, sizeof(Vector3));
}

//----------------------------------------------------

void CVehicle::GetTurnSpeed(Vector3 * vecTurnSpeed)
{
	memcpy(vecTurnSpeed, &m_vecTurnSpeed, sizeof(Vector3));
}

//----------------------------------------------------------