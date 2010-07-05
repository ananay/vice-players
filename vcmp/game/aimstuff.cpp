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

#include <stdio.h>
#include "../../raknet/WindowsIncludes.h"
#include "common.h"
#include "aimstuff.h"
#include "address.h"
#include "game.h"

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
	CAMERA_TYPE * pCamera = pGame->GetCamera()->GetCamera();
	if(pCamera) {
		memcpy(&caLocalPlayerAim,&pCamera->aim,sizeof(CAMERA_AIM));
	}
}

//----------------------------------------------------------

void GameSetLocalPlayerAim()
{
	CAMERA_TYPE * pCamera = pGame->GetCamera()->GetCamera();
	if(pCamera) {
		memcpy(&pCamera->aim,&caLocalPlayerAim,sizeof(CAMERA_AIM));
	}
}

//----------------------------------------------------------

CAMERA_AIM * GameGetInternalAim()
{
	CAMERA_TYPE * pCamera = pGame->GetCamera()->GetCamera();
	if(pCamera) {
		return &pCamera->aim;
	}
	return NULL;
}

//----------------------------------------------------------

void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim)
{
	memcpy(&caRemotePlayerAim[iPlayer],caAim,sizeof(CAMERA_AIM));
}

//----------------------------------------------------------

void GameSetRemotePlayerAim(int iPlayer)
{
	CAMERA_TYPE * pCamera = pGame->GetCamera()->GetCamera();
	if(pCamera) {
		memcpy(&pCamera->aim,&caRemotePlayerAim[iPlayer],sizeof(CAMERA_AIM));
	}
}

//----------------------------------------------------------


