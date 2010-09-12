//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#pragma once

#define MAX_OBJECTS 100

class CObjectPool
{
public:
	CObjectPool();
	~CObjectPool();

	EntityId New(int iModel, Vector3 * vecPos, Vector3 * vecRot);
	bool Delete(EntityId ObjectID);

	CObject* GetAt(EntityId ObjectID)
	{
		if(ObjectID > MAX_OBJECTS) { return NULL; }
		return m_pObjects[ObjectID];
	};

	BOOL GetSlotState(EntityId ObjectID)
	{
		if(ObjectID > MAX_OBJECTS) { return FALSE; }
		return m_bObjectSlotState[ObjectID];
	};

	void InitForPlayer(EntityId playerID);

	EntityId GetFreeSlot();

private:
	BOOL m_bObjectSlotState[MAX_OBJECTS];
	CObject * m_pObjects[MAX_OBJECTS];
};