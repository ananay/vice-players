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
//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
//----------------------------------------------------------

typedef struct _GTA_CONTROL
{
	WORD wUpDownAnalog;
	WORD wLeftRightAnalog;
	WORD wButton2; // (turn left/right) (turret horizontal)
	WORD wButton3; // (turret up/down)
	WORD wButton4; // (action) (next radio)
	WORD wButton5; // (next weapon) (look left) // next/previous might be mixed up
	WORD wButton6; // (aim) (handbrake)
	WORD wButton7; // (previous weapon) (look right) // next/previous might be mixed up
	WORD wButton8; // (forward)
	WORD wButton9; // (backward)
	WORD wButton10; // (left)
	WORD wButton11; // (right)
	WORD wButton12; // ?
	WORD wButton13; // (change camera view)
	WORD wButton14; // (jump) (brake/reverse)
	WORD wButton15; // (enter vehicle) (exit vehicle)
	WORD wButton16; // (sprint) (accelerate)
	WORD wButton17; // (fire)
	WORD wButton18; // (crouch)
	WORD wButton19; // (look behind) (submission)
	WORD wButton20; // ?

} GTA_CONTROL;

typedef struct _GTA_CONTROLSET
{
	GTA_CONTROL currentPadState;
	GTA_CONTROL previousPadState;
	_pad(__pad0, 0xC0);
} GTA_CONTROLSET;

//-----------------------------------------------------------

GTA_CONTROLSET *GameGetInternalKeys();
GTA_CONTROLSET *GameGetLocalPlayerKeys();
GTA_CONTROLSET *GameGetPlayerKeys(int iPlayer);

void GameKeyStatesInit();
void GameStoreLocalPlayerKeys();
void GameSetLocalPlayerKeys();
void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET *pGcsKeyStates);
void GameSetRemotePlayerKeys(int iPlayer);
void GameResetPlayerKeys(int iPlayer);
void GameResetLocalKeys();

//-----------------------------------------------------------