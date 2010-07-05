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
//----------------------------------------------------------

#include "physical.h"


//----------------------------------------------------------

PHYSICAL_TYPE *CPhysical::GetPhysical()
{
	return (PHYSICAL_TYPE *)GetEntity();
}

//----------------------------------------------------------

void CPhysical::GetMoveSpeed(PVECTOR Vector)
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();
	if(pPhysical) {
		Vector->X = pPhysical->vecMoveSpeed.X;
		Vector->Y = pPhysical->vecMoveSpeed.Y;
		Vector->Z = pPhysical->vecMoveSpeed.Z;
	}
}

//-----------------------------------------------------------

void CPhysical::SetMoveSpeed(VECTOR Vector)
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();
	if(pPhysical) {
		pPhysical->vecMoveSpeed.X = Vector.X;
		pPhysical->vecMoveSpeed.Y = Vector.Y;
		pPhysical->vecMoveSpeed.Z = Vector.Z;
	}
}

//-----------------------------------------------------------

void CPhysical::GetTurnSpeed(PVECTOR Vector)
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();
	if(pPhysical) {
		Vector->X = pPhysical->vecTurnSpeed.X;
		Vector->Y = pPhysical->vecTurnSpeed.Y;
		Vector->Z = pPhysical->vecTurnSpeed.Z;
	}
}

//-----------------------------------------------------------

void CPhysical::SetTurnSpeed(VECTOR Vector)
{
	PHYSICAL_TYPE * pPhysical = (PHYSICAL_TYPE *)GetEntity();
	if(pPhysical) {
		pPhysical->vecTurnSpeed.X = Vector.X;
		pPhysical->vecTurnSpeed.Y = Vector.Y;
		pPhysical->vecTurnSpeed.Z = Vector.Z;
	}
}

//-----------------------------------------------------------