//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "../main.h"
#include "game.h"
#include "util.h"
#include "keystuff.h"

extern CNetGame * pNetGame;
extern CGame *    pGame;
BOOL              bScriptInited=FALSE;

#define NUDE void _declspec(naked) 

// TODO: hooks.h
#define VAR_CPlayerPed__VFTable 0x694D70
#define VAR_CBike__VFTable 0x6D7B34
#define VAR_CBoat__VFTable 0x69B0B4
#define VAR_CAutomobile__VFTable 0x69AD90
#define FUNC_CPlayerPed__ProcessControl 0x537270
#define FUNC_CBike__ProcessControl 0x60E3E0
#define FUNC_CBoat__ProcessControl 0x59FE90
#define FUNC_CAutomobile__ProcessControl 0x593030
#define CALLBACK_CRunningScript__Process 0x450245

//-----------------------------------------------------------

PED_TYPE	 * _pPlayer = NULL;
VEHICLE_TYPE * _pVehicle = NULL;
DWORD		 * _pEntity = NULL;
int			 _iWeapon = 0;
float		 _fUnk = 0;
int			 _iPedPieces = 0;
BYTE		 _byteUnk = 0;
DWORD	     dwStackFrame = 0;
DWORD	     dwCurPlayerActor = 0;
BYTE	     byteCurPlayer = 0;
DWORD        dwRGBARadar = 0;
int		     iRadarColor1 = 0;
BYTE	     byteSavedCameraMode = 0;
BYTE	   * pbyteCameraMode = (BYTE *)0x7E481C;
BYTE	   * pbyteCurrentPlayer = (BYTE *)0xA10AFB; 
BYTE	     byteUnkSave = 0;
DWORD        dwVFTable = 0;
DWORD        dwFunc = 0;
DWORD        dwObjectiveActor = 0;
DWORD        dwObjectiveType = 0;
DWORD        dwObjectiveEntity = 0;

extern GTA_CONTROLSET * pGcsInternalKeys;

//-----------------------------------------------------------

BYTE RadarTranslateColor_HookJmpCode[] = {0xFF,0x25,0x44,0x30,0x4C,0x00,0x90}; // 4C3044
BYTE EnterCarAnimCallback_HookJmpCode[] = {0xFF,0x25,0xD8,0x28,0x51,0x00,0x90,0x90}; // 5128D8
BYTE InflictDamage_HookJmpCode[] = {0xFF,0x25,0x15,0x5B,0x52,0x00}; // 525B15

//-----------------------------------------------------------

void _stdcall DoOnFootWorldBoundsStuff()
{
	if(pNetGame) {
		CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		CPlayerPed *pPlayerPed = pGame->FindPlayerPed();

		// Make sure they don't leave their worldy confines.
		if(pLocalPlayer->IsActive() && !pPlayerPed->IsInVehicle()) {
			if(pPlayerPed->EnforceWorldBoundries(
				pNetGame->m_WorldBounds[0],pNetGame->m_WorldBounds[1],
				pNetGame->m_WorldBounds[2],pNetGame->m_WorldBounds[3]))
			{
				pGcsInternalKeys->wKeys1[KEY_ONFOOT_JUMP] = 0xFF;
				pGcsInternalKeys->wKeys2[KEY_ONFOOT_JUMP] = 0xFF;
				pPlayerPed->SetArmedWeapon(0);
				//pGame->DisplayTextMessage("Stay within the world boundries");
			}
		}
	}
}

//-----------------------------------------------------------

