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
//                 adamix
//
//-----------------------------------------------------
#include "../raknet/WindowsIncludes.h"
#include "netgame.h"
#include <stdio.h>
#include "plugin.h"

extern CNetGame *pNetGame;

#ifndef WIN32
#include "dlfcn.h"
#endif

CPlugin::CPlugin(const char *szName)
{
	m_pLibrary = NULL;
	m_szName = szName;
}

bool CPlugin::Load()
{
	if(!m_pLibrary)
	{
		std::string szPath = "plugins/";
		szPath += m_szName;
	#ifdef WIN32
		szPath += ".dll";
		m_pLibrary = LoadLibrary(szPath.c_str());
	#else
		szPath += ".so";
		m_pLibrary = dlopen(szPath.c_str(), RTLD_LAZY);
	#endif
		if(!m_pLibrary)
		{
			return false;
		}
		return true;
	}
	return false;
}

CPlugin::~CPlugin()
{
	if(m_pLibrary)
	{
#ifdef WIN32
		FreeLibrary((HMODULE)m_pLibrary);
#else
		dlclose(m_pLibrary);
#endif
		m_pLibrary = NULL;
	}
}


void * CPlugin::GetProcedureAddress(const char * szProcName)
{
	if(m_pLibrary)
	{
		void * pProcAddress = NULL;
#ifdef WIN32
		pProcAddress = (void *)GetProcAddress((HMODULE)m_pLibrary, szProcName);
#else
		pProcAddress = dlsym(m_pLibrary, szProcName);
#endif
		return pProcAddress;
	}

	return NULL;
}
