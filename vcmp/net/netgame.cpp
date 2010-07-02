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

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

#define NETGAME_VERSION 6

using namespace RakNet;

RPC4 rpc1;

//----------------------------------------------------

BYTE GetPacketID(RakNet::Packet *p)
{
	if (p==0) return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else {
		return (unsigned char) p->data[0];
	}
}

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

RakNet::RPC4 * CNetGame::GetRPC4()
{
	return &rpc1;
}

//----------------------------------------------------

CNetGame::CNetGame(PCHAR szHostOrIp, int iPort, 
				   PCHAR szPlayerName, PCHAR szPass)
{
	// Setup player pool
	m_pPlayerPool = new CPlayerPool();
	m_pPlayerPool->SetLocalPlayerName(szPlayerName);

	m_pVehiclePool = new CVehiclePool();

	m_pRakClient = RakNet::RakPeerInterface::GetInstance();

	//m_pRakClient->InitializeSecurity(0,0,0,0); // Removed in 0.4 RakNet

	RegisterRPCs();
	
	m_pRakClient->Startup(1,&SocketDescriptor(),1);

	m_pRakClient->Connect(szHostOrIp,iPort,szPass,strlen(szPass));

	m_pRakClient->AttachPlugin(GetRPC4());
	
	m_iGameState = GAMESTATE_CONNECTING;
	pChatWindow->AddDebugMessage("Vice City: Players started.");
	if(pChatWindow) pChatWindow->AddDebugMessage("Connecting to %s:%d..",szHostOrIp,iPort);

	m_pGameLogic = NULL;
	m_iSpawnsAvailable = 0;
	
	m_dwLastScoreUpdateTick = GetTickCount();

	m_uiLastRandSeed = 0;
}

//----------------------------------------------------

CNetGame::~CNetGame()
{
	m_pRakClient->Shutdown(0);
	UnRegisterRPCs();
	RakNet::RakPeerInterface::DestroyInstance(m_pRakClient);
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
	/*if(m_pRakClient->GetSynchronizedRandomInteger() != m_uiLastRandSeed) {
		m_uiLastRandSeed = m_pRakClient->GetSynchronizedRandomInteger();
		srand(m_uiLastRandSeed);
	}*/
}

//----------------------------------------------------

