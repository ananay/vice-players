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
// File Author: kyeman
//
//----------------------------------------------------------

#include "../../raknet/WindowsIncludes.h"

#include "../main.h"
#include "game.h"
#include "util.h"
#include "pools.h"
#include "keystuff.h"
#include "aimstuff.h"

extern CGame *pGame;
extern CNetGame *pNetGame;

//-----------------------------------------------------------

char szSpecialActorModels[][32] =
{"igdiaz","igpercy","igken","igcandy","ighlary","igjezz","igphil",
"sam","igsonny","igbuddy","igmerc","igdick","igalscb","igbudy2",
"igbudy3","igcolon","igdick","iggonz","ighlry2","igmerc2","igmike",
"igmike2", "igphil2", "igphil3","bounca","burger","cdrivra","cdrivrb",
"cgona","cgonb","cgonc","chef","cmraman","courier","crewa","crewb","cservra",
"dgoona","dgoonb","mba","mbb","mgoona","mporna","printra","printrb","printrc","psycho",
"sgc","spandxa","spandxb","stripa","stripb","stripc","S_keep"};

//-----------------------------------------------------------
// Obtain an instance of CPlayerPed for the local player.

CPlayerPed::CPlayerPed()
{
	m_dwGTAId = 1; // 0x001
	SetEntity((ENTITY_TYPE *)GamePool_FindPlayerPed());
	m_bytePlayerNumber = 0;
}

//-----------------------------------------------------------
// This is the constructor for creating new player.

CPlayerPed::CPlayerPed( int iPlayerNumber, int iModel, float fX,
					    float fY,float fZ,float fRotation )
{
	DWORD dwSystemAddress=0;

	SetEntity(NULL);
	m_dwGTAId=0;
	m_bytePlayerNumber = (BYTE)iPlayerNumber;

	Create(iModel, fX, fY, fZ, fRotation);
}

//-----------------------------------------------------------

CPlayerPed::~CPlayerPed()
{

}

//-----------------------------------------------------------

PED_TYPE *CPlayerPed::GetPed()
{
	return (PED_TYPE *)GetEntity();
}

//-----------------------------------------------------------

void CPlayerPed::Create(int iModel, float fX, float fY,float fZ,float fRotation)
{
	if((iModel < 107) && !pGame->IsModelLoaded(iModel)) {
		pGame->RequestModel(iModel);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iModel)) { Sleep(1); }
	}

	// TODO: I think this is wrong (the opcode calls), if it is fix it
	// (will also need to fix it in the running script process hook)
	DWORD dwPlayerHandle;
	int iPlayerNumber = m_bytePlayerNumber;
	ScriptCommand(&create_player, &iPlayerNumber, fX, fY, fZ, &dwPlayerHandle);
	ScriptCommand(&create_actor_from_player, &iPlayerNumber, &dwPlayerHandle);
	ScriptCommand(&toggle_player_infinite_run, iPlayerNumber, 1);
	SetRotation(fRotation);

	m_dwGTAId = dwPlayerHandle;
	SetEntity((ENTITY_TYPE *)CPools::GetPedFromIndex(m_dwGTAId));
	SetPlayerPedPtrRecord(m_bytePlayerNumber, (DWORD)GetEntity());
	ScriptCommand(&set_actor_immunities, m_dwGTAId, 1, 1, 1, 1, 1);

	SetModel(iModel);
}

//-----------------------------------------------------------

void CPlayerPed::Destroy()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		_asm mov ecx, pPed
		_asm mov ebx, [ecx] ; vtable
		_asm push 1
		_asm call [ebx+8] ; destroy
		SetEntity(NULL);
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetKeys(WORD wKeys)
{
	GTA_CONTROLSET * pPlayerKeys = GameGetPlayerKeys(m_bytePlayerNumber);

	memcpy(&pPlayerKeys->previousPadState, &pPlayerKeys->currentPadState, sizeof(GTA_CONTROL));
	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton19 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton19 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton18 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton18 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton17 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton17 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton16 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton16 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton15 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton15 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton14 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton14 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton11 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton11 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton10 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton10 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton9 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton9 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton8 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton8 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton7 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton7 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton6 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton6 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton5 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton5 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton3 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton3 = 0x00;
	}
	wKeys >>= 1;

	if(wKeys & 1) {
		pPlayerKeys->previousPadState.wButton2 = 0xFF;
	} else { 
		pPlayerKeys->previousPadState.wButton2 = 0x00;
	}
	wKeys >>= 1;

	GameStoreRemotePlayerKeys(m_bytePlayerNumber,pPlayerKeys);
}

