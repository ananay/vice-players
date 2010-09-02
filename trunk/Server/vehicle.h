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
// File Author(s): kyeman
//
//----------------------------------------------------------


#pragma once

typedef struct _VEHICLE_SPAWN_INFO
{
	BYTE byteVehicleType;
	Vector3 vecPos;
	float fRotation;
	int iColor1;
	int iColor2;
} VEHICLE_SPAWN_INFO;

//----------------------------------------------------

class CVehicle
{
public:

	EntityId				m_vehicleID;
	EntityId				m_driverID;
	EntityId				m_passengers[8];
	BOOL					m_bIsActive;
	BOOL					m_bIsWasted;
	VEHICLE_SPAWN_INFO		m_SpawnInfo;
	MATRIX4X4				m_matWorld;
	Vector3					m_vecMoveSpeed;
	Vector3					m_vecTurnSpeed;
	int						m_iColors[2];
	float					m_fHealth;
	BOOL					m_bHasHadUpdate;
	BOOL					m_bHasDriver;
	DWORD					m_dwTimeSinceLastDriven;
	BOOL					m_bHasBeenDriven;

	CVehicle(BYTE byteModel,Vector3 *vecPos,float fRotation,int iColor1,int iColor2);
	~CVehicle();

	void Reset();

	void SetHealth(float newHealth);
	float GetHealth() { return m_fHealth; };
	void SetColor(int color1, int color2);
	void GetColor(int * iColor1, int * iColor2) { *iColor1 = m_iColors[0]; *iColor2 = m_iColors[1]; }

	void PopTrunk();

	void GetPosition(Vector3 * vecPosition);
	void GetMoveSpeed(Vector3 * vecMoveSpeed);
	void GetTurnSpeed(Vector3 * vecTurnSpeed);

	void SetDriverId(EntityId driverId) { m_driverID = driverId; };
	EntityId GetDriverId() { return m_driverID; };

	BOOL IsActive() { return m_bIsActive; };

	BOOL IsWasted() { return m_bIsWasted; };
	void SetWasted(BOOL bWasted) { m_bIsWasted = bWasted; };

	DWORD GetTimeSinceLastDriven() { return m_dwTimeSinceLastDriven; };
	BOOL HasBeenDriven() { return m_bHasBeenDriven; };

	void SetID(EntityId vehicleID) { m_vehicleID = vehicleID; };
	VEHICLE_SPAWN_INFO * GetSpawnInfo() { return &m_SpawnInfo; };

	void SpawnForPlayer(EntityId forPlayerID);
	void DestroyForPlayer(EntityId forPlayerID);
	void SpawnForWorld();
	void DestroyForWorld();
	void Respawn();

	void UpdateLastDrivenTime();

	void Update(EntityId playerID, MATRIX4X4 * matWorld, Vector3 * vecMoveSpeed, Vector3 * vecTurnSpeed, float fHealth);
};