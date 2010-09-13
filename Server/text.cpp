//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
//				   bpeterson
// License: See LICENSE in root directory
//
//-----------------------------------------------------
#include "netgame.h"
extern CNetGame *pNetGame;

CText::CText(DWORD color, const char * szFontName, int iSize, float posX, float posY, const char * szText)
{
	m_dwColour = color;
	m_szFont = szFontName;
	m_iSize = iSize;
	m_fRelativePosX = posX;
	m_fRelativePosY = posY;
	m_szText = szText;

	m_bShow = false;
}

CText::~CText()
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(pPlayerPool->GetSlotState(i))
		{
			DestroyForPlayer(i);
		}
	}
}

void CText::InitForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_iID);
	bsSend.Write(m_dwColour);
	bsSend.Write(m_szFont.size());
	bsSend.Write(m_szFont.c_str(), m_szFont.size());
	bsSend.Write(m_iSize);
	bsSend.Write(m_fRelativePosX);
	bsSend.Write(m_fRelativePosY);
	bsSend.Write(m_szText.size());
	bsSend.Write(m_szText.c_str(), m_szText.size());
	
	pNetGame->GetRPC4()->Call("CreateText", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
	
	if(m_bShow)
	{
		BitStream bsSend;
		bsSend.Write(m_iID);
		bsSend.Write(true);
		pNetGame->GetRPC4()->Call("Script_toggleTextForPlayer", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
	}
}

void CText::InitForWorld()
{
	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			InitForPlayer(i);
		}
	}
}

void CText::DestroyForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_iID);

	pNetGame->GetRPC4()->Call("DestroyText", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
}

void CText::SetID(EntityId id)
{
	m_iID = id;
}

void CText::SetText(const char * szText)
{
	m_szText = szText;

	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			BitStream bsSend;
			bsSend.Write(m_iID);
			bsSend.Write(strlen(szText));
			bsSend.Write(szText, strlen(szText));
			pNetGame->GetRPC4()->Call("Script_SetText", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i), 0);
		}
	}
}

void CText::Show(bool show)
{
	m_bShow = show;

	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			BitStream bsSend;
			bsSend.Write(m_iID);
			bsSend.Write(m_bShow);
			pNetGame->GetRPC4()->Call("Script_toggleTextForPlayer", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i), 0);
		}
	}
}

void CText::SetPosition(float fPosX, float fPosY)
{
	m_fRelativePosX = fPosX;
	m_fRelativePosY = fPosY;

	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			BitStream bsSend;
			bsSend.Write(m_iID);
			bsSend.Write(fPosX);
			bsSend.Write(fPosY);
			pNetGame->GetRPC4()->Call("Script_SetTextPosition", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i), 0);
		}
	}
}

void CText::SetColor(DWORD color)
{
	m_dwColour = color;

	logprintf("color %d", color);

	CPlayerPool * pPlayerPool = pNetGame->GetPlayerPool();
	for(EntityId i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			BitStream bsSend;
			bsSend.Write(m_iID);
			bsSend.Write(color);
			pNetGame->GetRPC4()->Call("Script_SetTextColor", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(i), 0);
		}
	}
}
