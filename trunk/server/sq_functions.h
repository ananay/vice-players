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

#include "main.h"
#include "scripts.h"

//			Functions
//---------------------------------
SQInteger sq_isConnected(HSQUIRRELVM v); // done
SQInteger sq_setZAngle(HSQUIRRELVM v); // done client rpc
SQInteger sq_setPlayerHealth(HSQUIRRELVM v); // done
SQInteger sq_setPlayerArmour(HSQUIRRELVM v); // done
SQInteger sq_setPlayerAction(HSQUIRRELVM v); // done client rpc
SQInteger sq_setPlayerRotation(HSQUIRRELVM v); // done
SQInteger sq_getPlayerRotation(HSQUIRRELVM v); //done
SQInteger sq_givePlayerWeapon(HSQUIRRELVM v); // done
SQInteger sq_setPlayerPos(HSQUIRRELVM v); // done
SQInteger sq_getPlayerPos(HSQUIRRELVM v); //done
SQInteger sq_resetPlayerWeapons(HSQUIRRELVM v); // done
SQInteger sq_setPlayerArmedWeapon(HSQUIRRELVM v); // done
SQInteger sq_putPlayerInVehicle(HSQUIRRELVM v); // done
SQInteger sq_removePlayerFromVehicle(HSQUIRRELVM v); // done
SQInteger sq_getPlayerSkin(HSQUIRRELVM v);
SQInteger sq_setPlayerSkin(HSQUIRRELVM v); // done
SQInteger sq_togglePlayerControllable(HSQUIRRELVM v); // done
SQInteger sq_getPlayerCurrentAmmo(HSQUIRRELVM v);
SQInteger sq_getPlayerCurrentWeapon(HSQUIRRELVM v);
SQInteger sq_getPlayerVehicleID(HSQUIRRELVM v); //done
SQInteger sq_getPlayerHealth(HSQUIRRELVM v); //done
SQInteger sq_getPlayerArmour(HSQUIRRELVM v); //done
SQInteger sq_getPlayerAction(HSQUIRRELVM v);
SQInteger sq_isPlayerInVehicle(HSQUIRRELVM v); // done
SQInteger sq_getVehicleModel(HSQUIRRELVM v);
SQInteger sq_addPlayerClass(HSQUIRRELVM v); // done
SQInteger sq_setPlayerWorldBounds(HSQUIRRELVM v); // done
SQInteger sq_sendPlayerMessage(HSQUIRRELVM v); // done
SQInteger sq_sendPlayerMessageToAll(HSQUIRRELVM v); // done
SQInteger sq_kickPlayer(HSQUIRRELVM v);
SQInteger sq_banPlayer(HSQUIRRELVM v);
SQInteger sq_getPlayerName(HSQUIRRELVM * v); // done
SQInteger sq_getPlayerIP(HSQUIRRELVM * v); // done
SQInteger sq_createVehicle(HSQUIRRELVM * v); // done
SQInteger sq_destroyVehicle(HSQUIRRELVM * v); // done
int sq_register_vcmp(SQVM * pVM);
