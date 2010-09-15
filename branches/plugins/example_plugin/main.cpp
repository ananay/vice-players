//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: adamix
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include <stdio.h>
#include "main.h"
#include "sq_utils.h"
#include "sq_functions.h"

char * g_szName = NULL;

EXPORT void OnPluginLoad(char * szName)
{
	printf(">> Hello world from %s plugin!\n", szName);
	g_szName = szName;
}

EXPORT void OnPluginUnload()
{
	printf(">> %s plugin got unloaded.\n", g_szName);
}

EXPORT void OnScriptLoad(SQVM * pVM)
{
	RegisterFunction(pVM, "HelloWorld", sq_helloworld, -1, NULL);
}

EXPORT void OnScriptUnload(SQVM * pVM)
{

}

EXPORT void OnPulse()
{

}