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
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
//-----------------------------------------------------

#pragma once
#include "squirrel/squirrel.h"
#include "config.h"
#include "script.h"

#define MAX_SCRIPTS 10

class CScripts
{
private:
	CScript * m_pScripts[MAX_SCRIPTS];
	char m_szScriptNames[MAX_SCRIPTS][256];

public:
	CScripts();

	bool LoadScript(const char * szScriptName);
	bool UnloadScript(const char * szScriptName);
	bool LoadFromConfig(CConfig * pConfig);

	CScript * GetScript(int scriptID) { 
		if(!m_pScripts[scriptID]) return NULL;
		return m_pScripts[scriptID]; 
	};

	void onServerInit();
	void onServerExit();
	void onServerPulse();
	bool onRconCommand(const char *command, const char *arg);
	void onBan(const char *szText);
	void onKick(int playerId);

	void onPlayerConnect(int playerId);
	void onPlayerDisconnect(int playerId, int reason);
	bool onPlayerText(int playerId, const char *text);
	void onPlayerCommand(int playerId, const char *command);
	void onPlayerSpawn(int playerId);
	void onPlayerRequestClass(int playerId, int classid);
	void onPlayerDeath(int playerId, int killerId, int reason);
	void onPlayerEnterVehicle(int playerId, int vehicleid);
	void onPlayerExitVehicle(int playerId, int vehicleid);
	void onPlayerSync(int playerId);
	void onPlayerDamage(int playerId, int oldhp, int newhp);

	void onVehicleCreate(int vehicleid);
	void onVehicleDestroy(int vehicleid);
	void onVehicleSync(int vehicleid);
	void onVehicleDamage(int vehicleId, float oldhp, float newhp);
	void onVehicleSpawn(int vehicleid);
	void onVehicleDeath(int vehicleid);

	void onTimerCreate(int timerId);
	void onTimerDestroy(int timerId);
};