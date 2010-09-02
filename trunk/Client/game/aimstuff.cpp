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

#include <stdio.h>
#include "../../raknet/WindowsIncludes.h"
#include "common.h"
#include "aimstuff.h"
#include "game.h"

//-----------------------------------------------------------

extern CGame * pGame;

CAMERA_AIM caLocalPlayerAim;
CAMERA_AIM caRemotePlayerAim[MAX_PLAYERS];

//----------------------------------------------------------

void GameAimSyncInit()
{
	memset(&caLocalPlayerAim, 0, sizeof(CAMERA_AIM));
	memset(caRemotePlayerAim, 0, (sizeof(CAMERA_AIM) * MAX_PLAYERS));
}

//----------------------------------------------------------

void GameStoreLocalPlayerAim()
{
	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		memcpy(&caLocalPlayerAim, pCamera->GetAim(), sizeof(CAMERA_AIM));
	}
}

//----------------------------------------------------------

void GameSetLocalPlayerAim()
{
	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		pCamera->SetAim(&caLocalPlayerAim);
	}
}

//----------------------------------------------------------

void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim)
{
	memcpy(&caRemotePlayerAim[iPlayer], caAim, sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void GameSetRemotePlayerAim(int iPlayer)
{
	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		pCamera->SetAim(&caRemotePlayerAim[iPlayer]);
	}
}

//----------------------------------------------------------
