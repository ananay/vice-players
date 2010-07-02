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
//----------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author: kyeman
//
//----------------------------------------------------

#include "main.h"

extern CNetGame *pNetGame;

//----------------------------------------------------

DWORD HealthBarOldStateBlock = 0;
DWORD HealthBarNewStateBlock = 0;

struct HealthBarVertices_s
{
	float x, y, z;
	D3DCOLOR c;
};

HealthBarVertices_s HealthBarBDRVertices[4] = {
	//  x     y      z     rhw   c
	{-0.3f,  -0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},  // 1 . . 4
	{-0.3f,   0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},  // 2 . . 3
	{ 0.3f,   0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)},
	{ 0.3f,  -0.03f, 0.0f, D3DCOLOR_XRGB(0, 0, 0)}
};

HealthBarVertices_s HealthBarBGVertices[4] = {
	//  x     y      z     rhw   c
	{-0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},  // 1 . . 4
	{-0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},  // 2 . . 3
	{ 0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)},
	{ 0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(33, 33, 33)}
};

HealthBarVertices_s HealthBarVertices[4] = {
	//  x     y      z     rhw   c
	{-0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},  // 1 . . 4
	{-0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},  // 2 . . 3
	{ 0.29f,  0.02f, 0.0f, D3DCOLOR_XRGB(0, 255, 0)},
	{ 0.29f, -0.02f, 0.0f, D3DCOLOR_XRGB(0, 255, 0)}
};

D3DMATRIX matView;
CBBFont* bbfont;

CNameTags::CNameTags(IDirect3DDevice8 *pD3DDevice)
{
	bbfont = NULL;
	m_pD3DDevice = pD3DDevice;
}

CNameTags::~CNameTags()
{
	
}

void CNameTags::Draw()
{
	if(!bbfont) 
	{ 
		bbfont = new CBBFont(m_pD3DDevice, "vcmpfnt"); 
		bbfont->Initialise(); 
	} 

	if (!HealthBarOldStateBlock) 
	{ 
		m_pD3DDevice->BeginStateBlock(); 
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 3); 
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_WRAP0, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPING, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 15); 
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, 5); 
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, 6); 
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, 1); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 4); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); 
		m_pD3DDevice->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE); 
		m_pD3DDevice->SetStreamSource(0, NULL, 0); 
		m_pD3DDevice->EndStateBlock(&HealthBarOldStateBlock); 
	} 

	if (!HealthBarNewStateBlock) 
	{ 
		m_pD3DDevice->BeginStateBlock(); 
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); 
		m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 3); 
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_WRAP0, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPING, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0); 
		m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 15); 
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, 5); 
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, 6); 
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, 1); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, 4); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 1); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); 
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 0); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, 1); 
		m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, 1); 
		m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); 
		m_pD3DDevice->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE); 
		m_pD3DDevice->EndStateBlock(&HealthBarNewStateBlock); 
	} 

	m_pD3DDevice->CaptureStateBlock(HealthBarOldStateBlock); 
	m_pD3DDevice->ApplyStateBlock(HealthBarNewStateBlock); 

	D3DXMATRIX matTransposed; 
	D3DXMatrixTranspose(&matTransposed, (D3DXMATRIX*)&matView); 
	matTransposed._14 = matTransposed._24 = matTransposed._34 = 0.0f; 

	if(pNetGame) 
	{ 
		CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool(); 
		for(int x=0; x < MAX_PLAYERS; x++) 
		{ 
			if(pPlayerPool->GetSlotState(x) == TRUE) // player is in use 
			{ 
				CRemotePlayer* Player = pPlayerPool->GetAt(x); 

				if( Player->IsActive() && 
					(Player->GetDistanceFromLocalPlayer() <= 80.0f) ) // Active and within reasonable distance 
				{ 
					CPlayerPed* PlayerPed = Player->GetPlayerPed(); 

					if(PlayerPed->IsOnScreen()) 
					{ // They're onscreen 
						MATRIX4X4 matPlayer; 
						PlayerPed->GetMatrix(&matPlayer); 
						matTransposed._41 = matPlayer.vPos.X; 
						matTransposed._42 = matPlayer.vPos.Y; 
						matTransposed._43 = matPlayer.vPos.Z + 1.0f; 
						m_pD3DDevice->SetTransform(D3DTS_WORLD, &matTransposed); 

						float Health = Player->GetReportedHealth(); 
						if (Health > 100.0f) Health = 100.0f; 

						HealthBarBGVertices[0].x = HealthBarBGVertices[1].x = (0.0058f * Health) - 0.29f; 
						m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBDRVertices, sizeof(HealthBarVertices_s)); 
						m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarVertices,    sizeof(HealthBarVertices_s)); 
						m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, HealthBarBGVertices,  sizeof(HealthBarVertices_s)); 

						if(bbfont) { 
							bbfont->Begin(); 
							//bbfont->Draw(pPlayerPool->GetPlayerName(x), 0.285f, 0xFF000000);
							bbfont->Draw(pPlayerPool->GetPlayerName(x), 0.300f, Player->GetTeamColorAsARGB()); 
							bbfont->End(); 
						} 
					} 
				} 
			} 
		} 
	} 

	m_pD3DDevice->ApplyStateBlock(HealthBarOldStateBlock); 
}