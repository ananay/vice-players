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
// File Author(s): adamix
//
//-----------------------------------------------------
#pragma once

class CObject 
{
private:
	EntityId m_ObjectID;
	int m_iModel;
	BOOL m_bIsActive;

	Vector3 m_vecPos;
	Vector3 m_vecRot;
public:
	CObject(int iModel, Vector3 * vecPos, Vector3 * vecRot);
	~CObject();

	void SetID(EntityId objectID) { m_ObjectID = objectID; };
	BOOL IsActive() { return m_bIsActive; };
	
	void SpawnForPlayer(EntityId forPlayerID);
	//void DestroyForPlayer(EntityId forPlayerID);
	void SpawnForWorld();
	//void DestroyForWorld();
};