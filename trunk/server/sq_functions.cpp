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
// File Author: Christopher
// Support From: Jenksta
//-----------------------------------------------------



// setPlayerHealth
int sq_setPlayerHealth(SQVM * pVM)
{
	int playerSystemAddress;
	float newHealthValue;

	sq_getinteger(pVM, -2, &playerSystemAddress);
    sq_getfloat(pVM, -1, &newHealthValue);

	RakNet::BitStream bsSend;
	bsSend.Write(newHealthValue);
	GetRPC4()->Call("Script_SetHealth",&bsSend,HIGH_PRIORITY,RELIABLE,0,playerSystemAddress,false);

	sq_pushbool(pVM, true);
	return 1;
}