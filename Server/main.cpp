//-----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
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

	// Create the config instance
	pServerConfig = new CConfig();

	if(argc > 1) {
		strcpy(szConfigFile, argv[1]);
	} else {
		strcpy(szConfigFile, DEFAULT_CONFIG_FILE);
	}

	// Try and read the config file.
	if(pServerConfig->ReadFile(szConfigFile) != CCONF_ERR_SUCCESS)
	{
		// Read failed, exit
		sprintf(szError,"Can't read the config file %s\n%s\n- Server Shutting Down. -",
			DEFAULT_CONFIG_FILE,pServerConfig->m_szErrorString);
		fatal_exit(szError);
	}

	//logprintf("Using config file: %s",szConfigFile);

	// Get the max players setting
	if((iMaxPlayers=pServerConfig->GetConfigEntryAsInt("MaxPlayers"))==(-1)) {
		iMaxPlayers = DEFAULT_MAX_PLAYERS;
	}

	// Cap the max players setting if needed
	if(iMaxPlayers > MAX_PLAYERS)
	{
		iMaxPlayers = MAX_PLAYERS;
	}

	// Get the listen port setting
	if((iListenPort=pServerConfig->GetConfigEntryAsInt("ListenPort"))==(-1)) {
		iListenPort = DEFAULT_LISTEN_PORT;
	}

	// Print the startup string
	logprintf("------------------------------------------");
	logprintf("	GTA:Online - Vice City");
	logprintf("	(c) 2010 GTA:Online Team");
	logprintf("    Server started. Build: " __DATE__);
	logprintf("    Server Port: %d. Max Players: %d", iListenPort, iMaxPlayers);
	logprintf("------------------------------------------");


	// Set the server password if one is required.
	if(pServerConfig->GetConfigEntryAsBool("NeedPassword")==1) {
		szPass = pServerConfig->GetConfigEntryAsString("Password");
	}

	// Get the admin password
	szAdminPass = pServerConfig->GetConfigEntryAsString("AdminPassword");
	if(!szAdminPass || !strlen(szAdminPass) || !strcmp(szAdminPass,"invalid")) {
		fatal_exit("I need an AdminPassword in the .ini before I can start the server.\n");
	}

	// Get the friendly fire option from config (default is OFF)
	iFriendlyFireOption = pServerConfig->GetConfigEntryAsBool("FriendlyFire");
	if(iFriendlyFireOption == 1) byteFriendlyFire = 1;

	// Get the show on radar option (default is ON)
	iShowOnRadarOption = pServerConfig->GetConfigEntryAsBool("ShowOnRadar");
	if(iShowOnRadarOption == -1 || iShowOnRadarOption == 0)	{
		byteShowOnRadarOption = 0;
	}

	// Create the net game instance
	pNetGame = new CNetGame(iMaxPlayers, iListenPort, szPass, byteFriendlyFire, byteShowOnRadarOption);

	// Create the plugins instance
	pPlugins = new CPlugins();
	
	// Create the scripts instance
	pScripts = new CScripts();

	// Load the plugins from the config file
	pPlugins->LoadFromConfig(pServerConfig);

	// Load the scripts from the config file
	if(!pScripts->LoadFromConfig(pServerConfig)) {
		fatal_exit("I need at least one script loaded before I can start the server.\n");
	}

	// Call the scripts onServerInit function
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

	// Create the rcon instance
	pRcon = new CRcon(iRconPort, szAdminPass, iRconMaxUsers);

	// Loop until the game state is no longer set to running
	while(pNetGame->GetGameState() == GAMESTATE_RUNNING)
	{
		// Process the net game instance
		if(pNetGame)
			pNetGame->Process();

		// Process the rcon instance
		if(pRcon)
			pRcon->Process();

		// Call the scripts onServerPulse function
		if(pScripts)
			pScripts->onServerPulse();

		// Call the plugins OnPulse function
		if(pPlugins)
			pPlugins->OnPulse();

		Sleep(5);
	}

	// Call the scripts onServerExit function
	pScripts->onServerExit();

	// Delete the rcon instance
	SAFE_DELETE(pRcon);

	// Delete the scripts instance
	SAFE_DELETE(pScripts);

	// Delete the plugins instance
	SAFE_DELETE(pPlugins);

	// Delete the net game instance
	SAFE_DELETE(pNetGame);

	// Delete the config instance
	SAFE_DELETE(pServerConfig);

	return 0;
}

//----------------------------------------------------

void fatal_exit(char * szError)
{
	// Print the error string
	printf("%s\n\n", szError);

#ifdef WIN32
	// Wait for user input
	printf("Press any key to close.");
	getc(stdin);
#endif

	// Exit
	exit(1);
}

//----------------------------------------------------

void logprintf(char * szFormat, ...)
{
	if(iLogState)
	{
		// Collect the function arguments
		va_list args;
		char szLogString[512];
		va_start(args, szFormat);
		vsprintf(szLogString, szFormat, args);
		va_end(args);

		// Print the log string to the console
		puts(szLogString);

		// Open the log file
		FILE * fLog = fopen("server.log", "a");

		if(fLog)
		{
			// Print the log string to the log file
			fprintf(fLog, "%s\n", szLogString);

			// Close the log file
			fclose(fLog);
		}
	}
}

//----------------------------------------------------

void FilterInvalidNickChars(PCHAR szString)
{
	while(*szString)
	{
		if(*szString < ' ' || *szString > 'z')
		{
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

        return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
#endif

//----------------------------------------------------
