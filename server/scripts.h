#pragma once

#include "squirrel/squirrel.h"
#include "config.h"

#define MAX_SCRIPTS 10

class CScripts
{
private:
	SQVM * m_pScripts[MAX_SCRIPTS];
	char m_szScriptNames[MAX_SCRIPTS][256];

public:
	CScripts();

	bool LoadScript(const char * szScriptName);
	bool UnloadScript(const char * szScriptName);
	bool LoadFromConfig(CConfig * pConfig);
	void onServerInit();
	void onServerExit();
};