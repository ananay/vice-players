//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): Christopher
// License: See LICENSE in root directory
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

CObject::CObject(int iModel, Vector3 * vecPos, Vector3 * vecRot)
{
	DWORD dwReturnID     = 0;
	m_dwGTAId            = 0;

	ScriptCommand(&create_object, iModel, vecPos->X, vecPos->Y, vecPos->Z, &dwReturnID);

	SetEntity((ENTITY_TYPE *)CPools::GetObjectFromIndex(dwReturnID));

	//ScriptCommand(&set_object_rotation, dwReturnID, vecRot->X, vecRot->Y, vecRot->Z);

	m_dwGTAId = dwReturnID;
}

CObject::~CObject()
{
	ScriptCommand(&destroy_object, m_dwGTAId);
}

void CObject::SetObjectTargetable( )
{
	ScriptCommand(&set_target_state, m_dwGTAId);
}

int CObject::HasObjectBeenDamaged( )
{
	int iDamage = 0;

	ScriptCommand(&get_object_state, m_dwGTAId, &iDamage);

	return iDamage;
}