void _stdcall SwitchContext(DWORD dwPedPtr, bool bPrePost)
{
	if(dwPedPtr)
	{
		byteCurPlayer = FindPlayerNumFromPedPtr(dwPedPtr);

		// Is this player not the local player?
		if(byteCurPlayer != 0)
		{
			// Is this before the ProcessControl call or after?
			if(bPrePost)
			{
				// Key switching
				GameStoreLocalPlayerKeys(); // Save local player's keys
				GameSetRemotePlayerKeys(byteCurPlayer); // Set remote player's keys

				// Camera mode switching
				byteSavedCameraMode = *pbyteCameraMode; // Save the local player's camera mode
				*pbyteCameraMode = 4; // Set the current camera mode to onfoot mouse looking mode

				// Aim switching
				GameStoreLocalPlayerAim(); // Save local player's aim
				GameSetRemotePlayerAim(byteCurPlayer); // Set remote player's aim

				// Current player index switching
				*pbyteCurrentPlayer = byteCurPlayer; // Set the player index to the current player
			}
			else
			{
				// Current player index switching
				*pbyteCurrentPlayer = 0; // Set the player index to the local player

				// Aim switching
				GameSetLocalPlayerAim(); // Restore the local player's aim

				// Camera mode switching
				*pbyteCameraMode = byteSavedCameraMode; // Restore the local player's camera mode
					
				// Key switching
				GameSetLocalPlayerKeys(); // Restore the local player's keys
			}
		}
		else
		{
			// Its the local player
		}
	}
}

//-----------------------------------------------------------
// A hook function that switches keys for
// CPlayerPed::ProcessControl(void)

NUDE CPlayerPed__ProcessControl_Hook()
{
	_asm
	{
		mov dwCurPlayerActor, ecx
		pushad
	}

	SwitchContext(dwCurPlayerActor, true);

	// call the internal CPlayerPed::ProcessControl
	dwFunc = FUNC_CPlayerPed__ProcessControl;
	_asm
	{
		popad
		call dwFunc
		pushad
	}

	SwitchContext(dwCurPlayerActor, false);

	_asm
	{
		popad
		ret
	}
}

//-----------------------------------------------------------
// Hook for C*::ProcessControl(void)

NUDE Vehicle_ProcessControl_Hook()
{
	_asm
	{
		mov _pVehicle, ecx
		mov eax, [ecx]
		mov dwVFTable, eax
		pushad
	}
	
	if(dwVFTable == VAR_CBike__VFTable)
	{
		dwFunc = FUNC_CBike__ProcessControl;
	}
	else if(dwVFTable == VAR_CBoat__VFTable)
	{
		dwFunc = FUNC_CBoat__ProcessControl;
	}
	else if(dwVFTable == VAR_CAutomobile__VFTable)
	{
		dwFunc = FUNC_CAutomobile__ProcessControl;
	}
	else
	{
		_asm
		{
			popad
			ret
		}
	}

	if(_pVehicle)
	{
		SwitchContext((DWORD)_pVehicle->pDriver, true);
	}

	// call C*::ProcessControl
	_asm
	{
		popad
		call dwFunc
		pushad
	}

	if(_pVehicle)
	{
		SwitchContext((DWORD)_pVehicle->pDriver, false);
	}

	_asm
	{
		popad
		ret
	}
}

//-----------------------------------------------------------

extern CChatWindow * pChatWindow;
DWORD dwLastObjectiveVehicle = 0;

void _stdcall DoVehicleEntryExitNotification(bool bEnterExit, DWORD dwVehicle, bool bPassenger)
{
	// Do we have a valid net game instance?
	if(pNetGame)
	{
		// Get a pointer to the local player ped
		CPlayerPed * pPlayerPed = pGame->FindPlayerPed();

		// Make sure we are not currently entering exiting a vehicle
		if((bEnterExit && dwLastObjectiveVehicle == dwVehicle && pPlayerPed->GetAction() == ACTION_GETTING_IN_VEHICLE) || (!bEnterExit && pPlayerPed->GetAction() == ACTION_EXITING_VEHICLE))
		{
			// Already entering or exiting a vehicle
			return;
		}

		if(pChatWindow) pChatWindow->AddDebugMessage("DoVehicleEntryExitNotification(%d, 0x%p, %d)", bEnterExit, dwVehicle, bPassenger);

		// Get a pointer to the vehicle pool
		CVehiclePool * pVehiclePool = pNetGame->GetVehiclePool();

		// Get the vehicle's pool id
		int iVehicleId = pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)dwVehicle);

		// Is the vehicle valid?
		if(iVehicleId != -1)
		{
			// Get a pointer to the local player
			CLocalPlayer * pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

			// Are we entering the vehicle or exiting it?
			if(bEnterExit)
			{
				// Send the enter vehicle notification
				pLocalPlayer->SendEnterVehicleNotification(iVehicleId, bPassenger);

				// Set the last object vehicle pointer to this vehicle to avoid spamming enter vehicle objectives
				dwLastObjectiveVehicle = dwVehicle;
			}
			else
			{
				// Send the exit vehicle notification
				pLocalPlayer->SendExitVehicleNotification(iVehicleId);
			}
		}
	}
}

