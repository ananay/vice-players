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
// Portions by spookie and Kryptos
//
//----------------------------------------------------

#include "main.h"
#include <process.h>

CGame					*pGame=0;
DWORD					dwGameLoop=0;
DWORD					dwRenderLoop=0;
GAME_SETTINGS			tSettings;
CChatWindow				*pChatWindow=0;
CCmdWindow				*pCmdWindow=0;
CNetGame				*pNetGame=0;

BOOL					bGameInited=FALSE;
BOOL					bWindowedMode=FALSE;
BOOL					bShowNameTags=TRUE;
BOOL					bAntiCheat=TRUE;
bool					D3DInited=FALSE;

IDirect3DDevice8		*pD3DDevice;

HANDLE					hInstance;
CScoreBoard				*pScoreBoard;
CNameTags				*pNameTags;
CNetStats				*pNetStats;
CScripts				*pScripts;

extern BOOL             bScriptInited;

// forwards

void SetupCommands();

void TheGameLoop();
void TheRenderLoop();

void GameDebugDrawDebugScreens();
LONG WINAPI exc_handler(_EXCEPTION_POINTERS* exc_inf);
int DetermineGTAVersion();

#define UNKNOWN_VERSION	0
#define VICE_10			1
#define VICE_11			2

// backwards
//----------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		hInstance = hinstDLL;
		
		InitSettings();

		if(tSettings.bDebug || tSettings.bPlayOnline) {
			// Check the GTA version
			if(DetermineGTAVersion() != VICE_10) {
				MessageBox(0,"Incorrect gta-vc.exe version detected.\nYou must use GTA:VC 1.0 to play Vice City: Players","Vice City: Players Error",MB_OK);
				SetForegroundWindow(HWND_DESKTOP);
				ExitProcess(1);
			}

			dwGameLoop = (DWORD)TheGameLoop;
			dwRenderLoop = (DWORD)TheRenderLoop;

			pGame = new CGame();
			pScripts = new CScripts();

			InstallD3D8Hook();
		}
		// else they must want to play single
		// player or they got the command line
		// arguments wrong.
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
	{
		UninstallD3D8Hook();
		if(pNetGame) {
			pNetGame->Shutdown();
			pScripts->onExit();
		}
	}

	return TRUE;
}

//----------------------------------------------------

void InitD3DStuff()
{
	// Create instances of the chat and input classes.
	pChatWindow = new CChatWindow(pD3DDevice);
	pCmdWindow = new CCmdWindow(pD3DDevice);

	pScoreBoard = new CScoreBoard();
	pNameTags = new CNameTags(pD3DDevice);
	pNetStats = new CNetStats();

	SetupCommands();

	D3DInited = true;
}

//----------------------------------------------------
// Main loop which is called before game processing.
// This is actually a result of a hook inside GTA's
// main loop (TheGameLoop())

void TheGameLoop()
{
	// moved to the d3d end scene callback
}

//----------------------------------------------------

void TheRenderLoop()
{
	
}

//----------------------------------------------------

void TheSceneEnd()
{
	if(bScriptInited) {
		// If the game is not inited then do it now.
		if(!bGameInited && FileCheckSum()) {
			if(tSettings.bPlayOnline) {
				pNetGame = new CNetGame(tSettings.szConnectHost,atoi(tSettings.szConnectPort),
					tSettings.szNickName,tSettings.szConnectPass);
			}

			pGame->ToggleFrameLimiterState(TRUE);
			bGameInited = TRUE;

			return;
		}

		// Process the netgame if it's active.
		if(pNetGame) pNetGame->Process();

		if(!pGame->IsMenuActive() && bShowNameTags) {
			pNameTags->Draw();
		}

		if(!pGame->IsMenuActive()) {
			if((pNetGame) && GetAsyncKeyState(VK_F5)) {
				pGame->DisplayHud(FALSE);
				pScoreBoard->Draw();
			}
			else if((pNetGame) && GetAsyncKeyState(VK_F6)) {
				pGame->DisplayHud(FALSE);
				pNetStats->Draw();
			} 
			else
			{
				pGame->DisplayHud(TRUE);
				if(pChatWindow) pChatWindow->Draw();
				if(pCmdWindow) pCmdWindow->Draw();
			}
		}
	}
}

//----------------------------------------------------

void InitSettings()
{
	PCHAR szCmdLine = GetCommandLine();

	memset(&tSettings,0,sizeof(GAME_SETTINGS));

	while(*szCmdLine) {

		if(*szCmdLine == '-' || *szCmdLine == '/') {
			szCmdLine++;
			switch(*szCmdLine) {
				case 'd':
				case 'D':
					tSettings.bDebug = TRUE;
					tSettings.bPlayOnline = FALSE;
					break;
				case 'c':
				case 'C':
					tSettings.bPlayOnline = TRUE;
					tSettings.bDebug = FALSE;
					break;
				case 'w':
				case 'W':
					bWindowedMode = TRUE;
					break;
				case 'h':
				case 'H':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectHost);
					break;
				case 'p':
				case 'P':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPort);
					break;
				case 'n':
				case 'N':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szNickName);
					break;
				case 'z':
				case 'Z':
					szCmdLine++;
					SetStringFromCommandLine(szCmdLine,tSettings.szConnectPass);
					break;
			}
		}

		szCmdLine++;
	}
}

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString)
{
	while(*szCmdLine == ' ') szCmdLine++;
	while(*szCmdLine &&
		  *szCmdLine != ' ' &&
		  *szCmdLine != '-' &&
		  *szCmdLine != '/') 
	{
		*szString = *szCmdLine;
		szString++; szCmdLine++;
	}
	*szString = '\0';
}

//----------------------------------------------------

int DetermineGTAVersion()
{
	BYTE* VerCheck = (BYTE*)0x608578;
	
	switch (*VerCheck)
	{
	case 0x81: 
		return VICE_11;
	case 0x5D: 
		return VICE_10;
	}
	
	return UNKNOWN_VERSION;
}

//----------------------------------------------------

void logprintf(char * format, ...)
{
	char tmp_buf[512];
	va_list args;
	va_start(args, format);
	vsprintf(tmp_buf, format, args);
	va_end(args);
	puts(tmp_buf);
	FILE* log = fopen("vc-p/client.log", "a");
	fprintf(log, "%s\n", tmp_buf);
	fclose(log);
}

//----------------------------------------------------