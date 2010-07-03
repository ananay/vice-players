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

#include "main.h"
#include "game/util.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;
extern CNetGame		 *pNetGame;

DWORD dwPlayerIncrementer=2;
extern GAME_SETTINGS tSettings;
using namespace RakNet;

//----------------------------------------------------

void WriteCompression(ULONG *ulWrite)
{
	FILE *fileComp=fopen("ClientLayer.txt","w");

	int x=0;
	
	while(x!=256) {
		fprintf(fileComp,"0x%X,",ulWrite[x]);
		x++;
	}

	fclose(fileComp);
}

//----------------------------------------------------

void cmdDefaultCmdProc(PCHAR szCmd)
{
	if(pNetGame) {
		CLocalPlayer *pLocalPlayer;
		pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		pLocalPlayer->Say(szCmd);
	}
}

//----------------------------------------------------

void cmdQuit(PCHAR szCmd)
{
	if(pNetGame && pNetGame->GetGameState() == GAMESTATE_CONNECTED) {
		pNetGame->GetRakPeer()->Shutdown(500);
	}

	ExitProcess(0);
}

//----------------------------------------------------

void cmdKill(PCHAR szCmd)
{
	pGame->FindPlayerPed()->SetHealth(0.0f);
}

//----------------------------------------------------

void cmdSavePos(PCHAR szCmd)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	FILE *fileOut;
	DWORD dwVehicleID;
	float fZAngle;

	fileOut = fopen("savedpositions.txt","a");
	if(!fileOut) {
		pChatWindow->AddDebugMessage("I can't open the savepositions.txt file for append.");
		return;
	}

	// incar savepos

	if(pPlayer->IsInVehicle()) {

		VEHICLE_TYPE *pVehicle = pPlayer->GetGtaVehicle();
	
		dwVehicleID = GamePool_Vehicle_GetIndex(pVehicle);
		ScriptCommand(&get_car_z_angle,dwVehicleID,&fZAngle);

		fprintf(fileOut,"createVehicle(%u, %.4f, %.4f, %.4f, %.4f, %u, %u)\n",
			pVehicle->entity.nModelIndex,pVehicle->entity.mat.vPos.X,pVehicle->entity.mat.vPos.Y,pVehicle->entity.mat.vPos.Z,
			fZAngle,pVehicle->byteColor1,pVehicle->byteColor2);

		fclose(fileOut);

		return;
	}

	// onfoot savepos
	VECTOR vPos;
	pPlayer->GetPosition(&vPos);
	fZAngle = pPlayer->GetRotation();

	fprintf(fileOut,"addPlayerClass(0, 0, %.4f, %.4f, %.4f, %.4f, 0, 0, 0, 0, 0, 0);\n", vPos.X, vPos.Y, vPos.Z, fZAngle);

	fclose(fileOut);
}

//----------------------------------------------------

void cmdCreateVehicle(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	if(!strlen(szCmd)){
		pChatWindow->AddDebugMessage("Usage: /vehicle (vehicle id).");
		return;
	}	
	int iVehicleType = atoi(szCmd);

	if((iVehicleType < 130) || (iVehicleType > 236))
	{
		pChatWindow->AddDebugMessage("Invalid vehicle ID.");
		return;
	}

	pGame->RequestModel(iVehicleType);
	pGame->LoadRequestedModels();
	while(pGame->IsModelLoaded(iVehicleType)==FALSE) Sleep(5);

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	if(pPlayer) 
	{
		VECTOR vPos;
		pPlayer->GetPosition(&vPos);
		CVehicle *pTestVehicle = pGame->NewVehicle(iVehicleType, (vPos.X - 5.0f), (vPos.Y - 5.0f), vPos.Z, 0.0f);
	}
}

//----------------------------------------------------

void cmdSkin(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	if(!strlen(szCmd)){	
		pChatWindow->AddDebugMessage("Usage: /skin [default.ide number]");
		return;
	}
	int iPlayerSkin = atoi(szCmd);

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	if(pPlayer)
	{
		pGame->RequestModel(iPlayerSkin);
		pGame->LoadRequestedModels();
		while(pGame->IsModelLoaded(iPlayerSkin)==FALSE) Sleep(2);
		pPlayer->SetModel(iPlayerSkin);
	}
}

//----------------------------------------------------

void cmdBuild(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	CCamera * pGameCamera = pGame->GetCamera();
	pGameCamera->Restore();
	pGame->DisplayHud(TRUE);
}