//-----------------------------------------------------------

WORD CPlayerPed::GetKeys()
{
	WORD wKeys = 0;

	GTA_CONTROLSET * pInternalKeys = GameGetInternalKeys();

	if(pInternalKeys->currentPadState.wButton2) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton3) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton5) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton6) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton7) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton8) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton9) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton10) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton11) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton14) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton15) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton16) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton17) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton18) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->currentPadState.wButton19) wKeys |= 1;
	wKeys <<= 1;

	return wKeys;
}

//-----------------------------------------------------------

CAMERA_AIM * CPlayerPed::GetCurrentAim()
{
	return GameGetInternalAim();
}

//-----------------------------------------------------------

void CPlayerPed::SetCurrentAim(CAMERA_AIM *pAim)
{
	GameStoreRemotePlayerAim(m_bytePlayerNumber, pAim);
}

//-----------------------------------------------------------

BOOL CPlayerPed::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	VECTOR vPos;
	VECTOR vecMoveSpeed;

	if(!GetEntity()) return FALSE;

	GetPosition(&vPos);
	GetMoveSpeed(&vecMoveSpeed);

	if(vPos.X > fPX) // greatest X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = -0.25f;
		}
		SetMoveSpeed(vecMoveSpeed);
		return TRUE;
	}
	else if(vPos.X < fZX)  // least X coord check
	{
		if(vecMoveSpeed.X != 0.0f) {
			vecMoveSpeed.X = 0.25f;
		}
		SetMoveSpeed(vecMoveSpeed);
		return TRUE;
	}
	else if(vPos.Y > fPY) // Y coord check
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = -0.25f;
		}

		SetMoveSpeed(vecMoveSpeed);
		return TRUE;
	}
	else if(vPos.Y < fNY)
	{
		if(vecMoveSpeed.Y != 0.0f) {
			vecMoveSpeed.Y = 0.25f;
		}

		SetMoveSpeed(vecMoveSpeed);
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasAmmoForCurrentWeapon()
{
	int iAmmo;

	if(GetEntity()) {

		if(!GetEntity()) return FALSE;

		int iWeapon = (int)GetCurrentWeapon();

		// mellee weapons always have ammo
		if(iWeapon < 12) return TRUE;

		ScriptCommand(&get_player_weapon_ammo,m_bytePlayerNumber,iWeapon,&iAmmo);

		if(iAmmo) {
			return TRUE;
		} else {
			return FALSE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetCurrentWeapon()
{
	DWORD dwRetVal;
	if(!GetEntity()) return 0;
	ScriptCommand(&get_player_armed_weapon,m_bytePlayerNumber,&dwRetVal);
	return (BYTE)dwRetVal;
}

//-----------------------------------------------------------

void CPlayerPed::SetInitialState()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->byteJumpFlags = 0xA;
	}
}

//-----------------------------------------------------------

int CPlayerPed::GetCurrentVehicleID()
{
	PED_TYPE *pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return CPools::GetIndexFromVehicle((VEHICLE_TYPE *)pPed->pVehicle);
	}
	// Not sure about this one
	return -1;
}

//-----------------------------------------------------------

void CPlayerPed::ShowMarker(int iMarkerColor)
{	
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		DWORD hndMarker;
		DWORD dwPedID;
		float f=0.0f;

		dwPedID = CPools::GetIndexFromPed(pPed);

		_asm push 2
		_asm push 4
		_asm mov eax, dwPedID
		_asm push eax
		_asm push 2
		_asm mov edx, ADDR_TIE_MARKER_TO_ACTOR
		_asm call edx
		_asm mov hndMarker, eax
		_asm pop ecx
		_asm pop ecx
		_asm pop ecx
		_asm pop ecx

		_asm push iMarkerColor
		_asm push hndMarker
		_asm mov edx, ADDR_SET_MARKER_COLOR
		_asm call edx
		_asm pop ecx
		_asm pop ecx

		_asm push 2
		_asm push hndMarker
		_asm mov edx, ADDR_SHOW_ON_RADAR1
		_asm call edx
		_asm pop ecx
		_asm pop ecx
	}
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsOnScreen()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return GameIsEntityOnScreen((DWORD *)pPed);
	}
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::Say(UINT uiNum)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed)
	{
		DWORD dwPedPtr = (DWORD)pPed;
		_asm mov ecx, dwPedPtr
		_asm push uiNum
		_asm mov edx, FUNC_CPed__Say
		_asm call edx
	}
}

