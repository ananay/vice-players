//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "../main.h"
#include "../game/keystuff.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

using namespace RakNet;
extern CNetGame* pNetGame;

//----------------------------------------------------------

CSpawnSelection::CSpawnSelection()
{
	m_iSelectedClass = 0;
	m_dwLastSpawnSelectionTick = GetTickCount();
	HandleClassSelection(pNetGame->GetPlayerPool()->GetLocalPlayer());
}

//----------------------------------------------------------

void CSpawnSelection::ProcessLocalPlayer(CLocalPlayer *pLocalPlayer)
{
	CCamera *pGameCamera;
	CPlayerPed *pGamePlayer;
	DWORD dwTicksSinceLastSelection;
	Vector3 vPlayerPos;
	float fDrawX=25.0f;
	float fDrawY=250.0f;
	char szMsg[256];

	if(FALSE == pLocalPlayer->IsActive() && !pLocalPlayer->IsWasted() && m_bClearedToSpawn)
	{
		// DONT ALLOW ANY ACTIONS IF WE'RE STILL FADING OR WAITING.
		if((GetTickCount() - m_dwInitialSelectionTick) < 2000) return;
		pGame->ToggleKeyInputsDisabled(TRUE);
		
		if(GetKeyState(VK_SHIFT)&0x8000) // SHIFT PRESSED SO SPAWN
		{		
			pGameCamera = pGame->GetCamera();
			pGamePlayer = pGame->FindPlayerPed();
			pGameCamera->Restore();
			pLocalPlayer->SpawnPlayer();
			pGame->DisplayHud(TRUE);
			pGame->ToggleHud(TRUE);
			pGame->HideRadar(FALSE);
			pGameCamera->SetBehindPlayer();
			pGamePlayer->GetPosition(&vPlayerPos);		
			pGame->PlaySound(10, vPlayerPos);
			pGame->ToggleKeyInputsDisabled(FALSE);
			return;
		}
		else if(m_bClearedToSpawn) // WE ARE CLEARED TO SPAWN OR SELECT ANOTHER CLASS
		{
			pGame->ToggleHud(FALSE);
			pGame->HideRadar(TRUE);

			// SHOW INFO ABOUT THE SELECTED CLASS..
			szMsg[0] = '\0';
			strcat(szMsg, "> Use Left and Right arrow keys to select a class.\n");
			strcat(szMsg, "> Press Shift button when ready to spawn.\n");

			CD3DFont *pD3DFont = pChatWindow->m_pD3DFont;
			pD3DFont->DrawText(fDrawX,fDrawY,0xFFFFFFFF,szMsg);

			// GRAB PLAYER MATRIX FOR SOUND POSITION
			pGamePlayer = pGame->FindPlayerPed();
			pGamePlayer->GetPosition(&vPlayerPos);

			pGame->DisplayHud(FALSE);

			dwTicksSinceLastSelection = GetTickCount() - m_dwLastSpawnSelectionTick; // used to delay reselection.

			// ALLOW ANOTHER SELECTION WITH LEFT KEY
			if( (GetKeyState(VK_LEFT)&0x8000) && (dwTicksSinceLastSelection > 250)) { // LEFT ARROW
				m_bClearedToSpawn = FALSE;
				m_dwLastSpawnSelectionTick = GetTickCount();
				m_iSelectedClass--;
		
				pGame->PlaySound(14, vPlayerPos);
				pLocalPlayer->RequestClass(m_iSelectedClass);
				return;
			}

			// ALLOW ANOTHER SELECTION WITH RIGHT KEY
			if( (GetKeyState(VK_RIGHT)&0x8000) && (dwTicksSinceLastSelection > 250)) { // RIGHT ARROW
	
				m_bClearedToSpawn = FALSE;
				m_dwLastSpawnSelectionTick = GetTickCount();
				m_iSelectedClass++;

				pGame->PlaySound(13, vPlayerPos);
				pLocalPlayer->RequestClass(m_iSelectedClass);
				return;
			}	

		}
	}
}

//----------------------------------------------------------

void CSpawnSelection::HandleClassSelection(CLocalPlayer *pLocalPlayer)
{
	CCamera *pGameCamera = pGame->GetCamera();
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();
	m_bClearedToSpawn = FALSE;

	pGame->DisplayHud(FALSE);

	if(pGameCamera)	{
		pGameCamera->SetPosition(pNetGame->m_vecInitCameraPos);
		pGameCamera->LookAtPoint(pNetGame->m_vecInitCameraLook, 1);
	}

	pLocalPlayer->RequestClass(m_iSelectedClass);
	m_dwInitialSelectionTick = GetTickCount();
	m_dwLastSpawnSelectionTick = GetTickCount();
}

//----------------------------------------------------------

void CSpawnSelection::HandleClassSelectionOutcome(PLAYER_SPAWN_INFO *pSpawnInfo, BOOL bOutcome)
{
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();

	if(bOutcome) {
		if(pGamePlayer)	{
			pGamePlayer->ClearAllWeapons();
			pGamePlayer->Teleport(pNetGame->m_vecInitPlayerPos.X, pNetGame->m_vecInitPlayerPos.Y, 
				pNetGame->m_vecInitPlayerPos.Z);
			pGamePlayer->SetRotation(0.0f);
			pGamePlayer->SetModel(pSpawnInfo->byteSkin);
			if(pSpawnInfo->iSpawnWeapons[0] != (-1)) {
				pGamePlayer->GiveWeapon(pSpawnInfo->iSpawnWeapons[0],pSpawnInfo->iSpawnWeaponsAmmo[0]);
			}
		}

		m_bClearedToSpawn = TRUE;
	}
}

//----------------------------------------------------------

void CSpawnSelection::SetSelectedClass(int iSelectedClass)
{
	m_iSelectedClass = iSelectedClass;
}

//----------------------------------------------------------