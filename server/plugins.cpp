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
// File Author(s): jenksta
//				   adamix
//-----------------------------------------------------
#include "../raknet/WindowsIncludes.h"
#include "netgame.h"
#include "plugins.h"

CPlugins::CPlugins()
{
	for(int i = 0; i < MAX_PLUGINS; i++) {
		m_pPlugins[i] = NULL;
	}
}

CPlugins::~CPlugins()
{
	for(int i = 0; i < MAX_PLUGINS; i++) {
		if(m_pPlugins[i] != NULL) delete m_pPlugins[i];
		m_pPlugins[i] = NULL;
	}
}

bool CPlugins::LoadPlugin(const char *szName)
{
	// make sure a plugin with the same name isn't already loaded
	for(int i = 0; i < MAX_PLUGINS; i++) {
		if(!m_pPlugins[i]) continue;
		if(!strcmp(m_pPlugins[i]->GetPluginName(), szName)) {
			// a plugin with the same name already exists
			return false;
		}
	}
	// find a free plugin slot
	int iSlot = -1;

	for(int i = 0; i < MAX_PLUGINS; i++) {
		if(!m_pPlugins[i]) {
			// found a free slot
			iSlot = i;
			break;
		}
	}

	if(iSlot == -1) {
		// no free slot found
		return false;
	}

	if(m_pPlugins[iSlot] == NULL)
	{
		m_pPlugins[iSlot] = new CPlugin(szName);
	}

	if(!m_pPlugins[iSlot]->Load())
	{
		delete m_pPlugins[iSlot];
		m_pPlugins[iSlot] = NULL;
		return false;
	}

	OnPluginLoad(szName);
	// loaded successfully
	return true;
}

bool CPlugins::UnloadPlugin(const char *szName)
{
	// find the script slot
	for(int i = 0; i < MAX_PLUGINS; i++) {
		if(!strcmp(m_pPlugins[i]->GetPluginName(), szName)) {
			// found the script slot, unload the script

			delete m_pPlugins[i];

			m_pPlugins[i] = NULL;
			// script unloaded successfully
			return true;
		}
	}

	// plugin not found
	return false;
}

void CPlugins::LoadFromConfig(CConfig *pConfig)
{
	int iCount = pConfig->GetConfigArrayCount("PLUGIN");
		iCount++;
	int iLoaded = 0;
	
	for(int i = 1; i < iCount; i++) {
		char * szPluginName = pConfig->GetConfigEntryAsString("PLUGIN", i);

		if(!LoadPlugin(szPluginName)) {
			logprintf("Failed to load plugin %s.", szPluginName);
		} else {
			logprintf("Plugin %s loaded.", szPluginName);
			iLoaded++;
		}
	}
}

void CPlugins::OnPluginLoad(const char * szName)
{
	for(int i = 0; i < MAX_PLUGINS; i++) {
		if(m_pPlugins[i]) {

			OnPluginLoad_t pfn = (OnPluginLoad_t) m_pPlugins[i]->GetProcedureAddress("OnPluginLoad");

			if(!pfn)
				continue;

			pfn(szName);
		}
	}
}

void CPlugins::OnScriptLoad(SQVM *pVM)
{
	for(int i = 0; i < MAX_PLUGINS; i++) {
		if(m_pPlugins[i]) {

			OnScriptLoad_t pfn = (OnScriptLoad_t) m_pPlugins[i]->GetProcedureAddress("OnScriptLoad");

			if(!pfn)
				continue;
			
			pfn(pVM);
		}
	}
}