//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): kyeman
//                 jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#include "common.h"

#define ENTITY_TYPE_UNKNOWN		0
#define ENTITY_TYPE_PED			1
#define ENTITY_TYPE_VEHICLE		2

class CEntity
{
private:
	ENTITY_TYPE	* m_pEntity;

public:
	ENTITY_TYPE * GetEntity();
	void		  SetEntity(ENTITY_TYPE * pEntity);

	void		  GetMatrix(MATRIX4X4 * matMatrix);
	void		  SetMatrix(MATRIX4X4 matMatrix);
	void          GetPosition(Vector3 * vecPosition);
	void          SetPosition(Vector3 vecPosition);
	void          SetHeading(float fHeading);

	WORD		  GetModelIndex();

	BOOL          IsOnScreen();
};