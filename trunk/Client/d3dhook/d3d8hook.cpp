//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
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