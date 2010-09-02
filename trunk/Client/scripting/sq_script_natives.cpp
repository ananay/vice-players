//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "sq_timer_natives.h"
#include "scripts.h"
#include "../main.h"

extern CNetGame *pNetGame;
extern CScripts *pScripts;

using namespace RakNet;

SQInteger sq_setScriptAuthor(SQVM * pVM)
{
	for(int i = 0; i < MAX_SCRIPTS; i++)
	{
		if(pScripts->GetScript(i))
		{
			if(pScripts->GetScript(i)->GetVM() == pVM)
			{
				CScript * pScript = pScripts->GetScript(i);
				const SQChar * szAuthor;
				sq_getstring(pVM, -1, &szAuthor);
				pScript->SetScriptAuthor(szAuthor);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}

SQInteger sq_setScriptVersion(SQVM * pVM)
{
	for(int i = 0; i < MAX_SCRIPTS; i++)
	{
		if(pScripts->GetScript(i))
		{
			if(pScripts->GetScript(i)->GetVM() == pVM)
			{
				CScript * pScript = pScripts->GetScript(i);
				const SQChar * szVersion;
				sq_getstring(pVM, -1, &szVersion);
				pScript->SetScriptVersion(szVersion);
				sq_pushbool(pVM, true);
				return 1;
			}
		}
	}
	sq_pushbool(pVM, false);
	return 1;
}