//-----------------------------------------------------------

float CPlayerPed::GetHealth()
{	
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return pPed->fHealth;
	}
	return 0.0f;
}

//-----------------------------------------------------------

void CPlayerPed::SetHealth(float fHealth)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->fHealth = fHealth;
	}
}	

//-----------------------------------------------------------

float CPlayerPed::GetArmour()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return pPed->fArmour;
	}
	return 0.0f;
}

//-----------------------------------------------------------

void CPlayerPed::SetArmour(float fArmour)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->fArmour = fArmour;
	}
}	

//-----------------------------------------------------------

BOOL CPlayerPed::IsDead()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && pPed->fHealth > 0.0f) {
			return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------

BYTE CPlayerPed::GetAction()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return pPed->byteAction;
	}
	return 0;
}

//-----------------------------------------------------------

void CPlayerPed::SetAction(BYTE byteTrigger)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->byteAction = byteTrigger;
	}
}
//-----------------------------------------------------------

BYTE CPlayerPed::GetShootingFlags()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return pPed->byteShootFlags;
	}
	return 0;
}

//-----------------------------------------------------------

void CPlayerPed::SetShootingFlags(BYTE byteShooting)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->byteShootFlags = byteShooting;
	}
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsInVehicle()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && pPed->byteIsInVehicle) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

float CPlayerPed::GetRotation()
{
	//return (m_pPed->fRotation1);
	float fRotation;
	ScriptCommand(&get_player_z_angle, m_bytePlayerNumber, &fRotation);
	return fRotation;
}

//-----------------------------------------------------------

void CPlayerPed::SetRotation(float fRotation)
{
	//m_pPed->fRotation1 = fRotation;
	//m_pPed->fRotation2 = fRotation;
	ScriptCommand(&set_player_z_angle, m_bytePlayerNumber, fRotation);
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsAPassenger()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && pPed->byteIsInVehicle && pPed->pVehicle)
	{
		VEHICLE_TYPE * pVehicle = (VEHICLE_TYPE *)pPed->pVehicle;

		if(pVehicle->pDriver != pPed) {
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------

VEHICLE_TYPE * CPlayerPed::GetGtaVehicle()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return (VEHICLE_TYPE *)pPed->pVehicle;
	}
	return NULL;
}

//-----------------------------------------------------------

void CPlayerPed::GiveWeapon(int iWeaponID, int iAmmo)
{
	int iModelID = GameGetWeaponModelFromWeapon(iWeaponID);

	if(!pGame->IsModelLoaded(iModelID)) {
		pGame->RequestModel(iModelID);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iModelID)) Sleep(1);
	}

	ScriptCommand(&give_player_weapon,m_bytePlayerNumber,iWeaponID,iAmmo);
	SetArmedWeapon(iWeaponID);
}

//-----------------------------------------------------------

void CPlayerPed::CheckAndRepairInvisProblems()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && (pPed->byteAction != ACTION_EXITING_VEHICLE) &&
		((pPed->byteIsInVehicle) || (pPed->byteAction == ACTION_DRIVING_VEHICLE)) ) {
		pPed->byteIsInVehicle = 0;
		pPed->byteAction = 1;
	}
}

//-----------------------------------------------------------

void CPlayerPed::Teleport(float x, float y, float z)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		DWORD dwPedPtr = (DWORD)pPed;
		_asm mov ecx, dwPedPtr
		_asm push z
		_asm push y
		_asm push x
		_asm mov edx, 0x4F5690
		_asm call edx
	}
}

//-----------------------------------------------------------

void CPlayerPed::ClearTargetAndVehicle()
{
	
}

//-----------------------------------------------------------

void CPlayerPed::ClearAllWeapons()
{
	ScriptCommand(&remove_player_weapons,m_bytePlayerNumber);
}

//-----------------------------------------------------------

void CPlayerPed::SetArmedWeapon(int iWeaponType)
{
	ScriptCommand(&set_player_armed_weapon,m_bytePlayerNumber,iWeaponType);
}

//-----------------------------------------------------------

void CPlayerPed::PutDirectlyInVehicle(int iVehicleID)
{
	DWORD dwSystemAddress = m_bytePlayerNumber;
	ScriptCommand(&put_player_in_car,dwSystemAddress,iVehicleID);
}

