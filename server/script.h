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
// File Authors: adamix
//
//-----------------------------------------------------

#pragma once
#include "../squirrel/squirrel.h"
#include "config.h"
#include "sq_functions.h"

class CScript
{
private:
	SQVM * m_pVM;
	char m_szScriptName[256];
	char m_szScriptAuthor[256];
	char m_szScriptVersion[256];
public:
	CScript(const char * szScriptName);
	~CScript();

	SQVM * GetVM() { return m_pVM; };

	char * GetScriptName() { return (char*)&m_szScriptName; };
	char * GetScriptAuthor() { return (char*)&m_szScriptAuthor; };
	char * GetScriptVersion() { return (char*)&m_szScriptVersion; };
	void SetScriptAuthor(const char * szAuthor) { strncpy(m_szScriptAuthor, szAuthor, sizeof(m_szScriptAuthor)); }; 
	void SetScriptVersion(const char * szVersion) { strncpy(m_szScriptVersion, szVersion, sizeof(m_szScriptVersion)); }; 
};