//-----------------------------------------------------------
// Hooks CPed::SetObjective(eObjectiveType objectiveType, CEntity * pObjectiveEntity)

NUDE CPed__SetObjective_Hook()
{	
	_asm
	{
		mov dwObjectiveActor, ecx
		mov eax, [esp+4]
		mov dwObjectiveType, eax
		mov eax, [esp+8]
		mov dwObjectiveEntity, eax
		pushad
	}

	if((PED_TYPE *)dwObjectiveActor == GamePool_FindPlayerPed())
	{
		if(dwObjectiveType == OBJECTIVE_TYPE_ENTER_CAR_AS_DRIVER)
		{
			DoVehicleEntryExitNotification(true, dwObjectiveEntity, false);
		}
		else if(dwObjectiveType == OBJECTIVE_TYPE_ENTER_CAR_AS_PASSENGER)
		{
			DoVehicleEntryExitNotification(true, dwObjectiveEntity, true);
		}
		else if(dwObjectiveType == OBJECTIVE_TYPE_EXIT_CAR)
		{
			DoVehicleEntryExitNotification(false, dwObjectiveEntity, false);
		}
	}

	dwFunc = (FUNC_CPed__SetObjective + 9);
	_asm
	{
		popad
		push ebx
		mov ebx, ecx
		mov eax, [ebx+244h]
		jmp dwFunc
	}
}

//-----------------------------------------------------------

NUDE RadarTranslateColor()
{
	_asm
	{
		mov eax, [esp+4]
		mov iRadarColor1, eax
	}

	TranslateColorCodeToRGBA(iRadarColor1); // return will still be in eax.

	_asm
	{
		ret
	}
}

//-----------------------------------------------------------

NUDE CantFindFuckingAnim()
{
	_asm
	{
		mov eax, [esp+4]
		test eax, eax
		jnz its_ok

		ret ; was 0, so foobarred
	}

its_ok:
	_asm
	{
		mov edx, 0x405AC0
		add edx, 4
	}
}

//-----------------------------------------------------------
// ok, this bullshit procedure don't check the fucking
// vehicle pointer for 0 and caused the widely hated 5128fb crash.

NUDE EnterCarAnimCallback_Hook()
{
	_asm
	{
		mov edx, [esp+4]
		mov eax, [esp+8]
		mov _pPlayer, eax
		pushad
	}

	if(_pPlayer->pVehicle == 0 && _pPlayer != GamePool_FindPlayerPed())
	{
		_asm
		{
			popad
			ret
		}
	}

	_asm
	{
		popad
		mov ebp, 0x5128E0
		add ebp, 8
		jmp ebp
	}
}

//-----------------------------------------------------------

#define NO_TEAM 255

