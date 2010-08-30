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

#include "../main.h"
#include "../game/util.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

using namespace RakNet;
extern CNetGame* pNetGame;

//----------------------------------------------------------

CLocalPlayer::CLocalPlayer()
{
	m_bHasSpawnInfo = FALSE;
	m_pPlayerPed = pGame->FindPlayerPed();
	m_bIsActive = FALSE;
	m_bIsWasted = FALSE;
	m_wLastKeys = 0;
	m_dwLastSendTick = GetTickCount();
	m_dwLastPassengerSendTick = GetTickCount();
}

//----------------------------------------------------------

BOOL CLocalPlayer::DestroyPlayer()
{
	return TRUE;
}

//----------------------------------------------------------

BOOL CLocalPlayer::Process()
{
	CGameModeGeneric *pGameLogic;
	CVehicle *pGameVehicle;
	CVehiclePool *pVehiclePool;

	BYTE byteVehicleID;

	DWORD dwThisTick;

	if(m_bIsActive && !m_bIsWasted && (NULL != m_pPlayerPed))
	{
		// If they're in a vehicle and their health reaches 0 we need to
		// kill them manually or they don't actually die
		if(m_pPlayerPed->IsInVehicle() && m_pPlayerPed->IsDead()) {
			m_pPlayerPed->SetDead();
		}

		if((m_pPlayerPed->GetAction() == ACTION_WASTED) && !m_bIsWasted)
		{
			m_bIsWasted = TRUE;
			m_bIsActive = FALSE;
			SendWastedNotification();
		}
		else
		{
			dwThisTick = GetTickCount();

			if(m_pPlayerPed->IsInVehicle())
			{
				if(!m_pPlayerPed->IsAPassenger())
				{
					// DRIVING VEHICLE

					pChatWindow->AddDebugMessage("In vehicle as driver");

					// VEHICLE WORLD BOUNDS STUFF
					pVehiclePool = pNetGame->GetVehiclePool();
					byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
					if(byteVehicleID != 255) {
						pGameVehicle = pVehiclePool->GetAt(byteVehicleID);
						pGameVehicle->EnforceWorldBoundries(
							pNetGame->m_WorldBounds[0],pNetGame->m_WorldBounds[1],
							pNetGame->m_WorldBounds[2],pNetGame->m_WorldBounds[3]);
					}

					if((dwThisTick - m_dwLastSendTick) > (UINT)GetOptimumInCarSendRate()) {
						m_dwLastSendTick = GetTickCount();
					
						// send nothing while we're getting out.
						if(m_pPlayerPed->GetAction() != ACTION_EXITING_VEHICLE) {
							SendInCarFullSyncData();
						}
					}

					pGame->DisablePassengerEngineAudio();
				}
				else
				{
					// PASSENGER

					if((dwThisTick - m_dwLastPassengerSendTick) > 1000) {
						m_dwLastPassengerSendTick = GetTickCount();
						SendInCarPassengerData();
					}

					pGame->EnablePassengerEngineAudio();
				}
			}
			else
			{
				// ONFOOT

				if((dwThisTick - m_dwLastSendTick) > (UINT)GetOptimumOnFootSendRate()) {
					m_dwLastSendTick = GetTickCount();

					// send nothing while we're getting in
					if(m_pPlayerPed->GetAction() != ACTION_DRIVING_VEHICLE)
					{
						SendOnFootFullSyncData(); 
					}
				}

				pGame->DisablePassengerEngineAudio();
			}			
		}
	}
	else if ((m_bIsWasted == TRUE) && 
			(m_pPlayerPed->GetAction() != ACTION_WASTED))
	{
		m_bIsWasted = FALSE;
		//pNetGame->GetGameLogic()->HandleClassSelection(this);
		this->SpawnPlayer();
		return TRUE;
	}

	pGameLogic = pNetGame->GetGameLogic();
	if(pGameLogic) pGameLogic->ProcessLocalPlayer(this);

	return TRUE;
}

//----------------------------------------------------------

