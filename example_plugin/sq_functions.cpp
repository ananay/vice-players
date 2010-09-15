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

#include "sq_functions.h"
#include <stdio.h>

SQInteger sq_helloworld(HSQUIRRELVM pVM)
{
	printf("Hello world from example module!\n");
	sq_pushbool(pVM, true);
	return 1;
}