//----------------------------------------------------

void cmdAdmin(PCHAR szCmd)
{
	int iPassLen=0;
	RakNet::BitStream bsSend;

	if(pNetGame) {
		iPassLen = strlen(szCmd);
		if(iPassLen > 64) return;
		bsSend.Write(iPassLen);
		bsSend.Write(szCmd,iPassLen);
		pNetGame->GetRPC4()->Call("Admin",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------

void cmdKickEm(PCHAR szCmd)
{
	if(!strlen(szCmd)) {
	    pChatWindow->AddDebugMessage("Usage: /kick [player_id]");
		return;
	}

	BYTE byteSystemAddress;
	sscanf(szCmd,"%u",&byteSystemAddress);

	RakNet::BitStream bsSend;

	if(pNetGame) {
		bsSend.Write(byteSystemAddress);
		pNetGame->GetRPC4()->Call("KickPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------

void cmdBanTheirSorryAsses(PCHAR szCmd)
{
	int iIPLen=0;
	RakNet::BitStream bsSend;

	if(pNetGame) {
		iIPLen = strlen(szCmd);
		if(iIPLen > 64) return;
		bsSend.Write(iIPLen);
		bsSend.Write(szCmd,iIPLen);
		pNetGame->GetRPC4()->Call("BanIPAddress",&bsSend,HIGH_PRIORITY,RELIABLE,0,UNASSIGNED_SYSTEM_ADDRESS,TRUE);
	}
}

//----------------------------------------------------

void IP2String(DWORD ip, char *ret)
{
	BYTE p[4];

	p[0] = (BYTE)(ip & 0xFF); ip >>= 8;
	p[1] = (BYTE)(ip & 0xFF); ip >>= 8;	
	p[2] = (BYTE)(ip & 0xFF); ip >>= 8;
	p[3] = (BYTE)(ip & 0xFF);

	sprintf(ret,"%u.%u.%u.%u",p[0],p[1],p[2],p[3]);
}

//----------------------------------------------------

void cmdGetIP(PCHAR szCmd)
{
	if(!strlen(szCmd)) {
	    pChatWindow->AddDebugMessage("Usage: /getip [player_id]");
		return;
	}

	BYTE byteSystemAddress;
	sscanf(szCmd,"%u",&byteSystemAddress);
	
	if(pNetGame) {
		CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
		if(pPlayerPool->GetSlotState(byteSystemAddress)) {

			char ret[30];
			DWORD ip = pPlayerPool->GetIPAddress(byteSystemAddress);
			IP2String(ip,ret);
			pChatWindow->AddDebugMessage("%s is: %s",
				pPlayerPool->GetPlayerName(byteSystemAddress),ret);

		} else {
			pChatWindow->AddDebugMessage("That player doesn't appear to be active.");
		}
	}
}

//----------------------------------------------------

void cmdGenComp(PCHAR szCmd)
{

}

//----------------------------------------------------

void cmdNewPlayer(PCHAR szCmd)
{
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	pPlayerPool->New(1, "jaja");
	CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt(1);
	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	BYTE byteSkin = (BYTE)pPlayerPed->GetModelIndex();
	VECTOR vPos;
	pPlayerPed->GetPosition(&vPos);
	float fRotation = pPlayerPed->GetRotation();
	pRemotePlayer->SpawnPlayer(255, byteSkin, &vPos, fRotation, 0,0, 0,0, 0,0);
}

//----------------------------------------------------

void SetupCommands()
{
	pCmdWindow->AddDefaultCmdProc(cmdDefaultCmdProc);
	pCmdWindow->AddCmdProc("quit",cmdQuit);
	pCmdWindow->AddCmdProc("q",cmdQuit);
	pCmdWindow->AddCmdProc("admin",cmdAdmin);
	pCmdWindow->AddCmdProc("kick",cmdKickEm);
	pCmdWindow->AddCmdProc("getip",cmdGetIP);
	pCmdWindow->AddCmdProc("ban",cmdBanTheirSorryAsses);
	pCmdWindow->AddCmdProc("gencomp",cmdGenComp);
	pCmdWindow->AddCmdProc("kill",cmdKill);
	pCmdWindow->AddCmdProc("save",cmdSavePos);
	pCmdWindow->AddCmdProc("v",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("vehicle",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("skin",cmdSkin);
	pCmdWindow->AddCmdProc("build",cmdBuild);
	pCmdWindow->AddCmdProc("np", cmdNewPlayer);
}

//----------------------------------------------------