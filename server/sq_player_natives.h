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

SQInteger sq_setPlayerHealth(SQVM * v); // done
SQInteger sq_setPlayerArmour(SQVM * v); // done
SQInteger sq_setPlayerPosition(SQVM * v); // done
SQInteger sq_setPlayerTurnSpeed(SQVM * pVM); // done
SQInteger sq_setPlayerMoveSpeed(SQVM * pVM); // done
SQInteger sq_getPlayerPosition(SQVM * v); // done
SQInteger sq_setPlayerZAngle(SQVM * v); // done
SQInteger sq_forceClassSelection(SQVM * v); // done
SQInteger sq_getPlayerSkin(SQVM * v); // done
SQInteger sq_setPlayerSkin(SQVM * v); //done
SQInteger sq_setPlayerAction(SQVM * v); // done
SQInteger sq_setPlayerRotation(SQVM * v); // done
SQInteger sq_getPlayerRotation(SQVM * v); //done
SQInteger sq_resetPlayerWeapons(SQVM * v); // done
SQInteger sq_setPlayerArmedWeapon(SQVM * v); // done
SQInteger sq_givePlayerWeapon(SQVM * v); // done
SQInteger sq_removePlayerFromVehicle(SQVM * v); // done
SQInteger sq_putPlayerInVehicle(SQVM * v); // done
SQInteger sq_togglePlayerControls(SQVM * v); // done
SQInteger sq_sendPlayerMessage(SQVM * v); // done
SQInteger sq_sendMessageAsPlayer(SQVM * v);
SQInteger sq_sendPlayerMessageToAll(SQVM * v);
SQInteger sq_setPlayerWorldBounds(SQVM * v); //done
SQInteger sq_setPlayerTime(SQVM * v); //done
SQInteger sq_getPlayerHealth(SQVM * v); //done
SQInteger sq_getPlayerArmour(SQVM * v);
SQInteger sq_getPlayerName(SQVM * v); // done
SQInteger sq_getPlayerIP(SQVM * v); // done
SQInteger sq_isConnected(SQVM * v); // done
SQInteger sq_isPlayerInVehicle(SQVM * v); // done
SQInteger sq_getPlayerVehicleID(SQVM * v); // done
SQInteger sq_setPlayerCameraPos(SQVM * v); // done
SQInteger sq_setPlayerCameraRot(SQVM * v); // done
SQInteger sq_setPlayerCameraLookAt(SQVM * v); // done
SQInteger sq_setCameraBehindPlayer(SQVM * v); // done
SQInteger sq_playSound(SQVM * v); // done
SQInteger sq_fadeScreen(SQVM * v); // done
SQInteger sq_kickPlayer(SQVM * v); // done
SQInteger sq_banIP(SQVM * v); // done
SQInteger sq_getPlayerTeam(SQVM * v); // done
SQInteger sq_getPlayerTurnSpeed(SQVM * v); // done
SQInteger sq_getPlayerMoveSpeed(SQVM * v); // done
SQInteger sq_addPlayerClass(SQVM * v); // done

// Added by VC-Players team.
SQInteger sq_forceClassSelection(SQVM * pVM);
SQInteger sq_togglePlayerBleeding(SQVM * pVM);
SQInteger sq_setItemFlashing(SQVM * pVM);
SQInteger sq_setPlayerSkyColor(SQVM * pVM);
SQInteger sq_setPlayerCash(SQVM * pVM);
SQInteger sq_getPlayerCash(SQVM * pVM);