BOOL _stdcall IsFriendlyFire(PED_TYPE *pPlayer,DWORD *pdwEnt, int iWeapon, float fUnk, int PedPiece, BYTE byteUnk)
{
	CPlayerPool *pPlayerPool;
	CVehiclePool *pVehiclePool;
	CRemotePlayer *pRemotePlayer;
	CVehicle *pVehicle;
	CLocalPlayer *pLocalPlayer;
	BYTE byteRemoteSystemAddress;
	BYTE byteLocalPlayerTeam;

	if(pPlayer == GamePool_FindPlayerPed()) {
		if(pNetGame && pNetGame->GetFriendlyFire()) {
			pPlayerPool = pNetGame->GetPlayerPool();
			pLocalPlayer = pPlayerPool->GetLocalPlayer();
			byteLocalPlayerTeam = pLocalPlayer->GetTeam();

			if(pLocalPlayer->IsWasted() || (byteLocalPlayerTeam == NO_TEAM)) return FALSE;

			byteRemoteSystemAddress = pPlayerPool->FindPlayerIDFromGtaPtr((PED_TYPE *)pdwEnt);

			if(byteRemoteSystemAddress != INVALID_PLAYER_ID) {
				pRemotePlayer = pPlayerPool->GetAt(byteRemoteSystemAddress);
				if(pRemotePlayer->GetTeam() == byteLocalPlayerTeam) {
					return TRUE;
				} else {
					return FALSE;
				}
			}

			pVehiclePool = pNetGame->GetVehiclePool();

			int iVehicleId = pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)pdwEnt);

			if(iVehicleId != -1) {
				pVehicle = pVehiclePool->GetAt(iVehicleId);
				if(pVehicle->GetDriver()) {
					byteRemoteSystemAddress = pPlayerPool->FindPlayerIDFromGtaPtr((PED_TYPE *)pdwEnt);
					if(byteRemoteSystemAddress != INVALID_PLAYER_ID) {
						pRemotePlayer = pPlayerPool->GetAt(byteRemoteSystemAddress);
						if(pRemotePlayer->GetTeam() == byteLocalPlayerTeam) {
							return TRUE;
						}
					}
				} else {
					return FALSE;
				}
			}
		}
	}

	return FALSE;	
}

//-----------------------------------------------------------

NUDE CPed__InflictDamage_Hook()
{
	_asm
	{
		mov dwStackFrame, esp
		mov _pPlayer, ecx
		mov eax, [esp+4]
		mov _pEntity, eax
		mov eax, [esp+8]
		mov _iWeapon, eax
		mov eax, [esp+12]
		mov _fUnk, eax
		mov eax, [esp+16]
		mov _iPedPieces, eax
		mov al, [esp+20]
		mov _byteUnk, al
		pushad
	}

	if(pNetGame)
	{
		if(IsFriendlyFire(_pPlayer,_pEntity,_iWeapon,_fUnk,_iPedPieces,_byteUnk))
		{
			_asm
			{
				popad
				mov esp, dwStackFrame
				xor al, al
				retn 0x14
			}
		}

		/*
		if(_pPlayer == GamePool_FindPlayerPed()) {
			fLastDamageAmount = _fUnk;
			fLastHealth = _pPlayer->fHealth - fLastDamageAmount;
			bLastDamageProcessed = FALSE;
		}*/
	}

	_asm
	{
		popad
		mov esp, dwStackFrame
		fld ds:[0x694170]
		mov edx, 0x525B20
		add edx, 6
		jmp edx
	}
}

//-----------------------------------------------------------

void CRunningScript_Process()
{
	if(!bScriptInited)
	{
		// Code from VCMP.SCM (Minus the pickups and teleports)
		DWORD PLAYER_ACTOR = 0;
		DWORD PLAYER_CHAR = 0;
		int iPlayerNumber = 0;
		ScriptCommand(&name_thread, "MAIN");
		ScriptCommand(&set_current_time, 12, 0);
		ScriptCommand(&create_player, &iPlayerNumber, -1000.0, 195.5, 11.0, &PLAYER_ACTOR);
		ScriptCommand(&create_actor_from_player, &PLAYER_ACTOR, &PLAYER_CHAR);
		ScriptCommand(&create_forbidden_for_peds_cube, -100000.0f, -100000.0f, -100000.0f, 100000.0f, 100000.0f, 100000.0f);
		ScriptCommand(&create_forbidden_for_cars_cube, -100000.0f, -100000.0f, -100000.0f, 100000.0f, 100000.0f, 100000.0f);
		ScriptCommand(&set_max_wanted_level, 0);
		ScriptCommand(&toggle_player_controllable, PLAYER_ACTOR, 1);
		ScriptCommand(&force_weather, 0);
		ScriptCommand(&fade, 0, 0);
		bScriptInited = TRUE;
	}
}

//-----------------------------------------------------------

NUDE CRunningScript__Process_Hook()
{
	_asm
	{
		pushad
	}

	CRunningScript_Process();

	_asm
	{
		popad
		retn
	}
}