//-----------------------------------------------------------

void CPlayerPed::EnterVehicleAsDriver(int iVehicleID)
{
	VEHICLE_TYPE *pVehicle = CPools::GetVehicleFromIndex(iVehicleID);

	if(pVehicle) {
		SetObjective((PDWORD)pVehicle, OBJECTIVE_TYPE_ENTER_CAR_AS_DRIVER);
	}
}

//-----------------------------------------------------------

void CPlayerPed::EnterVehicleAsPassenger(int iVehicleID)
{
	VEHICLE_TYPE *pVehicle = CPools::GetVehicleFromIndex(iVehicleID);

	if(pVehicle) {
		SetObjective((PDWORD)pVehicle, OBJECTIVE_TYPE_ENTER_CAR_AS_PASSENGER);
	}
}

//-----------------------------------------------------------

void CPlayerPed::ExitCurrentVehicle()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && pPed->byteIsInVehicle) {
		SetObjective((PDWORD)pPed->pVehicle, OBJECTIVE_TYPE_EXIT_CAR);
	}
}

//-----------------------------------------------------------

void CPlayerPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		ScriptCommand(&put_player_at_and_remove_from_car,m_bytePlayerNumber,fX,fY,fZ);

		/*
		if(pPed->physical.entity.nModelIndex <= 106) {
			SetModel(m_pPed->physical.entity.nModelIndex); // attempts to bypass model destruction probs
		}*/
	}
}

//-----------------------------------------------------------

void CPlayerPed::ForceIntoPassengerSeat(UINT uiVehicleID, UINT uiSeat)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		VEHICLE_TYPE * pVehicle = CPools::GetVehicleFromIndex(uiVehicleID);
		UINT uiPassengerOffset;

		if(!pPed || !pVehicle) return;
		if(uiSeat > pVehicle->byteMaxPassengers) return;

		pPed->byteAction = 50;

		pVehicle->pPassengers[uiSeat-1] = pPed;
		uiPassengerOffset = 424 + (uiSeat * 4);

		_asm mov     eax, pVehicle
		_asm add	 eax, uiPassengerOffset
		_asm push    eax
		_asm mov     eax, pVehicle
		_asm mov     ecx, pPed
		_asm mov	 edx, ADDR_ACTOR_PUT_IN_VEHICLE
		_asm call    edx

		_asm mov	 ebx, pPed
		_asm mov     eax, pVehicle
		_asm mov     [ebx+936], eax
		_asm lea     eax, [ebx+936]
		_asm mov     ecx, [ebx+936]
		_asm push    eax
		_asm mov	 edx, ADDR_ACTOR_PUT_IN_VEHICLE
		_asm call    edx

		pPed->byteIsInVehicle = 1;
		pPed->physical.entity.nControlFlags2 &= 0xFE;

		_asm push 0
		_asm push pVehicle
		_asm mov ecx, pPed
		_asm mov edx, ADDR_VEHICLE_SET_DRIVER
		_asm call edx
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetModel(int iModel)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		PCHAR szModelName=0;

		if(iModel == 8) return; // invalid skin
		
		if(iModel > 106)
		{
			iModel -= 106;

			if(iModel < 54)
			{
				szModelName = szSpecialActorModels[iModel];

				_asm mov ecx, pPed
				_asm push szModelName
				_asm mov edx, FUNC_CPed__ResetSkin
				_asm call edx

				_asm push 0
				_asm mov edx, ADDR_LOAD_REQUESTED_MODELS2
				_asm call edx
				_asm pop ecx

				_asm mov ecx, pPed
				_asm mov edx, FUNC_CPed__Recreate
				_asm call edx
			}
		}
		else // default.ide number
		{
			if(!pGame->IsModelLoaded(iModel)) {
				pGame->RequestModel(iModel);
				pGame->LoadRequestedModels();
				while(!pGame->IsModelLoaded(iModel)) Sleep(1);
			}

			_asm mov ecx, pPed
			_asm push iModel
			_asm mov edx, FUNC_CPed__SetModelIndex
			_asm call edx
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetObjective(PDWORD pObjectiveEntity, eObjectiveType objectiveType)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && pObjectiveEntity) {
		_asm mov ecx, pPed
		_asm push pObjectiveEntity
		_asm push objectiveType
		_asm mov edx, FUNC_CPed__SetObjective
		_asm call edx
	}
}

