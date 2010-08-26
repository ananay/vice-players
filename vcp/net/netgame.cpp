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
//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
//----------------------------------------------------

#include "../main.h"
#include <stdlib.h>

#define NETGAME_VERSION 6

using namespace RakNet;

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;
RPC4				 *CNetGame::m_pRPC4;
extern bool D3DInited;

//----------------------------------------------------

void CompressVector1(VECTOR * vec, C_VECTOR1 * c1)
{
	c1->X = (short)(vec->X * 10000);
	c1->Y = (short)(vec->Y * 10000);
	c1->Z = (short)(vec->Z * 10000);	
}

//----------------------------------------------------

void DecompressVector1(VECTOR * vec, C_VECTOR1 * c1)
{
	vec->X = (float)c1->X;
	vec->X = (float)((double)vec->X / 10000.0);
	vec->Y = (float)c1->Y;
	vec->Y = (float)((double)vec->Y / 10000.0);
	vec->Z = (float)c1->Z;
	vec->Z = (float)((double)vec->Z / 10000.0);	
}

//----------------------------------------------------

CNetGame::CNetGame(PCHAR szHostOrIp, int iPort, 
				   PCHAR szPlayerName, PCHAR szPass)
{
	// Setup player pool
	m_pPlayerPool = new CPlayerPool();
	m_pPlayerPool->SetLocalPlayerName(szPlayerName);

	m_pVehiclePool = new CVehiclePool();

	m_pRakPeer = RakPeerInterface::GetInstance();
	m_pRPC4 = RPC4::GetInstance();

	RegisterRPCs();
	
	m_pRakPeer->Startup(1,&SocketDescriptor(),1);
	strcpy(m_szHostOrIp, szHostOrIp);
	m_iPort = iPort;
	strcpy(m_szPass, szPass);
	m_pRakPeer->AttachPlugin(m_pRPC4);
	
	pChatWindow->AddDebugMessage("Vice City: Players started " CLIENT_VERSION ".");

	Connect();
	if(pChatWindow) pChatWindow->AddDebugMessage("Connecting to %s:%d..",szHostOrIp,iPort);

	m_pGameLogic = NULL;
	m_dwLastScoreUpdateTick = GetTickCount();
	m_uiLastRandSeed = 0;
}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	Shutdown();
	m_pRakPeer->DetachPlugin(m_pRPC4);
	UnRegisterRPCs();
	RPC4::DestroyInstance(m_pRPC4);
	RakPeerInterface::DestroyInstance(m_pRakPeer);
	delete m_pPlayerPool;
	delete m_pGameLogic;
}

//----------------------------------------------------

void CNetGame::InitGameLogic()
{
	m_pGameLogic = new CGameModeGeneric();
}

//----------------------------------------------------

void CNetGame::Process()
{
	UpdateNetwork();

	if(GetGameState() == GAMESTATE_CONNECTED) {
		if(m_pPlayerPool) m_pPlayerPool->Process();
		if(m_pVehiclePool) m_pVehiclePool->Process();
	}

	// For syncing rand()
	/*if(m_pRakPeer->GetSynchronizedRandomInteger() != m_uiLastRandSeed) {
		m_uiLastRandSeed = m_pRakPeer->GetSynchronizedRandomInteger();
		srand(m_uiLastRandSeed);
	}*/
}

//----------------------------------------------------

