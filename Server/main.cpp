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
//-----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//
//-----------------------------------------------------

#ifdef WIN32
#include <stdio.h>
#include "../RakNet/WindowsIncludes.h"
#else
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

#define Sleep(x) usleep(x * 1000)
#endif

#include "main.h"
#include "netgame.h"
#include "rcon.h"
#include "scripts.h"
#include "plugins.h"

#include "../RakNet/BitStream.h"
#include "../RakNet/RakPeerInterface.h"

void fatal_exit(char * szError);

CNetGame	*pNetGame;
CRcon		*pRcon;
CConfig		*pServerConfig;
CScripts	*pScripts;
CPlugins	*pPlugins;
int			iLogState=1;
char		*szAdminPass;

//----------------------------------------------------

int main (int argc, char* argv[])
{
	int iMaxPlayers=0;
	int iListenPort=0;
	char *szPass=NULL;
	int iCurrentGame=1;
	char szConfigFile[512];
	char szError[256];

	int iFriendlyFireOption;
	BYTE byteFriendlyFire=0;
	int iShowOnRadarOption;
	BYTE byteShowOnRadarOption=1;

	int iRconPort = 0;
	int iRconMaxUsers = 0;

	pServerConfig = new CConfig();

	if(argc > 1) {
		strcpy(szConfigFile,argv[1]);
	} else {
		strcpy(szConfigFile,DEFAULT_CONFIG_FILE);
	}

	// try and read the config file.
	if(pServerConfig->ReadFile(szConfigFile) != CCONF_ERR_SUCCESS) {
		sprintf(szError,"Can't read the config file %s\n%s\n- Server Shutting Down. -",
			DEFAULT_CONFIG_FILE,pServerConfig->m_szErrorString);
		fatal_exit(szError);
	}

	//logprintf("Using config file: %s",szConfigFile);

	// get the max players setting
	if((iMaxPlayers=pServerConfig->GetConfigEntryAsInt("MaxPlayers"))==(-1)) {
		iMaxPlayers = DEFAULT_MAX_PLAYERS;
	}

	// cap the max players setting if needed
	if(iMaxPlayers > MAX_PLAYERS)
	{
		iMaxPlayers = MAX_PLAYERS;
	}

	// get the listen port setting
	if((iListenPort=pServerConfig->GetConfigEntryAsInt("ListenPort"))==(-1)) {
		iListenPort = DEFAULT_LISTEN_PORT;
	}

	logprintf("------------------------------------------");
	logprintf("	GTA:Online - Vice City");
	logprintf("	(c) 2010 GTA:Online Team");
	logprintf("    Server started. Build: %s", __DATE__);
	logprintf("    Server Port: %d. Max Players: %d", iListenPort, iMaxPlayers);
	logprintf("------------------------------------------");


	// set the server pass if one is required.
	if(pServerConfig->GetConfigEntryAsBool("NeedPassword")==1) {
		szPass=pServerConfig->GetConfigEntryAsString("Password");
	}

	// get the admin pass
	szAdminPass = pServerConfig->GetConfigEntryAsString("AdminPassword");
	if(!szAdminPass || !strlen(szAdminPass) || !strcmp(szAdminPass,"invalid")) {
		fatal_exit("I need an AdminPassword in the .ini before I can start the server.\n");
	}

	// get the ff option from config (default is OFF)
	iFriendlyFireOption = pServerConfig->GetConfigEntryAsBool("FriendlyFire");
	if(iFriendlyFireOption == 1) byteFriendlyFire = 1;

	// get the show on radar option (default is ON)
	iShowOnRadarOption = pServerConfig->GetConfigEntryAsBool("ShowOnRadar");
	if(iShowOnRadarOption == -1 || iShowOnRadarOption == 0)	{
		byteShowOnRadarOption = 0;
	}

	// create the NetGame.
	pNetGame = new CNetGame(iMaxPlayers,iListenPort,0,szPass,0,byteFriendlyFire,byteShowOnRadarOption);

	// create plugins
	pPlugins = new CPlugins();
	
	// create the scripts
	pScripts = new CScripts();

	pPlugins->LoadFromConfig(pServerConfig);

	// load the scripts from the config file
	if(!pScripts->LoadFromConfig(pServerConfig)) {
		fatal_exit("I need at least one script loaded before I can start the server.\n");
	}

	pScripts->onServerInit();

	// Get the remote console port.
	if (iRconPort = pServerConfig->GetConfigEntryAsInt("RconPort") == -1) {
		iRconPort = DEFAULT_RCON_PORT;
	} else {
		iRconPort = pServerConfig->GetConfigEntryAsInt("RconPort");
	}

	// Get the remote console max users.
	if (iRconMaxUsers = pServerConfig->GetConfigEntryAsInt("RconMaxUsers") <= 0) {
		iRconMaxUsers = DEFAULT_RCON_MAXUSERS;
	} else {
		iRconMaxUsers = pServerConfig->GetConfigEntryAsInt("RconMaxUsers");
	}

	// create rcon
	pRcon = new CRcon(iRconPort, szAdminPass, iRconMaxUsers);

	// Process the network game.
	while(pNetGame->GetGameState() == GAMESTATE_RUNNING)
	{
		pNetGame->Process();
		pRcon->Process();

		if(pScripts)
			pScripts->onServerPulse();

		if(pPlugins)
			pPlugins->OnPulse();

		Sleep(5);

	}

	pScripts->onServerExit();

	delete pScripts;
	delete pRcon;
	delete pNetGame;

	return 0;
}

//----------------------------------------------------

void fatal_exit(char * szError)
{
	#ifdef WIN32
		printf("%s\n\n",szError);
		printf("Press any key to close.");
		getc(stdin);
	#else
        printf("%s\n\n",szError);
    #endif
		exit(1);
}

//----------------------------------------------------

void logprintf(char * format, ...)
{
	char tmp_buf[512];
	if(iLogState) {
		va_list args;
		va_start(args, format);
		vsprintf(tmp_buf, format, args);
		va_end(args);
		puts(tmp_buf);
		FILE* log = fopen("server.log", "a");
		fprintf(log, "%s\n", tmp_buf);
		fclose(log);
	}
}

//----------------------------------------------------

void FilterInvalidNickChars(PCHAR szString)
{
	while(*szString) {
		if(*szString < ' ' || *szString > 'z') {
			*szString = '_';
		}
		szString++;
	}
}

//----------------------------------------------------

#ifndef _WIN32
long GetTickCount()
{
        struct timeval tv;
        if(gettimeofday(&tv, NULL) != 0)
                return 0;

        return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
#endif

//----------------------------------------------------
