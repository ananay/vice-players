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



//			Functions
//---------------------------------
void sq_isConnected(HSQUIRRELVM v);
void sq_setZAngle(HSQUIRRELVM v); // done client rpc
void sq_setPlayerHealth(HSQUIRRELVM v); // done client rpc
void sq_setPlayerArmour(HSQUIRRELVM v); // done client rpc
void sq_setPlayerAction(HSQUIRRELVM v); // done client rpc
void sq_setPlayerRoatation(HSQUIRRELVM v); // done client rpc
void sq_givePlayerWeapon(HSQUIRRELVM v); // done client rpc
void sq_setPlayerPos(HSQUIRRELVM v); // done client rpc
void sq_resetWeapons(HSQUIRRELVM v); // done client rpc
void sq_setArmedWeapon(HSQUIRRELVM v); // done client rpc
void sq_putPlayerInVehicle(HSQUIRRELVM v); // done client rpc
void sq_removePlayerFromVehicle(HSQUIRRELVM v); // done client rpc
void sq_setPlayerSkin(HSQUIRRELVM v); // done client rpc
void sq_togglePlayerControllable(HSQUIRRELVM v); // done client rpc
void sq_getPlayerCurrentAmmo(HSQUIRRELVM v); 
void sq_getPlayerCurrentWeapon(HSQUIRRELVM v);
void sq_getPlayerVehicleID(HSQUIRRELVM v); 
void sq_getPlayerHealth(HSQUIRRELVM v);
void sq_getPlayerArmour(HSQUIRRELVM v);
void sq_getPlayerAction(HSQUIRRELVM v);
void sq_isPlayerInVehicle(HSQUIRRELVM v);
void sq_getPlayerRoatation(HSQUIRRELVM v);
void sq_getVehicleModel(HSQUIRRELVM v);
void sq_createVehicle(HSQUIRRELVM v);
void sq_addPlayerClass(HSQUIRRELVM v);
void sq_setPlayerWorldBounds(HSQUIRRELVM v); // done client rpc
void sq_sendPlayerMessage(HSQUIRRELVM v); // done client rpc
void sq_sendPlayerMessageToAll(HSQUIRRELVM v);
void sq_kickPlayer(HSQUIRRELVM v);
void sq_banPlayer(HSQUIRRELVM v);