//-----------------------------------------------------------

void StartGame()
{
	pGame->StartGame();
	pGame->SetGameState(GS_MENUPROCESS);
}

//-----------------------------------------------------------

void InstallMethodHook(DWORD dwInstallAddress,DWORD dwHookFunction)
{
	Unprotect(dwInstallAddress, 4);
	*(DWORD *)dwInstallAddress = (DWORD)dwHookFunction;
}

//-----------------------------------------------------------

void InstallHook(DWORD dwInstallAddress, DWORD dwHookFunction, DWORD dwHookStorage, BYTE * pbyteJmpCode, int iJmpCodeSize)
{
	// Install the pointer to procaddr.
	Unprotect(dwHookStorage, 4);
	*(PDWORD)dwHookStorage = (DWORD)dwHookFunction;

	// Install the Jmp code.
	Unprotect(dwInstallAddress, iJmpCodeSize);
	memcpy((PVOID)dwInstallAddress,pbyteJmpCode,iJmpCodeSize);
}

//-----------------------------------------------------------

void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction)
{
	Unprotect(dwInstallAddress, 5);
	*(BYTE *)dwInstallAddress = 0xE8;
	*(DWORD *)(dwInstallAddress + 1) = (dwHookFunction - (dwInstallAddress + 5));
}

//-----------------------------------------------------------

void InstallJmpHook(DWORD dwInstallAddress, DWORD dwHookFunction)
{
	Unprotect(dwInstallAddress, 5);
	*(BYTE *)dwInstallAddress = 0xE9;
	*(DWORD *)(dwInstallAddress + 1) = (dwHookFunction - (dwInstallAddress + 5));
}

//-----------------------------------------------------------

void GameInstallHooks()
{	
	// Install hook for CPlayerPed::ProcessControl
	InstallMethodHook((VAR_CPlayerPed__VFTable + 0x20), (DWORD)CPlayerPed__ProcessControl_Hook);

	// Install hook for CBike::ProcessControl
	InstallMethodHook((VAR_CBike__VFTable + 0x20), (DWORD)Vehicle_ProcessControl_Hook);

	// Install hook for CBoat::ProcessControl
	InstallMethodHook((VAR_CBoat__VFTable + 0x20), (DWORD)Vehicle_ProcessControl_Hook);

	// Install hook for CAutomobile::ProcessControl
	InstallMethodHook((VAR_CAutomobile__VFTable + 0x20), (DWORD)Vehicle_ProcessControl_Hook);

	// Install hook for CPed::SetObjective
	InstallJmpHook(FUNC_CPed__SetObjective, (DWORD)CPed__SetObjective_Hook);
								
	// Install hook for RadarTranslateColor
	InstallHook(0x4C3050, (DWORD)RadarTranslateColor, 0x4C3044, 
		RadarTranslateColor_HookJmpCode, sizeof(RadarTranslateColor_HookJmpCode));
	
	// Install hook for CPed::InflictDamage
	InstallHook(0x525B20, (DWORD)CPed__InflictDamage_Hook, 0x525B15, 
		InflictDamage_HookJmpCode, sizeof(InflictDamage_HookJmpCode));

	// Install hook for enter car animation callback..
	// Update: Causing even more problems.
	InstallHook(0x5128E0, (DWORD)EnterCarAnimCallback_Hook, 0x5128D8, 
		EnterCarAnimCallback_HookJmpCode, sizeof(EnterCarAnimCallback_HookJmpCode));

	/* Hook/patch code to get around 0x405AC5 animation bug
	InstallHook(0x405AC0, (DWORD)CantFindFuckingAnim, 0x405A95,
		CantFindFuckingAnim_HookJmpCode, sizeof(CantFindFuckingAnim_HookJmpCode));*/

	// Install hook for CRunningScript::Process (thx Merlin)
	InstallCallHook(CALLBACK_CRunningScript__Process, (DWORD)CRunningScript__Process_Hook);

	// Install hook for menu process game state setting
	InstallCallHook(0x6003B3, (DWORD)StartGame);
}

//-----------------------------------------------------------
