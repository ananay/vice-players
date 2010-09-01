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
// Copyright 2010 VC-Players Team
//
// File Author(s): Christopher
//
//----------------------------------------------------------

#include "../../raknet/WindowsIncludes.h"

#include "../main.h"
#include "game.h"
#include "util.h"
#include "pools.h"
#include "keystuff.h"
#include "aimstuff.h"
#include "physical.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CChatWindow   *pChatWindow;


CObject::CObject(int iModel, float fX, float fY, float fZ)
{
	DWORD dwReturnID     = 0;
	m_dwGTAId            = 0;

	ScriptCommand(&create_object, iModel, fX, fY, fZ, &dwReturnID);

	SetEntity((ENTITY_TYPE *)ObjectPool_FindPlayerObject(dwReturnID));

	m_dwGTAId = dwReturnID;
}

CObject::~CObject()
{
	SetEntity((ENTITY_TYPE *)ObjectPool_FindPlayerObject(m_dwGTAId));
	ScriptCommand(&destroy_object, m_dwGTAId);
}

void CObject::SetObjectTargetable( )
{
	ScriptCommand(&set_target_state, m_dwGTAId);
}

int CObject::HasObjectBeenDamaged( )
{
	int iDamage;

	ScriptCommand(&get_object_state, m_dwGTAId, iDamage);

	return iDamage;
}