void CNetGame::UpdateNetwork()
{
	RakNet::Packet* pkt=NULL;

	while((pkt = m_pRakClient->Receive()))
	{
		switch(GetPacketID(pkt))
		{
			/*
		case ID_RSA_PUBLIC_KEY_MISMATCH:
			pChatWindow->AddDebugMessage("Failed to init encryption.");
			break;
			*/
		case ID_CONNECTION_BANNED:
			pChatWindow->AddDebugMessage("You're banned from the server.");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			pChatWindow->AddDebugMessage("The server is full.");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			pChatWindow->AddDebugMessage("Disconnected.");
			break;
		case ID_CONNECTION_LOST:
			pChatWindow->AddDebugMessage("Lost connection to the server.");
			break;
		case ID_INVALID_PASSWORD:
			pChatWindow->AddDebugMessage("Wrong server password.");
			break;
			/*
		case ID_MODIFIED_PACKET:
			m_pRakClient->Shutdown(100);
			break;
			*/
		case ID_CONNECTION_ATTEMPT_FAILED:
			pChatWindow->AddDebugMessage("Failed to connect.");
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

		m_pRakClient->DeallocatePacket(pkt);		
	}

}

//----------------------------------------------------

void CNetGame::PlayerSync(RakNet::Packet *p)
{
	CRemotePlayer * pPlayer;
	RakNet::BitStream bsPlayerSync(p->data, p->length, FALSE);
	BYTE bytePacketID=0;
	BYTE byteSystemAddress=0;

	WORD wKeys=0;
	float fRotation;
	BYTE  bytePlayerHealth;
	BYTE  byteCurrentWeapon;
	BYTE  byteAction;

	MATRIX4X4 matWorld;

	CAMERA_AIM caAiming;

	bsPlayerSync.Read(bytePacketID);
	bsPlayerSync.Read(byteSystemAddress);
	bsPlayerSync.Read(wKeys);

	pPlayer = GetPlayerPool()->GetAt(byteSystemAddress);
	bsPlayerSync.Read(matWorld.vPos.X);
	bsPlayerSync.Read(matWorld.vPos.Y);
	bsPlayerSync.Read(matWorld.vPos.Z);
	bsPlayerSync.Read(fRotation);
	bsPlayerSync.Read(byteAction);
	bsPlayerSync.Read(bytePlayerHealth);
	bsPlayerSync.Read(byteCurrentWeapon);

	if(IS_FIRING(wKeys)) {
		C_VECTOR1		cvecAimRoll;
		C_VECTOR1		cvecAimDir;
		VECTOR	vecAimRoll;
		VECTOR	vecAimDir;

		// aiming
		bsPlayerSync.Read(cvecAimRoll.X);
		bsPlayerSync.Read(cvecAimRoll.Y);
		bsPlayerSync.Read(cvecAimRoll.Z);
		bsPlayerSync.Read(cvecAimDir.X);
		bsPlayerSync.Read(cvecAimDir.Y);
		bsPlayerSync.Read(cvecAimDir.Z);
		bsPlayerSync.Read(caAiming.pos1x);
		bsPlayerSync.Read(caAiming.pos1y);
		bsPlayerSync.Read(caAiming.pos1z);

		DecompressVector1(&vecAimRoll,&cvecAimRoll);
		DecompressVector1(&vecAimDir,&cvecAimDir);

		caAiming.f1x = vecAimRoll.X;
		caAiming.f1y = vecAimRoll.Y;
		caAiming.f1z = vecAimRoll.Z;
		caAiming.f2x = vecAimDir.X;
		caAiming.f2y = vecAimDir.Y;
		caAiming.f2z = vecAimDir.Z;
	}
	
	if(pPlayer) {
		pPlayer->StoreOnFootFullSyncData(wKeys,&matWorld,fRotation,byteCurrentWeapon,byteAction);
		if(IS_FIRING(wKeys)) pPlayer->StoreAimSyncData(&caAiming);
		pPlayer->SetReportedHealth(bytePlayerHealth);
	}
}

//----------------------------------------------------

void CNetGame::VehicleSync(RakNet::Packet *p)
{
	CRemotePlayer * pPlayer;
	RakNet::BitStream bsVehicleSync(p->data, p->length, FALSE);
	BYTE		bytePacketID=0;
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

	bsVehicleSync.Read(bytePacketID);
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
	}
}

//----------------------------------------------------

void CNetGame::ConnectionSucceeded(RakNet::Packet *p)
{
	if(pChatWindow) {
		pChatWindow->AddDebugMessage("Connection success. Loading network game...");
	}

	m_iGameState = GAMESTATE_AWAIT_JOIN;

	BYTE byteVersion = NETGAME_VERSION;
	BYTE byteNameLen = (BYTE)strlen(m_pPlayerPool->GetLocalPlayerName());
	
	RakNet::BitStream bsSend;
	bsSend.Write(byteVersion);
	bsSend.Write(byteNameLen);
	bsSend.Write(m_pPlayerPool->GetLocalPlayerName(),byteNameLen);
	//m_pRakClient->RPC("ClientJoin",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE,FALSE,UNASSIGNED_NETWORK_ID,0);
	GetRPC4()->Call("ClientJoin",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
}

//----------------------------------------------------

void CNetGame::UpdatePlayerScoresAndPings()
{
	if( (GetTickCount() - m_dwLastScoreUpdateTick) > 1000 ) {
		m_dwLastScoreUpdateTick = GetTickCount();

		RakNet::BitStream bsSend;
		//m_pRakClient->RPC("UpdateScoreAndPing",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE,FALSE,UNASSIGNED_NETWORK_ID,0);
		GetRPC4()->Call("UpdateScoreAndPing",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------