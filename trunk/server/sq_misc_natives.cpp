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
// File Authors: Christopher, adamix
//
//-----------------------------------------------------

#include "sq_misc_natives.h"
#include "netgame.h"

extern CNetGame *pNetGame;

using namespace RakNet;


SQInteger sq_setGameTime(SQVM * pVM)
{
	SQInteger h, m;

	sq_getinteger(pVM, -2, &h);
	sq_getinteger(pVM, -1, &m);

	pNetGame->GetPlayerPool()->SetGameTime(h, m);
	sq_pushbool(pVM, true);
	return 1;
}

SQInteger sq_getMaxPlayers(SQVM * pVM)
{
	sq_pushinteger(pVM, pNetGame->GetMaxPlayers());
	return 1;
}

SQInteger sq_getTickCount(SQVM * pVM)
{
	sq_pushinteger(pVM, GetTickCount());
	return 1;
}