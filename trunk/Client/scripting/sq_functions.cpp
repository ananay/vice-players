//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: Christopher
//               adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "sq_functions.h"
#include "../main.h"
#include "sq_utils.h"
extern CNetGame *pNetGame;

using namespace RakNet;

int sq_register_natives(SQVM * pVM)
{
	RegisterFunction(pVM, "setScriptAuthor", sq_setScriptAuthor, 2 , ".s");
	RegisterFunction(pVM, "setScriptVersion", sq_setScriptVersion, 2, ".s");
	RegisterFunction(pVM, "addChatMessage", sq_addChatMessage, 2, ".s");
	RegisterFunction(pVM, "_call", sq__call, -1, NULL);
	RegisterFunction(pVM, "toggleCheatCodes", sq_toggleCheatCodes, 2, ".b");

	// Timers
	RegisterFunction(pVM, "setTimer", sq_setTimer, -1, NULL);
	RegisterFunction(pVM, "killTimer", sq_killTimer, -1, NULL);
	RegisterFunction(pVM, "isTimerActive", sq_isTimerActive, -1, NULL);
	return 1;
}
