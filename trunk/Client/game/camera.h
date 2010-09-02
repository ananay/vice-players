//---------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP Team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
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
