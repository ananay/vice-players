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
#include "netgame.h"

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

SQInteger sq_loadClientScript(SQVM * pVM)
{
	SQInteger playerSystemAddress;
	const char * szScript;

	sq_getinteger(pVM, -2, &playerSystemAddress);
	sq_getstring(pVM, -1, &szScript);

	if(pNetGame->GetPlayerPool()->GetSlotState(playerSystemAddress))
	{
		long nameSize;
		std::string str = "clientscripts/";
		std::string script;
		RakNet::BitStream bs;

		nameSize = strlen(szScript);
		str.append(szScript);

		FILE *f = fopen(str.c_str(), "rb");
		if(f)
		{
			char szInput[128];
			memset(szInput, '\0', 128);
			fseek(f, 0, SEEK_END);
			long fileSize = ftell(f);
			rewind(f);
			while(!feof(f))
			{
				fgets(szInput, 128, f);
				script.append(szInput);
			}
			bs.Write(nameSize);
			bs.Write(script.size());
			bs.Write(fileSize);
			bs.Write(szScript, nameSize);
			bs.Write(script.c_str(), script.size());
			pNetGame->GetRPC4()->Call("UploadClientScript",&bs,LOW_PRIORITY,UNRELIABLE,0,pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerSystemAddress),false);

			fclose(f);
			sq_pushbool(pVM, true);
			return 1;
		}
	}

	sq_pushbool(pVM, false);
	return 1;
}