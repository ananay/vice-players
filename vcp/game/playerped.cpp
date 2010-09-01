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
// Copyright 2010 VC-Players Team
//
// File Author(s): Christopher
//
//----------------------------------------------------------

#include "../../raknet/WindowsIncludes.h"

#include "../main.h"
#include "game.h"
#include "util.h"
#include "pools.h"
#include "keystuff.h"
#include "aimstuff.h"
#include "physical.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CChatWindow   *pChatWindow;

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
		_asm
		{
			mov ecx, pPed
			mov ebx, [ecx] ; vtable
			push 1
			call [ebx+8] ; destroy
		}
		SetEntity(NULL);
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetKeys(WORD wKeys)
{
	GTA_CONTROLSET * pPlayerKeys = GameGetPlayerKeys(m_bytePlayerNumber);

	memcpy(pPlayerKeys->wKeys2, pPlayerKeys->wKeys1, (sizeof(WORD) * 19));
	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_INCAR_HANDBRAKE] = 0xFF;
	} else { 
		pPlayerKeys->wKeys1[KEY_INCAR_HANDBRAKE] = 0x00;
	}
	wKeys >>= 1; // 1

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKR] = 0xFF;
	} else { 
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKR] = 0x00;
	}
	wKeys >>= 1; // 2

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKL] = 0xFF;
	} else { 
		pPlayerKeys->wKeys1[KEY_INCAR_LOOKL] = 0x00;
	}
	wKeys >>= 1; // 3

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0xFF80;
	} else {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0x0;
	}		
	wKeys >>= 1; // 4

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0x80;
	} else {
		pPlayerKeys->wKeys1[KEY_INCAR_TURRETUD] = 0x0;
	}		
	wKeys >>= 1; // 5

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_CROUCH] = 0xFF;
	} else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_CROUCH] = 0x00;
	}
	wKeys >>= 1; // 6

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_FIRE] = 0xFF;
	} else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_FIRE] = 0x00;
	}
	wKeys >>= 1; // 7

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_SPRINT] = 0xFF;
	} else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_SPRINT] = 0x00;
	}
	wKeys >>= 1; // 8

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_JUMP] = 0xFF;
	} else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_JUMP] = 0x00;
	}
	wKeys >>= 1; // 9

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_RIGHT] = 0xFF;
	} else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_RIGHT] = 0x00;
	}
	wKeys >>= 1; // 10

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_LEFT] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_LEFT] = 0x00;
	}
	wKeys >>= 1; // 11

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_BACKWARD] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_BACKWARD] = 0x00;
	}
	wKeys >>= 1; // 12

	if(wKeys & 1) {
		pPlayerKeys->wKeys1[KEY_ONFOOT_FORWARD] = 0xFF;
	}
	else {
		pPlayerKeys->wKeys1[KEY_ONFOOT_FORWARD] = 0x00;
	}

	GameStoreRemotePlayerKeys(m_bytePlayerNumber,pPlayerKeys);
}

//-----------------------------------------------------------

WORD CPlayerPed::GetKeys()
{
	WORD wKeys = 0;

	GTA_CONTROLSET * pInternalKeys = GameGetInternalKeys();

	if(pInternalKeys->wKeys1[KEY_ONFOOT_FORWARD]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_BACKWARD]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_LEFT]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_RIGHT]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_JUMP]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_SPRINT]) wKeys |= 1;
	wKeys <<= 1;

	// As ammo is not synced, don't send the fire key if they have no ammo
	// or they will fire their weapon for remote players but not themselves
	if(HasAmmoForCurrentWeapon() && pInternalKeys->wKeys1[KEY_ONFOOT_FIRE]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_ONFOOT_CROUCH]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_INCAR_TURRETUD] == 0x80) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_INCAR_TURRETUD] == 0xFF80) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_INCAR_LOOKL]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_INCAR_LOOKR]) wKeys |= 1;
	wKeys <<= 1;

	if(pInternalKeys->wKeys1[KEY_INCAR_HANDBRAKE]) wKeys |= 1;

	return wKeys;
}

//-----------------------------------------------------------

bool CPlayerPed::IsFiring()
{
	GTA_CONTROLSET * pInternalKeys = GameGetInternalKeys();

	return (pInternalKeys->wKeys1[KEY_ONFOOT_FIRE] != 0);
}

//-----------------------------------------------------------

CAMERA_AIM * CPlayerPed::GetCurrentAim()
{
	CCamera * pCamera = pGame->GetCamera();
	
	if(pCamera)
	{
		return pCamera->GetAim();
	}

	return NULL;
}

//-----------------------------------------------------------

void CPlayerPed::SetCurrentAim(CAMERA_AIM * pAim)
{
	GameStoreRemotePlayerAim(m_bytePlayerNumber, pAim);
}

//-----------------------------------------------------------

