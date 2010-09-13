//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
// License: See LICENSE in root directory
//
//----------------------------------------------------

#pragma once

#include <stdio.h>
#include <string>
#include "../raknet/WindowsIncludes.h"

#define MAX_PLAYER_NAME		24
#define MAX_NETWORK_PLAYERS	50
#define MAX_SETTINGS_STRING 128
#define CLIENT_VERSION "(Alpha-2)"
#define PROJECT_WEBSITE "www.vc-p.com"

// NOTE: Must ALWAYS be unsigned
typedef unsigned char EntityId;

#define INVALID_ENTITY_ID 255

typedef struct _GAME_SETTINGS {
	BOOL bDebug;
	CHAR szConnectHost[MAX_SETTINGS_STRING+1];
	CHAR szConnectPort[MAX_SETTINGS_STRING+1];
	CHAR szConnectPass[MAX_SETTINGS_STRING+1];
	CHAR szNickName[MAX_SETTINGS_STRING+1];
} GAME_SETTINGS;

#include "d3d8/include/d3d8.h"
#include "d3d8/include/d3dfont.h"
#include "d3d8/include/d3dx8tex.h"
#include "d3d8/include/DXUtil.h"

#include "game/CGame.h"
#include "game/CObject.h"
#include "game/CPickup.h"

#include "d3dhook/IDirect3D8Hook.h"
#include "d3dhook/IDirect3DDevice8Hook.h"
#include "d3dhook/IDirectInput8Hook.h"
#include "d3dhook/IDirectInputDevice8Hook.h"
#include "d3dhook/d3d8hook.h"

#include "../raknet/RakPeer.h"
#include "../raknet/RPC4Plugin.h"
#include "../raknet/BitStream.h"
#include "../raknet/MessageIdentifiers.h"

#include "net/CLocalPlayer.h"
#include "net/CRemotePlayer.h"
#include "net/CSpawnSelection.h"
#include "net/netrpc.h"
#include "net/CPlayerPool.h"
#include "net/CVehiclePool.h"
#include "net/CObjectPool.h"
#include "net/CPickupPool.h"
#include "net/CTextPool.h"
#include "net/CCheckpoints.h"
#include "net/CNetGame.h"
#include "scripting/CScripts.h"

#include "CChatWindow.h"
#include "CCmdWindow.h"
#include "CBBFont.h"
#include "CScoreBoard.h"
#include "CNameTags.h"
#include "CNetStats.h"
#include "anticheat.h"

#include "game/scripting.h"

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString);
void InitSettings();


void logprintf(char * format, ...);
//----------------------------------------------------
// EOF

