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
	m_fPosX = posX;
	m_fPosY = posY;
	m_szText = szText;
}

CText::~CText()
{

}


void CText::ShowForPlayer(EntityId playerId)
{

}

void CText::HideForPlayer(EntityId playerId)
{

}

void CText::InitForPlayer(EntityId playerId)
{
	RakNet::BitStream bsSend;

	bsSend.Write(m_iID);
	bsSend.Write(m_dwColour);
	bsSend.Write(strlen(m_szFont));
	bsSend.Write(m_szFont, strlen(m_szFont));
	bsSend.Write(m_iSize);
	bsSend.Write(m_fPosX);
	bsSend.Write(m_fPosY);
	bsSend.Write(strlen(m_szText));
	bsSend.Write(m_szText, strlen(m_szText));
	
	pNetGame->GetRPC4()->Call("CreateText", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pNetGame->GetRakPeer()->GetSystemAddressFromIndex(playerId), 0);
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

void CText::SetID(EntityId id)
{
	m_iID = id;
}