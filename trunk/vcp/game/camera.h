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
//                 jenksta
//
//----------------------------------------------------

#pragma once

#include "game.h"

//-----------------------------------------------------------

class CCamera
{
private:
	CAMERA_TYPE * m_pCamera;

public:
	CCamera();
	~CCamera();

	void          SetCamera(CAMERA_TYPE * pCamera);
	CAMERA_TYPE * GetCamera();
	void          SetBehindPlayer();
	void          SetPosition(Vector3 vecPosition);
	void          SetRotation(Vector3 vecRotation);
	void          LookAtPoint(Vector3 vecPoint, int iType);
	void          Restore();
	void          SetInFreeMode(bool bFreeMode);
	bool          IsInFreeMode();
	void          SetDriveByLeft(BYTE byteDriveByLeft);
	BYTE          GetDriveByLeft();
	void          SetDriveByRight(BYTE byteDriveByRight);
	BYTE          GetDriveByRight();
	void          SetAim(CAMERA_AIM * pAim);
	CAMERA_AIM *  GetAim();
};

//-----------------------------------------------------------
