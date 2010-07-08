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
//-----------------------------------------------------

#include "sq_timer_natives.h"
#include "../main.h"

extern CNetGame *pNetGame;

using namespace RakNet;

// setTimer(function, interval, repetitions, ...)
SQInteger sq_setTimer(SQVM * pVM)
{
	int iTop = sq_gettop(pVM) - 1;

	int interval;
	int repeations;
	sq_getinteger(pVM, -iTop + 1, &interval);
	sq_getinteger(pVM, -iTop + 2, &repeations);

	if(interval < 1 || repeations < -1 || repeations == 0)
	{
		sq_pushbool(pVM, false);
		return 1;
	}

	SQObjectPtr pFunction;
	pFunction = stack_get(pVM, -iTop);

	int iArgCount = iTop - 3;
	SQObjectPtr * pArguments = NULL;
	if(iArgCount > 0)
	{
		pArguments = new SQObjectPtr[iArgCount];

		for (int i = 3; i < iTop; i++)
			pArguments[i - 3] = stack_get(pVM, -iTop + i);
	}

	int iTimerId = pNetGame->GetTimerPool()->Set(pVM, pFunction, interval, repeations, iArgCount, pArguments);
	if(iTimerId != -1)
	{
		sq_pushinteger(pVM, iTimerId);
		return 1;
	}
	sq_pushbool(pVM, false);
	return 1;
}

// isTimerActive(id)
SQInteger sq_isTimerActive(SQVM * pVM)
{
	int id;
	sq_getinteger(pVM, -1, &id);

	sq_pushbool(pVM, pNetGame->GetTimerPool()->IsActive(id));
	return 1;
}

// killTimer(id)
SQInteger sq_killTimer(SQVM * pVM)
{
	int id;
	sq_getinteger(pVM, -1, &id);

	sq_pushbool(pVM, pNetGame->GetTimerPool()->Kill(id));
	return 1;
}