void CNetGame::UpdateNetwork()
{
	Packet* pkt=NULL;

	while((pkt = m_pRakPeer->Receive()))
	{
		switch(pkt->data[0])
		{
			/*
		case ID_RSA_PUBLIC_KEY_MISMATCH:
			pChatWindow->AddDebugMessage("Failed to init encryption.");
			break;
			*/
		case ID_CONNECTION_BANNED:
			pChatWindow->AddDebugMessage("You're banned from the server.");
			Shutdown();
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			pChatWindow->AddDebugMessage("The server is full, Retrying...");
			Connect();
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			pChatWindow->AddDebugMessage("Disconnected.");
			Shutdown();
			pGame->FadeScreen(0, 0);
			break;
		case ID_CONNECTION_LOST:
			pChatWindow->AddDebugMessage("Lost connection to the server, Reconnecting...");
			Connect();
			pGame->FadeScreen(0, 0);
			break;
		case ID_INVALID_PASSWORD:
			pChatWindow->AddDebugMessage("Wrong server password.");
			break;
			/*
		case ID_MODIFIED_PACKET:
			m_pRakPeer->Shutdown(100);
			break;
			*/
		case ID_CONNECTION_ATTEMPT_FAILED:
			pChatWindow->AddDebugMessage("Failed to connect, Retrying...");
			Connect();
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			ConnectionSucceeded(pkt);
			break;
		case ID_PLAYER_SYNC:
			PlayerSync(pkt);
			break;
		case ID_VEHICLE_SYNC:
			VehicleSync(pkt);
			break;

		}

		m_pRakPeer->DeallocatePacket(pkt);		
	}

}

//----------------------------------------------------

void CNetGame::PlayerSync(Packet *p)
{
	CRemotePlayer * pPlayer;
	BitStream bsPlayerSync(p->data, p->length, FALSE);
	BYTE byteSystemAddress=0;

	WORD wKeys=0;
	float fRotation;
	BYTE  bytePlayerHealth;
	BYTE  bytePlayerArmour;
	BYTE  byteCurrentWeapon;
	BYTE  byteAction;

	MATRIX4X4 matWorld;

	CAMERA_AIM caAiming;

	bsPlayerSync.IgnoreBytes(sizeof(MessageID));
	bsPlayerSync.Read(byteSystemAddress);
	bsPlayerSync.Read(wKeys);

	pPlayer = GetPlayerPool()->GetAt(byteSystemAddress);
	bsPlayerSync.Read(matWorld.vPos.X);
	bsPlayerSync.Read(matWorld.vPos.Y);
	bsPlayerSync.Read(matWorld.vPos.Z);
	bsPlayerSync.Read(fRotation);
	bsPlayerSync.Read(byteAction);
	bsPlayerSync.Read(bytePlayerHealth);
	bsPlayerSync.Read(bytePlayerArmour);
	bsPlayerSync.Read(byteCurrentWeapon);

	//if(IS_FIRING(wKeys)) {
		// aiming
		bsPlayerSync.Read((char *)&caAiming.vecA1, sizeof(VECTOR));
		bsPlayerSync.Read((char *)&caAiming.vecA2, sizeof(VECTOR));
		bsPlayerSync.Read((char *)&caAiming.vecAPos1, sizeof(VECTOR));
		memcpy(&caAiming.vecA2, &caAiming.vecA1, sizeof(VECTOR));
	//}
	
	if(pPlayer) {
		pPlayer->StoreOnFootFullSyncData(wKeys,&matWorld,fRotation,byteCurrentWeapon,byteAction);
		/*if(IS_FIRING(wKeys)) */pPlayer->StoreAimSyncData(&caAiming);
		pPlayer->SetReportedHealth(bytePlayerHealth);
		pPlayer->SetReportedArmour(bytePlayerArmour);
	}
}

void CNetGame::AimSync(Packet *p)
{
	CRemotePlayer * pPlayer;
	BitStream bsPlayerAimSync(p->data, p->length, FALSE);

	AIM_SYNC_DATA aimSync;
	BYTE byteSystemAddress=0;
	WORD		wKeys;

	if(GetGameState() != GAMESTATE_CONNECTED) return;

	bsPlayerAimSync.Read(byteSystemAddress);
	bsPlayerAimSync.Read(wKeys);
	bsPlayerAimSync.Read((PCHAR)&aimSync,sizeof(AIM_SYNC_DATA));

	if (m_pPlayerPool) {
		pPlayer = m_pPlayerPool->GetAt(playerId);
		if(pPlayer) {
			pPlayer->UpdateAimFromSyncData(&aimSync);
		}
	}
}

//----------------------------------------------------

