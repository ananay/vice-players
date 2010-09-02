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
// File Author(s): MaVe
//                 jenksta
//
//-----------------------------------------------------
#pragma once
#include "main.h"
#include "scripting/scripts.h"
#include "../squirrel/squirrel.h"
#include "../squirrel/sqvm.h"

#define MAX_TIMERS 100

struct ScriptTimer
{
	SQObjectPtr pFunction;
	unsigned int uInterval;
	int iRemainingRepeations;
	SQVM * pScriptVM;
	unsigned int uTick;
	int iArgCount;
	SQObjectPtr * pArguments;
};

class CTimerPool
{
private:
	ScriptTimer * m_Timers[MAX_TIMERS];
public:
	CTimerPool();
	~CTimerPool();
	void Process();

	int Set(SQVM * pScriptVM, SQObjectPtr pFunction, int iInterval, int iRepeations, int iArgCount, SQObjectPtr * pArguments);
	bool Kill(int iTimerId);
	void HandleScriptUnload(SQVM * pScriptVM);
	bool IsActive(int iTimerId);
};