BOOL CPlayerPed::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
	Vector3 vPos;
	Vector3 vecMoveSpeed;

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

		DWORD dwFunc = ADDR_TIE_MARKER_TO_ACTOR;
		_asm
		{
			push 2
			push 4
			mov eax, dwPedID
			push eax
			push 2
			call dwFunc
			mov hndMarker, eax
			pop ecx
			pop ecx
			pop ecx
			pop ecx
		}

		dwFunc = ADDR_SET_MARKER_COLOR;
		_asm
		{
			push iMarkerColor
			push hndMarker
			call dwFunc
			pop ecx
			pop ecx
		}

		dwFunc = ADDR_SHOW_ON_RADAR1;
		_asm
		{
			push 2
			push hndMarker
			call dwFunc
			pop ecx
			pop ecx
		}
	}
}

//-----------------------------------------------------------

BOOL CPlayerPed::IsOnScreen()
{
	if(GetEntity()) {
		return CEntity::IsOnScreen();
	}
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::Say(UINT uiNum)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed)
	{
		DWORD dwFunc = FUNC_CPed__Say;
		_asm
		{
			push uiNum
			mov ecx, pPed
			call dwFunc
		}
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

/*
void CPlayerPed::SetGameSpeed(float gSpeed)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->gSpeed = gSpeed;
	}

	ScriptCommand(&set_game_speed, gSpeed);
}	
*/
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

#define PI 3.1415927f

void CPlayerPed::SetRotation(float fRotation)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && !pPed->byteIsInVehicle) {
		float fRot = (PI * fRotation * 0.0055555557f);
		pPed->fRotation1 = pPed->fRotation2 = fRot;
		SetHeading(fRot);
	}
	//ScriptCommand(&set_player_z_angle, m_bytePlayerNumber, fRotation);
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
		DWORD dwFunc = FUNC_CPed__Teleport;
		_asm
		{
			push z
			push y
			push x
			mov ecx, pPed
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::ClearTargetAndVehicle()
{
	
}

//-----------------------------------------------------------

BOOL CPlayerPed::GiveWeapon(int iWeaponID, int iAmmo)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		int iModelID = GameGetWeaponModelFromWeapon(iWeaponID);
		if(iModelID != -1) {
			if(!pGame->IsModelLoaded(iModelID)) {
				pGame->RequestModel(iModelID);
				pGame->LoadRequestedModels();
				while(!pGame->IsModelLoaded(iModelID)) Sleep(1);
			}

			DWORD dwFunc = FUNC_CPed__GiveWeapon;
			_asm
			{
				push 1
				push iAmmo
				push iWeaponID
				mov ecx, pPed
				call dwFunc
			}

			SetArmedWeapon(iWeaponID);
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

void CPlayerPed::ClearAllWeapons()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		DWORD dwFunc = FUNC_CPed__RemoveAllWeapons;
		_asm
		{
			mov ecx, pPed
			call dwFunc
		}
	}
}





// -- ADDED BY THE VC-Players Team -- //


void CPlayerPed::SetWaterDeaths(int iToggle)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		ScriptCommand(&water_deaths, m_dwGTAId, iToggle);
	}
}	

//-----------------------------------------------------------

void CPlayerPed::SetCellAction(int iToggle)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		ScriptCommand(&load_model, 258);
		ScriptCommand(&cell_phone, m_dwGTAId, iToggle);
	}
}	

//-----------------------------------------------------------

void CPlayerPed::Flash(int iItem)
{
	ScriptCommand(&flash_item, iItem);
	pChatWindow->AddDebugMessage("Flashing called");
}

//-----------------------------------------------------------

void CPlayerPed::SetActorBleeding(int iToggle)
{
	DWORD dwSystemAddress = m_bytePlayerNumber;

	ScriptCommand(&set_actor_bleeding, m_dwGTAId, iToggle);	
}

//-----------------------------------------------------------

void CPlayerPed::SetSkyColor(int iColorID, int iFade)
{
	ScriptCommand(&set_ped_sky_color, iColorID, iFade);
}

//-----------------------------------------------------------

void CPlayerPed::SetDrivebyState(int iToggle)
{
	ScriptCommand(&set_driveby_state, m_dwGTAId, iToggle);
}






