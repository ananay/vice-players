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
//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): kyeman
//
//----------------------------------------------------

#include "game.h"
#include "util.h"
#include "address.h"

//-----------------------------------------------------------

CCamera::CCamera()
{
	m_pCamera = (CAMERA_TYPE *)VAR_Camera;
}

//-----------------------------------------------------------

void CCamera::SetBehindPlayer()
{
	CAMERA_TYPE * pCamera = m_pCamera;
	if(pCamera) {
		DWORD dwFunc = FUNC_CCamera__PutBehindPlayer;
		_asm
		{
			mov ecx, pCamera
			call dwFunc
		}
	}
}

CCamera::~CCamera()
{
	
}

//-----------------------------------------------------------

void CCamera::SetCamera(CAMERA_TYPE * pCamera)
{
	m_pCamera = pCamera;
}

//-----------------------------------------------------------

CAMERA_TYPE * CCamera::GetCamera()
{
	return m_pCamera;
}

//-----------------------------------------------------------

void CCamera::SetPosition(Vector3 vecPosition)
{
	if(m_pCamera) {
		m_pCamera->vecPosition.X = vecPosition.X;
		m_pCamera->vecPosition.Y = vecPosition.Y;
		m_pCamera->vecPosition.Z = vecPosition.Z;
		SetInFreeMode(FALSE);
	}
}

//-----------------------------------------------------------

void CCamera::SetRotation(Vector3 vecRotation)
{
	if(m_pCamera) {
		m_pCamera->vecRotation.X = vecRotation.X;
		m_pCamera->vecRotation.Y = vecRotation.Y;
		m_pCamera->vecRotation.Z = vecRotation.Z;
		SetInFreeMode(FALSE);
	}
}

//-----------------------------------------------------------

void CCamera::LookAtPoint(Vector3 vecPoint, int iType)
{
	CAMERA_TYPE * pCamera = m_pCamera;
	if(pCamera) {
		/*if(fZ < -100.0) { // min ground point
			fZ = FindGroundForZCoord
		}*/
		Vector3 * pPoint = &vecPoint;
		DWORD dwFunc = FUNC_CCamera__SetTargetPoint;
		_asm
		{
			push 1
			push iType
			push pPoint
			mov ecx, pCamera
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CCamera::Restore()
{
	CAMERA_TYPE * pCamera = m_pCamera;
	if(pCamera) {
		DWORD dwFunc = FUNC_CCamera__Restore;
		_asm
		{
			mov ecx, pCamera
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CCamera::SetInFreeMode(BOOL bFreeMode)
{
	if(m_pCamera) {
		if(bFreeMode) {
			m_pCamera->byteInFreeMode = 1;
		} else {
			m_pCamera->byteInFreeMode = 0;
		}
	}
}

//-----------------------------------------------------------

BOOL CCamera::IsInFreeMode()
{
	if(m_pCamera) {
		if(m_pCamera->byteInFreeMode) {
			return TRUE;
		} else {
			return FALSE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------