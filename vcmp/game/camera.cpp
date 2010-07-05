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
// File Author: kyeman
//
//----------------------------------------------------

#include "game.h"
#include "util.h"
#include "address.h"

//-----------------------------------------------------------

void CCamera::SetBehindPlayer()
{
	DWORD dwFunc = FUNC_CCamera__PutBehindPlayer;
	_asm
	{
		mov ecx, VAR_Camera
		call dwFunc
	}
}

//-----------------------------------------------------------

void CCamera::SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ)
{
	ScriptCommand(&set_camera_position,fX,fY,fZ,fRotationX,fRotationY,fRotationZ);
}

//-----------------------------------------------------------

void CCamera::LookAtPoint(VECTOR vPoint, int iType)
{
	/*if(fZ < -100.0) { // min ground point
		fZ = FindGroundForZCoord
	}*/
	VECTOR * pPoint = &vPoint;
	DWORD dwFunc = FUNC_CCamera__SetTargetPoint;
	_asm
	{
		push 1
		push iType
		push pPoint
		mov ecx, VAR_Camera
		call dwFunc
	}
}

//-----------------------------------------------------------

void CCamera::Restore()
{
	DWORD dwFunc = FUNC_CCamera__Restore;
	_asm
	{
		mov ecx, VAR_Camera
		call dwFunc
	}
}

//-----------------------------------------------------------