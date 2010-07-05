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
//----------------------------------------------------------

#include "entity.h"
#include "util.h"
#include "address.h"

//----------------------------------------------------------

ENTITY_TYPE *CEntity::GetEntity()
{
	return m_pEntity;
}

//----------------------------------------------------------

void CEntity::SetEntity(ENTITY_TYPE *pEntity)
{
	m_pEntity = pEntity;
}

//----------------------------------------------------------

void CEntity::GetMatrix(PMATRIX4X4 Matrix)
{
	if(m_pEntity) {
		Matrix->vLookRight.X = m_pEntity->placeable.matMatrix.vLookRight.X;
		Matrix->vLookRight.Y = m_pEntity->placeable.matMatrix.vLookRight.Y;
		Matrix->vLookRight.Z = m_pEntity->placeable.matMatrix.vLookRight.Z;
		Matrix->vLookUp.X = m_pEntity->placeable.matMatrix.vLookUp.X;
		Matrix->vLookUp.Y = m_pEntity->placeable.matMatrix.vLookUp.Y;
		Matrix->vLookUp.Z = m_pEntity->placeable.matMatrix.vLookUp.Z;
		Matrix->vLookAt.X = m_pEntity->placeable.matMatrix.vLookAt.X;
		Matrix->vLookAt.Y = m_pEntity->placeable.matMatrix.vLookAt.Y;
		Matrix->vLookAt.Z = m_pEntity->placeable.matMatrix.vLookAt.Z;
		Matrix->vPos.X = m_pEntity->placeable.matMatrix.vPos.X;
		Matrix->vPos.Y = m_pEntity->placeable.matMatrix.vPos.Y;
		Matrix->vPos.Z = m_pEntity->placeable.matMatrix.vPos.Z;
	}
}

//-----------------------------------------------------------

void CEntity::SetMatrix(MATRIX4X4 Matrix)
{
	if(m_pEntity) {
		m_pEntity->placeable.matMatrix.vLookRight.X = Matrix.vLookRight.X;
		m_pEntity->placeable.matMatrix.vLookRight.Y = Matrix.vLookRight.Y;
		m_pEntity->placeable.matMatrix.vLookRight.Z = Matrix.vLookRight.Z;
		m_pEntity->placeable.matMatrix.vLookUp.X = Matrix.vLookUp.X;
		m_pEntity->placeable.matMatrix.vLookUp.Y = Matrix.vLookUp.Y;
		m_pEntity->placeable.matMatrix.vLookUp.Z = Matrix.vLookUp.Z;
		m_pEntity->placeable.matMatrix.vLookAt.X = Matrix.vLookAt.X;
		m_pEntity->placeable.matMatrix.vLookAt.Y = Matrix.vLookAt.Y;
		m_pEntity->placeable.matMatrix.vLookAt.Z = Matrix.vLookAt.Z;
		m_pEntity->placeable.matMatrix.vPos.X = Matrix.vPos.X;
		m_pEntity->placeable.matMatrix.vPos.Y = Matrix.vPos.Y;
		m_pEntity->placeable.matMatrix.vPos.Z = Matrix.vPos.Z;
	}
}

//-----------------------------------------------------------

void CEntity::GetPosition(PVECTOR Vector)
{
	if(m_pEntity) {
		Vector->X = m_pEntity->placeable.matMatrix.vPos.X;
		Vector->Y = m_pEntity->placeable.matMatrix.vPos.Y;
		Vector->Z = m_pEntity->placeable.matMatrix.vPos.Z;
	}
}

//-----------------------------------------------------------

void CEntity::SetPosition(VECTOR Vector)
{
	if(m_pEntity) {
		m_pEntity->placeable.matMatrix.vPos.X = Vector.X;
		m_pEntity->placeable.matMatrix.vPos.Y = Vector.Y;
		m_pEntity->placeable.matMatrix.vPos.Z = Vector.Z;
	}
}

//-----------------------------------------------------------

void CEntity::SetHeading(float fHeading)
{
	if(m_pEntity) {
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
	if(m_pEntity) {
		return m_pEntity->wModelIndex;
	}
	return 0;
}

//-----------------------------------------------------------

BOOL CEntity::IsOnScreen()
{
	if(m_pEntity) {
		return GameIsEntityOnScreen((DWORD *)m_pEntity);
	}
	return FALSE;
}

//-----------------------------------------------------------