void CNetGame::VehicleSync(Packet *p)
{
	CRemotePlayer * pPlayer;
	BitStream bsVehicleSync(p->data, p->length, FALSE);
	BYTE		byteSystemAddress=0;
	BYTE		byteVehicleID=0;

	WORD		wKeys;
	C_VECTOR1 cvecRoll;
	C_VECTOR1 cvecDirection;
	MATRIX4X4	matWorld;
	VECTOR	vecMoveSpeed;

	float		fHealth;

	BYTE		byteReadVehicleHealth;
	BYTE		bytePlayerHealth;
	BYTE		bytePlayerArmour;

	bsVehicleSync.IgnoreBytes(sizeof(MessageID));
	bsVehicleSync.Read(byteSystemAddress);
	bsVehicleSync.Read(byteVehicleID);
	bsVehicleSync.Read(wKeys);
	bsVehicleSync.Read(cvecRoll.X);
	bsVehicleSync.Read(cvecRoll.Y);
	bsVehicleSync.Read(cvecRoll.Z);
	bsVehicleSync.Read(cvecDirection.X);
	bsVehicleSync.Read(cvecDirection.Y);
	bsVehicleSync.Read(cvecDirection.Z);
	bsVehicleSync.Read(matWorld.vPos.X);
	bsVehicleSync.Read(matWorld.vPos.Y);
	bsVehicleSync.Read(matWorld.vPos.Z);

	// move + turn speed.
	bsVehicleSync.Read(vecMoveSpeed.X);
	bsVehicleSync.Read(vecMoveSpeed.Y);

	bsVehicleSync.Read(byteReadVehicleHealth);
	bsVehicleSync.Read(bytePlayerHealth);
	bsVehicleSync.Read(bytePlayerArmour);

	// now unpack the roll, direction vectors from the shorts.
	DecompressVector1(&matWorld.vLookRight,&cvecRoll);
	DecompressVector1(&matWorld.vLookUp,&cvecDirection);

	// unpack vehicle health
	fHealth = UNPACK_VEHICLE_HEALTH(byteReadVehicleHealth);

	pPlayer = GetPlayerPool()->GetAt(byteSystemAddress);

	if(pPlayer)	{
		pPlayer->StoreInCarFullSyncData(byteVehicleID,wKeys,&matWorld,
			&vecMoveSpeed,fHealth);
		pPlayer->SetReportedHealth(bytePlayerHealth);
		pPlayer->SetReportedArmour(bytePlayerArmour);
	}
}

//----------------------------------------------------

void CNetGame::ConnectionSucceeded(Packet *p)
{
	if(pChatWindow) {
		pChatWindow->AddDebugMessage("Connection success. Loading network game...");
	}

	m_iGameState = GAMESTATE_AWAIT_JOIN;

	BYTE byteVersion = NETGAME_VERSION;
	BYTE byteNameLen = (BYTE)strlen(m_pPlayerPool->GetLocalPlayerName());
	
	BitStream bsSend;
	bsSend.Write(byteVersion);
	bsSend.Write(byteNameLen);
	bsSend.Write(m_pPlayerPool->GetLocalPlayerName(),byteNameLen);
	m_pRPC4->Call("ClientJoin",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------

void CNetGame::UpdatePlayerScoresAndPings()
{
	if( (GetTickCount() - m_dwLastScoreUpdateTick) > 1000 ) {
		m_dwLastScoreUpdateTick = GetTickCount();

		m_pRPC4->Call("UpdateScoreAndPing",NULL,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------

void CNetGame::Connect()
{
	if(m_pRakPeer) {
		m_pRakPeer->Connect(m_szHostOrIp, m_iPort, m_szPass, strlen(m_szPass));
		m_iGameState = GAMESTATE_CONNECTING;
	}
}

//----------------------------------------------------

bool CNetGame::IsConnected()
{
	if(GetGameState() == GAMESTATE_CONNECTED) {
		return true;
	}
	return false;
}

//----------------------------------------------------

void CNetGame::Shutdown()
{
	if(m_pRakPeer && m_iGameState == GAMESTATE_CONNECTED) {
		m_pRakPeer->Shutdown(500);
	}
	m_iGameState = GAMESTATE_DISCONNECTED;
}

//----------------------------------------------------