//-----------------------------------------------------------

void CPlayerPed::RestartIfWastedAt(VECTOR *vecRestart, float fRotation)
{
	ScriptCommand(&restart_if_wasted_at,vecRestart->X,vecRestart->Y,
		vecRestart->Z,fRotation);
}

//-----------------------------------------------------------

void CPlayerPed::TogglePlayerControllable(int iControllable)
{
	ScriptCommand(&toggle_player_controllable,m_bytePlayerNumber,iControllable);
}

//-----------------------------------------------------------

void CPlayerPed::SetDead()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->fHealth = 0.0f;
		_asm mov ecx, pPed
		_asm mov edx, 0x4F6430
		_asm call edx
	}
}

//-----------------------------------------------------------

BYTE CPlayerPed::FindDeathReasonAndResponsiblePlayer(BYTE * nPlayer)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		BYTE byteDeathReason;
		BYTE byteSystemAddressWhoKilled;
		CVehiclePool *pVehiclePool;
		CPlayerPool *pPlayerPool;

		// grab the vehicle/player pool now anyway, even though we may not need it.
		if(pNetGame) {
			pVehiclePool = pNetGame->GetVehiclePool();
			pPlayerPool = pNetGame->GetPlayerPool();
		}
		else { // just leave if there's no netgame.
			*nPlayer = INVALID_PLAYER_ID;
			return INVALID_PLAYER_ID;
		}

		if(pPed) 
		{
			byteDeathReason = (BYTE)pPed->dwWeaponUsed;

			if(byteDeathReason < WEAPON_CAMERA) { // It's a weapon of some sort.

				if(pPed->pDamageEntity) { // check for a player pointer.
					
					byteSystemAddressWhoKilled = pPlayerPool->
						FindRemoteSystemAddressFromGtaPtr((PED_TYPE *)pPed->pDamageEntity);

					if(byteSystemAddressWhoKilled != INVALID_PLAYER_ID) {
						// killed by another player with a weapon, this is all easy.
						*nPlayer = byteSystemAddressWhoKilled;
						return byteDeathReason;
					}
				}
				else { // weapon was used but who_killed is 0 (?)
					*nPlayer = INVALID_PLAYER_ID;
					return INVALID_PLAYER_ID;
				}
			}
			else if(byteDeathReason == WEAPON_DROWN) {
				*nPlayer = INVALID_PLAYER_ID;
				return WEAPON_DROWN;
			}
			else if(byteDeathReason == WEAPON_DRIVEBY) {

				if(pPed->pDamageEntity) {
					// now, if we can find the vehicle
					// we can probably derive the responsible player.
					// Look in the vehicle pool for this vehicle.
					if(pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)pPed->pDamageEntity) != (-1))
					{
						VEHICLE_TYPE *pGtaVehicle = (VEHICLE_TYPE *)pPed->pDamageEntity;

						byteSystemAddressWhoKilled = pPlayerPool->
							FindRemoteSystemAddressFromGtaPtr((PED_TYPE *)pGtaVehicle->pDriver);
												
						if(byteSystemAddressWhoKilled != INVALID_PLAYER_ID) {
							*nPlayer = byteSystemAddressWhoKilled;
							return WEAPON_DRIVEBY;
						}
					}									
				}
			}
			else if(byteDeathReason == WEAPON_COLLISION) {

				if(pPed->pDamageEntity) {
					if(pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)pPed->pDamageEntity) != (-1))
					{
						VEHICLE_TYPE *pGtaVehicle = (VEHICLE_TYPE *)pPed->pDamageEntity;
											
						byteSystemAddressWhoKilled = pPlayerPool->
							FindRemoteSystemAddressFromGtaPtr((PED_TYPE *)pGtaVehicle->pDriver);
							
						if(byteSystemAddressWhoKilled != INVALID_PLAYER_ID) {
							*nPlayer = byteSystemAddressWhoKilled;
							return WEAPON_COLLISION;
						}
					}									
				}
			}
		}
	}

	// Unhandled death type.
	*nPlayer = INVALID_PLAYER_ID;
	return INVALID_PLAYER_ID;
}

//-----------------------------------------------------------

UINT CPlayerPed::GetPassengerSeat()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE *)pPed->pVehicle;

		if(pVehicle) {
			for(BYTE i = 0; i < pVehicle->byteMaxPassengers; i++) {
				if(pVehicle->pPassengers[i] == pPed) {
					return (i + 1);
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------