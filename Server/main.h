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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string>

#define MAX_PLAYER_NAME		24
#define MAX_PLAYERS			50
#define MAX_VEHICLES		200

typedef unsigned char EntityId;

#include "../RakNet/RakPeer.h"
#include "../RakNet/RPC4Plugin.h"
#include "../RakNet/BitStream.h"

#include "system.h"
#include "config.h"

#define DEFAULT_CONFIG_FILE "server.ini"
#define DEFAULT_MAX_PLAYERS 32
#define DEFAULT_LISTEN_PORT 8192
#define DEFAULT_RCON_PORT	8190
#define DEFAULT_RCON_MAXUSERS	8

//----------------------------------------------------

void logprintf(char * format, ...);

#ifndef _WIN32
#include <sys/time.h>
long GetTickCount();
#endif

//----------------------------------------------------

