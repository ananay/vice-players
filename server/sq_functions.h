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
//
//-----------------------------------------------------

#include "main.h"


//			Functions
//---------------------------------
int sq_isConnected(HSQUIRRELVM v);
int sq_setZAngle(HSQUIRRELVM v); // done client rpc
int sq_setPlayerHealth(HSQUIRRELVM v); // done client rpc
int sq_setPlayerArmour(HSQUIRRELVM v); // done client rpc
int sq_setPlayerAction(HSQUIRRELVM v); // done client rpc
int sq_setPlayerRoatation(HSQUIRRELVM v); // done client rpc
int sq_givePlayerWeapon(HSQUIRRELVM v); // done client rpc
int sq_setPlayerPos(HSQUIRRELVM v); // done client rpc
int sq_resetWeapons(HSQUIRRELVM v); // done client rpc
int sq_setArmedWeapon(HSQUIRRELVM v); // done client rpc
int sq_putPlayerInVehicle(HSQUIRRELVM v); // done client rpc
int sq_removePlayerFromVehicle(HSQUIRRELVM v); // done client rpc
int sq_setPlayerSkin(HSQUIRRELVM v); // done client rpc
int sq_togglePlayerControllable(HSQUIRRELVM v); // done client rpc
int sq_getPlayerCurrentAmmo(HSQUIRRELVM v); 
int sq_getPlayerCurrentWeapon(HSQUIRRELVM v);
int sq_getPlayerVehicleID(HSQUIRRELVM v); 
int sq_getPlayerHealth(HSQUIRRELVM v);
int sq_getPlayerArmour(HSQUIRRELVM v);
int sq_getPlayerAction(HSQUIRRELVM v);
int sq_isPlayerInVehicle(HSQUIRRELVM v);
int sq_getPlayerRoatation(HSQUIRRELVM v);
int sq_getVehicleModel(HSQUIRRELVM v);
int sq_createVehicle(HSQUIRRELVM v);
int sq_addPlayerClass(HSQUIRRELVM v);
int sq_setPlayerWorldBounds(HSQUIRRELVM v); // done client rpc
int sq_sendPlayerMessage(HSQUIRRELVM v); // done client rpc
int sq_sendPlayerMessageToAll(HSQUIRRELVM v);
int sq_kickPlayer(HSQUIRRELVM v);
int sq_banPlayer(HSQUIRRELVM v);
