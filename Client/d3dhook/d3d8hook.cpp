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

#include "../main.h"
#include "../detours/detours.h"

typedef IDirect3D8 * (WINAPI * Direct3DCreate8_t)(UINT SDKVersion);

Direct3DCreate8_t  m_pfnDirect3DCreate8 = NULL;

IDirect3D8 * WINAPI Direct3DCreate8(UINT SDKVersion)
{
	IDirect3D8 * pDevice = m_pfnDirect3DCreate8(SDKVersion);
	return pDevice ? new IDirect3D8Hook(pDevice) : NULL;
}

void InstallD3D8Hook()
{
	if(!m_pfnDirect3DCreate8) {
		m_pfnDirect3DCreate8 = (Direct3DCreate8_t)DetourFunction(DetourFindFunction("d3d8.dll", "Direct3DCreate8"), (PBYTE)Direct3DCreate8);
	}
}

void UninstallD3D8Hook()
{
	if(m_pfnDirect3DCreate8) {
		DetourRemove((PBYTE)m_pfnDirect3DCreate8, (PBYTE)Direct3DCreate8);
	}
}