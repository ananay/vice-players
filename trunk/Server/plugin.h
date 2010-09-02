//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------
#include "../squirrel/squirrel.h"
#include "config.h"

typedef void                     (* OnPluginUnload_t)();

class CPlugin {
public:
	CPlugin(const char * szName);
	~CPlugin();
	bool Load();

	void * GetProcedureAddress(const char * szProcName);
	const char * GetPluginName() { return m_szName; };
private:
	void * m_pLibrary;
	const char * m_szName;
};