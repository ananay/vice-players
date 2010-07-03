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
		Matrix->vLookRight.X = m_pEntity->mat.vLookRight.X;
		Matrix->vLookRight.Y = m_pEntity->mat.vLookRight.Y;
		Matrix->vLookRight.Z = m_pEntity->mat.vLookRight.Z;
		Matrix->vLookUp.X = m_pEntity->mat.vLookUp.X;
		Matrix->vLookUp.Y = m_pEntity->mat.vLookUp.Y;
		Matrix->vLookUp.Z = m_pEntity->mat.vLookUp.Z;
		Matrix->vLookAt.X = m_pEntity->mat.vLookAt.X;
		Matrix->vLookAt.Y = m_pEntity->mat.vLookAt.Y;
		Matrix->vLookAt.Z = m_pEntity->mat.vLookAt.Z;
		Matrix->vPos.X = m_pEntity->mat.vPos.X;
		Matrix->vPos.Y = m_pEntity->mat.vPos.Y;
		Matrix->vPos.Z = m_pEntity->mat.vPos.Z;
	}
}

//-----------------------------------------------------------

void CEntity::SetMatrix(MATRIX4X4 Matrix)
{
	if(m_pEntity) {
		m_pEntity->mat.vLookRight.X = Matrix.vLookRight.X;
		m_pEntity->mat.vLookRight.Y = Matrix.vLookRight.Y;
		m_pEntity->mat.vLookRight.Z = Matrix.vLookRight.Z;
		m_pEntity->mat.vLookUp.X = Matrix.vLookUp.X;
		m_pEntity->mat.vLookUp.Y = Matrix.vLookUp.Y;
		m_pEntity->mat.vLookUp.Z = Matrix.vLookUp.Z;
		m_pEntity->mat.vLookAt.X = Matrix.vLookAt.X;
		m_pEntity->mat.vLookAt.Y = Matrix.vLookAt.Y;
		m_pEntity->mat.vLookAt.Z = Matrix.vLookAt.Z;
		m_pEntity->mat.vPos.X = Matrix.vPos.X;
		m_pEntity->mat.vPos.Y = Matrix.vPos.Y;
		m_pEntity->mat.vPos.Z = Matrix.vPos.Z;
	}
}

//-----------------------------------------------------------

WORD CEntity::GetModelIndex()
{
	if(m_pEntity) {
		return m_pEntity->nModelIndex;
	}
	return 0;
}

//-----------------------------------------------------------

void CEntity::GetMoveSpeed(PVECTOR Vector)
{
	if(m_pEntity) {
		Vector->X = m_pEntity->vecMoveSpeed.X;
		Vector->Y = m_pEntity->vecMoveSpeed.Y;
		Vector->Z = m_pEntity->vecMoveSpeed.Z;
	}
}

//-----------------------------------------------------------

void CEntity::SetMoveSpeed(VECTOR Vector)
{
	if(m_pEntity) {
		m_pEntity->vecMoveSpeed.X = Vector.X;
		m_pEntity->vecMoveSpeed.Y = Vector.Y;
		m_pEntity->vecMoveSpeed.Z = Vector.Z;
	}
}

//-----------------------------------------------------------

void CEntity::GetTurnSpeed(PVECTOR Vector)
{
	if(m_pEntity) {
		Vector->X = m_pEntity->vecTurnSpeed.X;
		Vector->Y = m_pEntity->vecTurnSpeed.Y;
		Vector->Z = m_pEntity->vecTurnSpeed.Z;
	}
}

//-----------------------------------------------------------

void CEntity::SetTurnSpeed(VECTOR Vector)
{
	if(m_pEntity) {
		m_pEntity->vecTurnSpeed.X = Vector.X;
		m_pEntity->vecTurnSpeed.Y = Vector.Y;
		m_pEntity->vecTurnSpeed.Z = Vector.Z;
	}
}

//-----------------------------------------------------------