BOOL CPlayerPed::SetArmedWeapon(int iWeaponType)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		for(BYTE i = 0; i < 10; i++) {
			if(pPed->weaponSlots[i].dwType == iWeaponType) {
				pPed->byteCurWepSlot = i;
				return TRUE;
			}
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

int CPlayerPed::GetCurrentWeapon()
{
	if(GetEntity()) {
		return GetCurrentWeaponSlot()->dwType;
	}
	return 0;
}

//-----------------------------------------------------------

BOOL CPlayerPed::HasAmmoForCurrentWeapon()
{
	if(GetEntity()) {
		WEAPON_SLOT * pWeapon = GetCurrentWeaponSlot();

		// melee weapons always have ammo
		if(pWeapon->dwType < 12) {
			return TRUE;
		}

		if(pWeapon->dwAmmo) {
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

WEAPON_SLOT * CPlayerPed::GetCurrentWeaponSlot()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		return &pPed->weaponSlots[pPed->byteCurWepSlot];
	}
	return NULL;
}

//-----------------------------------------------------------

WEAPON_SLOT * CPlayerPed::GetWeaponInSlot(BYTE byteSlot)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && (byteSlot >= 0 && byteSlot <= 10)) {
		return &pPed->weaponSlots[byteSlot];
	}
	return NULL;
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

		DWORD dwFunc = ADDR_ACTOR_PUT_IN_VEHICLE;
		_asm
		{
			mov     eax, pVehicle
			add	    eax, uiPassengerOffset
			push    eax
			mov     eax, pVehicle
			mov     ecx, pPed
			call    dwFunc
		}

		dwFunc = ADDR_ACTOR_PUT_IN_VEHICLE;
		_asm
		{
			mov     ebx, pPed
			mov     eax, pVehicle
			mov     [ebx+936], eax
			lea     eax, [ebx+936]
			mov     ecx, [ebx+936]
			push    eax
			call    dwFunc
		}

		pPed->byteIsInVehicle = 1;
		pPed->physical.entity.nControlFlags2 &= 0xFE;

		dwFunc = ADDR_VEHICLE_SET_DRIVER;
		_asm
		{
			push 0
			push pVehicle
			mov ecx, pPed
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetModel(int iModel)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();

	if(pPed)
	{
		PCHAR szModelName=0;

		if(iModel == 8) return; // invalid skin
		
		if(iModel > 106)
		{
			iModel -= 106;

			if(iModel < 54)
			{
				szModelName = szSpecialActorModels[iModel];

				DWORD dwFunc = FUNC_CPed__ResetSkin;
				_asm
				{
					push szModelName
					mov ecx, pPed
					call dwFunc
				}

				dwFunc = ADDR_LOAD_REQUESTED_MODELS2;
				_asm
				{
					push 0
					call dwFunc
					pop ecx
				}

				dwFunc = FUNC_CPed__Recreate;
				_asm
				{
					mov ecx, pPed
					call dwFunc
				}
			}
		}
		else // default.ide number
		{
			if(!pGame->IsModelLoaded(iModel))
			{
				pGame->RequestModel(iModel);
				pGame->LoadRequestedModels();
				while(!pGame->IsModelLoaded(iModel)) Sleep(1);
			}

			DWORD dwFunc = FUNC_CPed__SetModelIndex;
			_asm
			{
				push iModel
				mov ecx, pPed
				call dwFunc
			}
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::SetObjective(PDWORD pObjectiveEntity, eObjectiveType objectiveType)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed && pObjectiveEntity) {
		DWORD dwFunc = FUNC_CPed__SetObjective;
		_asm
		{
			push pObjectiveEntity
			push objectiveType
			mov ecx, pPed
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

void CPlayerPed::RestartIfWastedAt(Vector3 *vecRestart, float fRotation)
{
	ScriptCommand(&restart_if_wasted_at, vecRestart->X, vecRestart->Y, vecRestart->Z, fRotation);
}

//-----------------------------------------------------------

void CPlayerPed::TogglePlayerControllable(int iControllable)
{
	ScriptCommand(&toggle_player_controllable, m_bytePlayerNumber, iControllable);
}

//-----------------------------------------------------------

void CPlayerPed::SetDead()
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		pPed->fHealth = 0.0f;
		DWORD dwFunc = 0x4F6430;
		_asm
		{
			mov ecx, pPed
			call dwFunc
		}
	}
}

//-----------------------------------------------------------

BYTE CPlayerPed::FindDeathReasonAndResponsiblePlayer(BYTE * nPlayer)
{
	PED_TYPE * pPed = (PED_TYPE *)GetEntity();
	if(pPed) {
		BYTE byteDeathReason;
		EntityId playerIDWhoKilled;
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
					
					playerIDWhoKilled = pPlayerPool->
						FindPlayerIDFromGtaPtr((PED_TYPE *)pPed->pDamageEntity);

					if(playerIDWhoKilled != INVALID_PLAYER_ID) {
						// killed by another player with a weapon, this is all easy.
						*nPlayer = playerIDWhoKilled;
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

						playerIDWhoKilled = pPlayerPool->
							FindPlayerIDFromGtaPtr((PED_TYPE *)pGtaVehicle->pDriver);
												
						if(playerIDWhoKilled != INVALID_PLAYER_ID) {
							*nPlayer = playerIDWhoKilled;
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
											
						playerIDWhoKilled = pPlayerPool->
							FindPlayerIDFromGtaPtr((PED_TYPE *)pGtaVehicle->pDriver);
							
						if(playerIDWhoKilled != INVALID_PLAYER_ID) {
							*nPlayer = playerIDWhoKilled;
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