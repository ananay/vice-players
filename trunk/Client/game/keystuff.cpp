//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include <stdio.h>
#include "../../raknet/WindowsIncludes.h"
#include "common.h"
#include "keystuff.h"
#include "address.h"
#include "game.h"

//-----------------------------------------------------------

extern CGame * pGame;

GTA_CONTROLSET * pGcsInternalKeys = (GTA_CONTROLSET *)ADDR_KEYSTATES;
GTA_CONTROLSET GcsLocalPlayerKeys;
GTA_CONTROLSET GcsRemotePlayerKeys[MAX_PLAYERS];

BYTE byteSaveDriveByLeft = 0;
BYTE byteSaveDriveByRight = 0;

//-----------------------------------------------------------

void GameKeyStatesInit()
{
	memset(&GcsLocalPlayerKeys, 0, sizeof(GTA_CONTROLSET));

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		memset(&GcsRemotePlayerKeys[i], 0, sizeof(GTA_CONTROLSET));
	}
}

//-----------------------------------------------------------

void GameStoreLocalPlayerKeys()
{	
	memcpy(&GcsLocalPlayerKeys, pGcsInternalKeys, sizeof(GTA_CONTROLSET));

	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		// save current drive by state
		byteSaveDriveByLeft = pCamera->GetDriveByLeft();
		byteSaveDriveByRight = pCamera->GetDriveByRight();
	}
}

//-----------------------------------------------------------

void GameSetLocalPlayerKeys()
{
	memcpy(pGcsInternalKeys, &GcsLocalPlayerKeys, sizeof(GTA_CONTROLSET));

	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		// restore the drive by state also
		pCamera->SetDriveByLeft(byteSaveDriveByLeft);
		pCamera->SetDriveByRight(byteSaveDriveByRight);
	}
}

//-----------------------------------------------------------

void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET * pGcsKeyStates)
{
	memcpy(&GcsRemotePlayerKeys[iPlayer], pGcsKeyStates, sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameSetRemotePlayerKeys(int iPlayer)
{
	memcpy(pGcsInternalKeys, &GcsRemotePlayerKeys[iPlayer], sizeof(GTA_CONTROLSET));

	CCamera * pCamera = pGame->GetCamera();

	if(pCamera)
	{
		if(GcsRemotePlayerKeys[iPlayer].wKeys1[KEY_INCAR_LOOKL]) {
			pCamera->SetDriveByLeft(1);
		} else {
			pCamera->SetDriveByLeft(0);
		}	

		if(GcsRemotePlayerKeys[iPlayer].wKeys1[KEY_INCAR_LOOKR]) {
			pCamera->SetDriveByRight(1);
		} else {
			pCamera->SetDriveByRight(0);
		}
	}
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetInternalKeys()
{
	return pGcsInternalKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetLocalPlayerKeys()
{
	return &GcsLocalPlayerKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetPlayerKeys(int iPlayer)
{
	return &GcsRemotePlayerKeys[iPlayer];
}

//-----------------------------------------------------------

void GameResetPlayerKeys(int iPlayer)
{
	memset(&GcsRemotePlayerKeys[iPlayer], 0, sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameResetLocalKeys()
{
	memset(pGcsInternalKeys, 0, sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------
