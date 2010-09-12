//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "entity.h"
#include "util.h"
#include "address.h"

//----------------------------------------------------------

ENTITY_TYPE * CEntity::GetEntity()
{
	return m_pEntity;
}

//----------------------------------------------------------

void CEntity::SetEntity(ENTITY_TYPE * pEntity)
{
	m_pEntity = pEntity;
}

//----------------------------------------------------------

void CEntity::GetMatrix(MATRIX4X4 * matMatrix)
{
	if(m_pEntity)
	{
		memcpy(matMatrix, &m_pEntity->placeable.matMatrix, sizeof(MATRIX4X4));
	}
}

//-----------------------------------------------------------

void CEntity::SetMatrix(MATRIX4X4 matMatrix)
{
	if(m_pEntity)
	{
		memcpy(&m_pEntity->placeable.matMatrix, &matMatrix, sizeof(MATRIX4X4));
	}
}

//-----------------------------------------------------------

void CEntity::GetPosition(Vector3 * vecPosition)
{
	if(m_pEntity)
	{
		memcpy(vecPosition, &m_pEntity->placeable.matMatrix.vPos, sizeof(Vector3));
	}
}

//-----------------------------------------------------------

void CEntity::SetPosition(Vector3 vecPosition)
{
	if(m_pEntity)
	{
		memcpy(&m_pEntity->placeable.matMatrix.vPos, &vecPosition, sizeof(Vector3));
	}
}

//-----------------------------------------------------------

void CEntity::SetHeading(float fHeading)
{
	if(m_pEntity)
	{
		PLACEABLE * pPlaceable = &m_pEntity->placeable;
		DWORD dwFunc = FUNC_CPlaceable__SetHeading;
		_asm
		{
			push fHeading
			mov ecx, pPlaceable
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

WORD CEntity::GetModelIndex()
{
	if(m_pEntity)
	{
		return m_pEntity->wModelIndex;
	}

	return 0;
}

//-----------------------------------------------------------

BOOL CEntity::IsOnScreen()
{
	if(m_pEntity)
	{
		return GameIsEntityOnScreen((DWORD *)m_pEntity);
	}

	return FALSE;
}

//-----------------------------------------------------------

float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
	float fDistance = 0.0f;

	if(m_pEntity)
	{
		ENTITY_TYPE * pEntity = m_pEntity;
		DWORD dwFunc = FUNC_CEntity__GetDistanceFromCentreOfMassToBaseOfModel;
		_asm
		{
			mov ecx, pEntity
			call dwFunc
			fstp fDistance
		}
	}

	return fDistance;
}

//-----------------------------------------------------------
