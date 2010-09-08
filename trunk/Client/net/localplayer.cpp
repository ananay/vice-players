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
#include "../game/util.h"

// Sync send rates

#define INCAR_SENDRATE_IDLE 200
#define INCAR_SENDRATE_ACTIVE 66
#define ONFOOT_SENDRATE_IDLE 60
#define ONFOOT_SENDRATE_ACTIVE 30

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
	CSpawnSelection *pGameLogic;
	CVehicle *pGameVehicle;
	CVehiclePool *pVehiclePool;

	EntityId vehicleID;

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

					// VEHICLE WORLD BOUNDS STUFF
					pVehiclePool = pNetGame->GetVehiclePool();
					vehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
					if(vehicleID != 255) {
						pGameVehicle = pVehiclePool->GetAt(vehicleID);
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
		pNetGame->GetGameLogic()->HandleClassSelection(this);
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

			// get the camera pointer
			CAMERA_TYPE * pCamera = pGame->GetCamera()->GetCamera();

			// write the aim sync data
			bsPlayerSync.Write((char *)&pCamera->aim, sizeof(CAMERA_AIM));
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

		vehicleSyncData.vehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());

		// make sure we have a valid vehicle
		if(vehicleSyncData.vehicleID == 255)
		{
			return;
		}

		// get the player keys
		vehicleSyncData.wKeys = m_pPlayerPed->GetKeys();

		// get the vehicle pointer
		pGameVehicle = pVehiclePool->GetAt(vehicleSyncData.vehicleID);

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

	EntityId vehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
	if(vehicleID == 255) return;

	BYTE bytePassengerSeat = m_pPlayerPed->GetPassengerSeat();
	
	m_pPlayerPed->GetPosition(&vPos);
	
	bsPassengerSync.Write((MessageID)ID_PASSENGER_SYNC);
	bsPassengerSync.Write(vehicleID);
	bsPassengerSync.Write(bytePassengerSeat);
	bsPassengerSync.Write((char *)&vPos, sizeof(Vector3));
	pNetGame->GetRakPeer()->Send(&bsPassengerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

//----------------------------------------------------------

int CLocalPlayer::GetOptimumInCarSendRate()
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CVehicle	 *pGameVehicle=NULL;
	Vector3		 vecMoveSpeed;
	BYTE		 vehicleID=0;

	if(m_pPlayerPed)
	{
		vehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
		pGameVehicle = pVehiclePool->GetAt(vehicleID);

		if(pGameVehicle)
		{
			pGameVehicle->GetMoveSpeed(&vecMoveSpeed);

			// is the vehicle moving?
			if(!((vecMoveSpeed.X == 0.0f) && (vecMoveSpeed.Y == 0.0f)))
			{
				return INCAR_SENDRATE_ACTIVE;
			}
		}
	}

	return INCAR_SENDRATE_IDLE;
}

//----------------------------------------------------------

int CLocalPlayer::GetOptimumOnFootSendRate()
{	
	Vector3	 vecMoveSpeed;

	if(m_pPlayerPed)
	{
		m_pPlayerPed->GetMoveSpeed(&vecMoveSpeed);

		// is the player moving?
		if(!((vecMoveSpeed.X == 0.0f) && (vecMoveSpeed.Y == 0.0f)))
		{
			return ONFOOT_SENDRATE_ACTIVE;
		}
	}

	return ONFOOT_SENDRATE_IDLE;
}

//----------------------------------------------------------

void CLocalPlayer::SendWastedNotification()
{
	CSpawnSelection *pGameLogic;
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

void CLocalPlayer::SetSpawnInfo(PLAYER_SPAWN_INFO * pSpawnInfo)
{
	memcpy(&m_SpawnInfo, pSpawnInfo, sizeof(PLAYER_SPAWN_INFO));
	m_bHasSpawnInfo = TRUE;
}

//----------------------------------------------------------

BOOL CLocalPlayer::SpawnPlayer()
{
	return SpawnPlayer(&m_SpawnInfo);
}

//----------------------------------------------------------

BOOL CLocalPlayer::SpawnPlayer(PLAYER_SPAWN_INFO * pSpawnInfo)
{
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();

	if(!pGamePlayer) return FALSE;	

	if(pGamePlayer)
	{
		pGamePlayer->RestartIfWastedAt(&pSpawnInfo->vecPos, pSpawnInfo->fRotation);

		m_pPlayerPed = pGamePlayer;

		// Set skin stuff.. logic is because of temperament
		if(pGamePlayer->GetModelIndex() != pSpawnInfo->byteSkin && (pSpawnInfo->byteSkin < 107)) {
			if(!pGame->IsModelLoaded(pSpawnInfo->byteSkin)) {
				pGame->RequestModel(pSpawnInfo->byteSkin);
				pGame->LoadRequestedModels();
				while(!pGame->IsModelLoaded(pSpawnInfo->byteSkin)) { Sleep(1); }
			}
			pGamePlayer->SetModel(pSpawnInfo->byteSkin);
		}

		pGamePlayer->ClearAllWeapons();

		if(pSpawnInfo->iSpawnWeapons[3] != -1) {
			pGamePlayer->GiveWeapon(pSpawnInfo->iSpawnWeapons[3], pSpawnInfo->iSpawnWeaponsAmmo[3]);
		}
		if(pSpawnInfo->iSpawnWeapons[2] != -1) {
			pGamePlayer->GiveWeapon(pSpawnInfo->iSpawnWeapons[2], pSpawnInfo->iSpawnWeaponsAmmo[2]);
		}		
		if(pSpawnInfo->iSpawnWeapons[1] != -1) {
			pGamePlayer->GiveWeapon(pSpawnInfo->iSpawnWeapons[1], pSpawnInfo->iSpawnWeaponsAmmo[1]);
		}

		pGamePlayer->Teleport(pSpawnInfo->vecPos.X, pSpawnInfo->vecPos.Y, pSpawnInfo->vecPos.Z);
		pGamePlayer->SetRotation(pSpawnInfo->fRotation);
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

void CLocalPlayer::SendEnterVehicleNotification(EntityId vehicleID, BOOL bPassenger)
{
	RakNet::BitStream bsSend;
	BYTE bytePassenger=0;

	if(bPassenger) bytePassenger=1;

	bsSend.Write(vehicleID);
	bsSend.Write(bytePassenger);
	pNetGame->GetRPC4()->Call("EnterVehicle",&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------------

void CLocalPlayer::SendExitVehicleNotification(EntityId vehicleID)
{
	RakNet::BitStream bsSend;
	bsSend.Write(vehicleID);
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
