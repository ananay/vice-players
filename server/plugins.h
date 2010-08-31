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
#include "../squirrel/squirrel.h"
#include "config.h"
#include "plugin.h"

#define MAX_PLUGINS 32

//
typedef void                     (* OnPluginLoad_t)(const char * szName);
typedef void                     (* OnScriptLoad_t)(SQVM * pVM);
//

class CPlugins
{
public:
	CPlugins();
	~CPlugins();

	CPlugin * GetScript(int pluginID) { 
		if(!m_pPlugins[pluginID]) return NULL;
		return m_pPlugins[pluginID]; 
	};

	bool LoadPlugin(const char * szName);
	bool UnloadPlugin(const char * szName);
	void LoadFromConfig(CConfig * pConfig);

	void OnPluginLoad(const char * szName);
	void OnScriptLoad(SQVM * pVM);
private:
	CPlugin * m_pPlugins[MAX_PLUGINS];
};