void CLocalPlayer::SendOnFootFullSyncData()
{
	if(m_pPlayerPed)
	{
		RakNet::BitStream bsPlayerSync;
		PLAYER_SYNC_DATA playerSyncData;

		// write packet id
		bsPlayerSync.Write((MessageID)ID_PLAYER_SYNC);

		// get the player keys
		playerSyncData.wKeys = m_pPlayerPed->GetKeys();

		// get the player position
		m_pPlayerPed->GetPosition(&playerSyncData.vecPos);

		// get the player rotation
		playerSyncData.fRotation = m_pPlayerPed->GetRotation();
		
		// get player current weapon (casted to a byte to save space)
		playerSyncData.byteCurrentWeapon = m_pPlayerPed->GetCurrentWeapon();
		
		// get player shooting flags
		playerSyncData.byteShootingFlags = m_pPlayerPed->GetShootingFlags();
		
		// get player health (casted to a byte to save space)
		playerSyncData.byteHealth = (BYTE)m_pPlayerPed->GetHealth();

		// get player armour (casted to a byte to save space)
		playerSyncData.byteArmour = (BYTE)m_pPlayerPed->GetArmour();

		// write player sync data struct
		bsPlayerSync.Write((char *)&playerSyncData, sizeof(PLAYER_SYNC_DATA));

		// send aiming data if player has fire key held down and we have ammo
		if(m_pPlayerPed->IsFiring() && m_pPlayerPed->HasAmmoForCurrentWeapon())
		{
			// write a 1 bit to say the bit stream has aim sync data
			bsPlayerSync.Write1();

			// get the camera aim
			CAMERA_AIM * pCameraAim = m_pPlayerPed->GetCurrentAim();

			// write the aim sync data
			bsPlayerSync.Write((char *)&pCameraAim->vecA1, sizeof(Vector3));
			bsPlayerSync.Write((char *)&pCameraAim->vecA2, sizeof(Vector3));
			bsPlayerSync.Write((char *)&pCameraAim->vecAPos1, sizeof(Vector3));
		}
		else
		{
			// write a 0 bit to say the bit stream has no aim sync data
			bsPlayerSync.Write0();
		}

		// send sync data
		pNetGame->GetRakPeer()->Send(&bsPlayerSync,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------------

void CLocalPlayer::SendInCarFullSyncData()
{
	if(m_pPlayerPed)
	{
		RakNet::BitStream bsVehicleSync;
		VEHICLE_SYNC_DATA vehicleSyncData;
		MATRIX4X4 matVehicle;
		CVehiclePool * pVehiclePool = pNetGame->GetVehiclePool();
		CVehicle * pGameVehicle = NULL;

		// write packet id
		bsVehicleSync.Write((MessageID)ID_VEHICLE_SYNC);

		vehicleSyncData.byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());

		// make sure we have a valid vehicle
		if(vehicleSyncData.byteVehicleID == 255)
		{
			return;
		}

		// get the player keys
		vehicleSyncData.wKeys = m_pPlayerPed->GetKeys();

		// get the vehicle pointer
		pGameVehicle = pVehiclePool->GetAt(vehicleSyncData.byteVehicleID);

		// make sure the vehicle pointer is valid
		if(!pGameVehicle)
		{
			return;
		}

		// get the vehicle matrix
		pGameVehicle->GetMatrix(&matVehicle);

		// copy the roll and direction vectors
		memcpy(&vehicleSyncData.vecRoll, &matVehicle.vLookRight, sizeof(Vector3));
		memcpy(&vehicleSyncData.vecDirection, &matVehicle.vLookUp, sizeof(Vector3));
		// pack the roll and direction vectors
		//CompressVector1(&matVehicle.vLookRight, &vehicleSyncData.cvecRoll);
		//CompressVector1(&matVehicle.vLookUp, &vehicleSyncData.cvecDirection);

		// copy the vehicle position
		memcpy(&vehicleSyncData.vecPos, &matVehicle.vPos, sizeof(Vector3));

		// get the vehicle move speed
		pGameVehicle->GetMoveSpeed(&vehicleSyncData.vecMoveSpeed);

		// get the vehicle turn speed
		pGameVehicle->GetTurnSpeed(&vehicleSyncData.vecTurnSpeed);

		// pack the vehicle health
		vehicleSyncData.byteVehicleHealth = PACK_VEHICLE_HEALTH(pGameVehicle->GetHealth());

		// get the player health (casted to a byte to save space)
		vehicleSyncData.bytePlayerHealth = (BYTE)m_pPlayerPed->GetHealth();

		// get the player armour (casted to a byte to save space)
		vehicleSyncData.bytePlayerArmour = (BYTE)m_pPlayerPed->GetArmour();

		// write vehicle sync struct to bit stream
		bsVehicleSync.Write((char *)&vehicleSyncData, sizeof(VEHICLE_SYNC_DATA));

		// send sync data
		pNetGame->GetRakPeer()->Send(&bsVehicleSync,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------------

void CLocalPlayer::SendInCarPassengerData()
{
	RakNet::BitStream bsPassengerSync;
	Vector3 vPos;
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	BYTE byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
	if(byteVehicleID == 255) return;

	UINT uiPassengerSeat = m_pPlayerPed->GetPassengerSeat();
	
	m_pPlayerPed->GetPosition(&vPos);
	
	bsPassengerSync.Write((BYTE)ID_PASSENGER_SYNC);
	bsPassengerSync.Write((BYTE)byteVehicleID);
	bsPassengerSync.Write(uiPassengerSeat);
	bsPassengerSync.Write(vPos.X);
	bsPassengerSync.Write(vPos.Y);
	bsPassengerSync.Write(vPos.Z);
	pNetGame->GetRakPeer()->Send(&bsPassengerSync,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------------

int CLocalPlayer::GetOptimumInCarSendRate()
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CVehicle	 *pGameVehicle=NULL;
	Vector3	 vecMoveSpeed;
	BYTE		 byteVehicleID=0;

	if(m_pPlayerPed) {

		byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
		pGameVehicle = pVehiclePool->GetAt(byteVehicleID);

		if(pGameVehicle) {

			pGameVehicle->GetMoveSpeed(&vecMoveSpeed);

			if( (vecMoveSpeed.X == 0.0f) &&
				(vecMoveSpeed.Y == 0.0f) ) {

				return 200;
			}
			else {
				return 66;
			}
		}
	}
	return 200;
}

//----------------------------------------------------------

int CLocalPlayer::GetOptimumOnFootSendRate()
{	
	Vector3	 vecMoveSpeed;

	if(m_pPlayerPed) {

		m_pPlayerPed->GetMoveSpeed(&vecMoveSpeed);

		if( (vecMoveSpeed.X == 0.0f) &&
			(vecMoveSpeed.Y == 0.0f) ) {

			return 60;
		}
		else {
			return 30;
		}
	}

	return 60;
}

//----------------------------------------------------------

void CLocalPlayer::SendWastedNotification()
{
	CGameModeGeneric *pGameLogic;
	RakNet::BitStream bsPlayerDeath;
	BYTE byteDeathReason;
	BYTE byteWhoWasResponsible;

	byteDeathReason = m_pPlayerPed->FindDeathReasonAndResponsiblePlayer(&byteWhoWasResponsible);
	
	bsPlayerDeath.Write(byteDeathReason);
	bsPlayerDeath.Write(byteWhoWasResponsible);
	pNetGame->GetRPC4()->Call("Death",&bsPlayerDeath,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	
	pGameLogic = pNetGame->GetGameLogic();
}

//----------------------------------------------------------

void CLocalPlayer::RequestClass(int iClass)
{
	RakNet::BitStream bsSpawnRequest;
	bsSpawnRequest.Write(iClass);
	pNetGame->GetRPC4()->Call("RequestClass",&bsSpawnRequest,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------------

void CLocalPlayer::SetSpawnInfo(BYTE byteTeam, BYTE byteSkin, Vector3 * vecPos, float fRotation,
		int iSpawnWeapon1, int iSpawnWeapon1Ammo, int iSpawnWeapon2, int iSpawnWeapon2Ammo,
		int iSpawnWeapon3, int iSpawnWeapon3Ammo)
{
	m_SpawnInfo.byteTeam = byteTeam;
	m_SpawnInfo.byteSkin = byteSkin;
	m_SpawnInfo.vecPos.X = vecPos->X;
	m_SpawnInfo.vecPos.Y = vecPos->Y;
	m_SpawnInfo.vecPos.Z = vecPos->Z;
	m_SpawnInfo.fRotation = fRotation;
	m_SpawnInfo.iSpawnWeapons[0] = iSpawnWeapon1;
	m_SpawnInfo.iSpawnWeapons[1] = iSpawnWeapon2;
	m_SpawnInfo.iSpawnWeapons[2] = iSpawnWeapon3;
	m_SpawnInfo.iSpawnWeaponsAmmo[0] = iSpawnWeapon1Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[1] = iSpawnWeapon2Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[2] = iSpawnWeapon3Ammo;
	m_bHasSpawnInfo = TRUE;
}

//----------------------------------------------------------

BOOL CLocalPlayer::SpawnPlayer()
{
	return SpawnPlayer(m_SpawnInfo.byteTeam,m_SpawnInfo.byteSkin,
		&m_SpawnInfo.vecPos,m_SpawnInfo.fRotation,
		m_SpawnInfo.iSpawnWeapons[0],
		m_SpawnInfo.iSpawnWeaponsAmmo[0],
		m_SpawnInfo.iSpawnWeapons[1],
		m_SpawnInfo.iSpawnWeaponsAmmo[1],
		m_SpawnInfo.iSpawnWeapons[2],
		m_SpawnInfo.iSpawnWeaponsAmmo[2]);
}

//----------------------------------------------------------

BOOL CLocalPlayer::SpawnPlayer( BYTE byteTeam,
							    BYTE byteSkin, 
							    Vector3 * vecPos,
							    float fRotation,int iSpawnWeapon1,
								int iSpawnWeapon1Ammo, int iSpawnWeapon2, 
								int iSpawnWeapon2Ammo, int iSpawnWeapon3,
								int iSpawnWeapon3Ammo )
{
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();

	if(!pGamePlayer) return FALSE;	

	if(pGamePlayer)
	{
		pGamePlayer->RestartIfWastedAt(vecPos, fRotation);

		m_pPlayerPed = pGamePlayer;

		// Set skin stuff.. logic is because of temperament
		if(pGamePlayer->GetModelIndex() != byteSkin && (byteSkin < 107)) {
			if(!pGame->IsModelLoaded(byteSkin)) {
				pGame->RequestModel(byteSkin);
				pGame->LoadRequestedModels();
				while(!pGame->IsModelLoaded(byteSkin)) { Sleep(1); }
			}
			pGamePlayer->SetModel(byteSkin);
		}

		pGamePlayer->ClearAllWeapons();

		if(iSpawnWeapon3 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon3,iSpawnWeapon3Ammo);
		}
		if(iSpawnWeapon2 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon2,iSpawnWeapon2Ammo);
		}		
		if(iSpawnWeapon1 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon1,iSpawnWeapon1Ammo);
		}

		pGamePlayer->Teleport(vecPos->X,vecPos->Y,vecPos->Z);
		pGamePlayer->SetRotation(fRotation);
		pGamePlayer->SetInitialState();

		m_bIsWasted = FALSE;
		m_bIsActive = TRUE;
	}	

	// Let the rest of the network know we're spawning.
	RakNet::BitStream bsSendSpawn;
	pNetGame->GetRPC4()->Call("Spawn",&bsSendSpawn,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	
	return TRUE;
}

//----------------------------------------------------------

void CLocalPlayer::Say(PCHAR szText)
{
	if(!pNetGame->IsConnected()) {
		pChatWindow->AddInfoMessage("Not connected");
		return;
	}

	BYTE byteTextLen = strlen(szText);

	RakNet::BitStream bsSend;
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);

	pNetGame->GetRPC4()->Call("Chat",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------------

void CLocalPlayer::SendEnterVehicleNotification(BYTE byteVehicleID, BOOL bPassenger)
{
	RakNet::BitStream bsSend;
	BYTE bytePassenger=0;

	if(bPassenger) bytePassenger=1;

	bsSend.Write(byteVehicleID);
	bsSend.Write(bytePassenger);
	pNetGame->GetRPC4()->Call("EnterVehicle",&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------------

void CLocalPlayer::SendExitVehicleNotification(BYTE byteVehicleID)
{
	RakNet::BitStream bsSend;
	bsSend.Write(byteVehicleID);
	pNetGame->GetRPC4()->Call("ExitVehicle",&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------

DWORD CLocalPlayer::GetTeamColorAsRGBA()
{
	return TranslateColorCodeToRGBA(m_SpawnInfo.byteTeam);
}

//----------------------------------------------------

DWORD CLocalPlayer::GetTeamColorAsARGB()
{
	return (TranslateColorCodeToRGBA(m_SpawnInfo.byteTeam) >> 8) | 0xFF000000;	
}

//----------------------------------------------------

