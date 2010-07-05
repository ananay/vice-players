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
#include <stdio.h>

extern CNetGame* pNetGame;
extern GAME_SETTINGS tSettings;
extern IDirect3DDevice8 *pD3DDevice;
D3DDISPLAYMODE DisplayMode;
extern CChatWindow * pChatWindow;


void CScoreBoard::Draw()
{
	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	//DWORD dwNickColor;
	//char * szNick;
	DWORD dwChatColor = 0xFFFFFFFF;
	char * szNameText = (char *)malloc(128);
	char szScoreText[32];
	char szPingText[32];
	int iCurrent = 0;

	pNetGame->UpdatePlayerScoresAndPings();

	pD3DDevice->GetDisplayMode(&DisplayMode);
	// Get the d3d font pointer
	CD3DFont * pFont = pChatWindow->m_pD3DFont;

	// Draw the background
	pFont->DrawBox((float)(DisplayMode.Width / 4), (float)(DisplayMode.Height / 4), (float)(DisplayMode.Width / 2), (float)(DisplayMode.Height / 2), 0.0f, 0x50000000);

	// Draw the title
	SIZE size;
	char szTitle[128];
	sprintf(szTitle, "Vice City: Players - %s:%s",  tSettings.szConnectHost,tSettings.szConnectPort);
	pFont->GetTextExtent(szTitle, &size);
	pFont->DrawText((float)((DisplayMode.Width / 2) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 40)), 0xFFFFFFFF, szTitle);

	// Draw the player title
	const char * szPlayerTitleText = "Player";
	pFont->GetTextExtent(szPlayerTitleText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + (DisplayMode.Width / 8)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 20)), 0xFFFFFFFF, szPlayerTitleText);

	// Draw the score title
	const char * szScoreTitleText = "Score";
	pFont->GetTextExtent(szScoreTitleText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 2)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 20)), 0xFFFFFFFF, szScoreTitleText);

	// Draw the ping title
	const char * szPingTitleText = "Ping";
	pFont->GetTextExtent(szPingTitleText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 3)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 20)), 0xFFFFFFFF, szPingTitleText);

	// Format the local player name
	/*dwNickColor = pPlayerPool->GetLocalPlayer()->GetTeamColorAsARGB();
	szNick = pPlayerPool->GetLocalPlayerName();
	*szNameText = '\01'; szNameText++; // color start
	memcpy(szNameText, &dwNickColor, sizeof(DWORD)); szNameText += 4; // nick color
	memcpy(szNameText, szNick, strlen(szNick)); szNameText += strlen(szNick); // nick string
	*szNameText = '\01'; szNameText++; // color start
	memcpy(szNameText, &dwChatColor, sizeof(DWORD)); szNameText += 4; // chat color
	*szNameText = '\0';*/
	sprintf(szNameText, "%s", pPlayerPool->GetLocalPlayerName());

	// Draw the local player name
	pFont->GetTextExtent(szNameText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + (DisplayMode.Width / 8)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szNameText);

	// Format the local player score
	sprintf(szScoreText, "%d", pPlayerPool->GetLocalPlayerScore());

	// Draw the local player score
	pFont->GetTextExtent(szScoreText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 2)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szScoreText);

	// Format the local player ping
	sprintf(szPingText, "%d", pPlayerPool->GetLocalPlayerPing());

	// Draw the local player ping
	pFont->GetTextExtent(szPingText, &size);
	pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 3)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szPingText);

	iCurrent ++;

	for(BYTE i = 0; i < MAX_PLAYERS; i++) {
		if(pPlayerPool->GetSlotState(i)) {
			// Format the player name
			/*dwNickColor = pPlayerPool->GetAt(i)->GetTeamColorAsARGB();
			szNick = pPlayerPool->GetPlayerName(i);
			*szNameText = '\01'; szNameText++; // color start
			memcpy(szNameText, &dwNickColor, sizeof(DWORD)); szNameText += 4; // nick color
			memcpy(szNameText, szNick, strlen(szNick)); szNameText += strlen(szNick); // nick string
			*szNameText = '\01'; szNameText++; // color start
			memcpy(szNameText, &dwChatColor, sizeof(DWORD)); szNameText += 4; // chat color
			*szNameText = '\0';*/
			sprintf(szNameText, "%s", pPlayerPool->GetPlayerName(i));

			// Draw the player name
			pFont->GetTextExtent(szNameText, &size);
			pFont->DrawText((float)(((DisplayMode.Width / 4) + (DisplayMode.Width / 8)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szNameText);

			// Format the player score
			sprintf(szScoreText, "%d", pPlayerPool->GetScore(i));

			// Draw the player score
			pFont->GetTextExtent(szScoreText, &size);
			pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 2)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szScoreText);

			// Format the player ping
			sprintf(szPingText, "%d", pPlayerPool->GetPing(i));

			// Draw the player ping
			pFont->GetTextExtent(szPingText, &size);
			pFont->DrawText((float)(((DisplayMode.Width / 4) + ((DisplayMode.Width / 8) * 3)) - (size.cx / 2)), (float)((DisplayMode.Height / 4) + (DisplayMode.Height / 14) + ((DisplayMode.Height / 60) * iCurrent)), 0xFFFFFFFF, szPingText);

			iCurrent++;
		}
	}

	free(szNameText);
}
