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
/*

	VC:MP Multiplayer Modification
	Copyright 2004-2005 SA:MP Team

	file:
		rcon.cpp
	desc:
		Remote Console.
	author:
		spookie

*/

#include "main.h"
#include "rcon.h"
#include "netgame.h"
#include "../raknet/MessageIdentifiers.h"

using namespace RakNet;

#ifndef WIN32
#	define stricmp strcasecmp
#endif

extern CNetGame			*pNetGame;

CRcon::CRcon(WORD iPort, char* szPass, WORD iMaxAdmins)
{
	m_pRak = RakNet::RakPeerInterface::GetInstance();
	m_pRak->SetIncomingPassword(szPass, strlen(szPass));
	m_pRak->Startup(iMaxAdmins, &SocketDescriptor(iPort, 0), 1);

	if (!m_pRak)
	{
		logprintf("RCON: Unable to start Remote Console server.");
	} else {
		m_pRak->SetMaximumIncomingConnections(iMaxAdmins);
		logprintf("RCON: Remote Console server started.");
	}
}

CRcon::~CRcon()
{
	if (m_pRak)
		RakNet::RakPeerInterface::DestroyInstance(m_pRak);
}

void CRcon::Process()
{
	if (!m_pRak)
		return;

	RakNet::Packet* pPacket;
	while(pPacket = m_pRak->Receive())
	{
		switch (pPacket->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			Packet_NewIncomingConnection(pPacket);
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			Packet_DisconnectionNotification(pPacket);
			break;
		case ID_CONNECTION_LOST:
			Packet_ConnectionLost(pPacket);
			break;
		case ID_RCON_COMMAND:
			Packet_RconCommand(pPacket);
			break;
		}
		m_pRak->DeallocatePacket(pPacket);		
	}
}

void CRcon::ConsoleOutput(char* szOutput)
{
	if (!m_pRak) return;

	BYTE bytePacketId = ID_RCON_RESPONSE;
	RakNet::BitStream bsResponce;
	bsResponce.Write(bytePacketId);
	DWORD dwRespLen = (DWORD)strlen(szOutput);
	bsResponce.Write(dwRespLen);
	bsResponce.Write(szOutput, dwRespLen);

	m_pRak->Send(&bsResponce, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, TRUE);
}

void CRcon::ConsolePrintf( char* szFormat, ... )
{
	char tmp_buf[2048];

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	ConsoleOutput(tmp_buf);
}

void CRcon::Packet_NewIncomingConnection(RakNet::Packet* pPacket)
{
	logprintf("RCON: Admin [%s] has connected.", pPacket->systemAddress.ToString(false));
}

void CRcon::Packet_DisconnectionNotification(RakNet::Packet* pPacket)
{
	logprintf("RCON: Admin [%s] has disconnected.", pPacket->systemAddress.ToString(false));
}

void CRcon::Packet_ConnectionLost(RakNet::Packet* pPacket)
{
	logprintf("RCON: Admin [%s] has lost connection.", pPacket->systemAddress.ToString(false));
}

void CRcon::Packet_RconCommand(RakNet::Packet* pPacket)
{
	DWORD dwCmdLen;
	memcpy(&dwCmdLen, &pPacket->data[1], 4);
	char* cmd = (char*)malloc(dwCmdLen+1);
	memcpy(cmd, &pPacket->data[5], dwCmdLen);
	cmd[dwCmdLen] = 0;

	char* rconcmd = strtok(cmd, " ");
	char* arg = strtok(NULL, " ");
	if (rconcmd)
	{
		if (stricmp(rconcmd, "players") == 0)
		{
			CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
			RakPeerInterface* pRak = pNetGame->GetRakServer();
			ConsoleOutput("\n id\tname\t\tip");
			ConsoleOutput(" --\t----\t\t--\n");
			char output[1024];
			for (int x=0; x<MAX_PLAYERS; x++)
			{
				if (pPlayerPool->GetSlotState(x))
				{
					sprintf(output, " %d\t%s\t\t%s", x,
						pPlayerPool->GetPlayerName(x), pRak->GetSystemAddressFromIndex(x).ToString(true));
					ConsoleOutput(output);
				}
			}
		}
		else if (stricmp(rconcmd, "kick") == 0)
		{
			if(arg) {
				pNetGame->KickPlayer((BYTE)atoi(arg));
			} else {
				ConsoleOutput("RCON: Kick Usage: kick <playerid>");
			}
		}
		else if (stricmp(rconcmd, "ban") == 0)
		{
			if(arg) {
				pNetGame->AddBan(arg);
			} else {
				ConsoleOutput("RCON: Ban Usage: ban <ip_mask>");
			}
		}
		else if (stricmp(rconcmd, "password") == 0)
		{
			pNetGame->GetRakServer()->SetIncomingPassword(arg, strlen(arg));
			if(arg) {
				ConsolePrintf("Password set to: %s", arg);
			} else {
				ConsoleOutput("Password removed.");
			}
		}
		else if (stricmp(rconcmd, "maxplayers") == 0)
		{
			if (arg)
			{
				int iMaxPlayers = atoi(arg);
				if(iMaxPlayers > 32) iMaxPlayers = 32;
				pNetGame->GetRakServer()->SetMaximumIncomingConnections(iMaxPlayers);
				ConsolePrintf("MaxPlayers set to: %d", iMaxPlayers);
			} else {
				ConsoleOutput("RCON: MaxPlayers Usage: maxplayers <num>");
			}
		}
		else if (stricmp(rconcmd, "cmdlist") == 0)
		{
			// *shudder!*
			ConsoleOutput("\n Command List");
			ConsoleOutput(" ------------\n");
			ConsoleOutput(" players		| Get player list.");
			ConsoleOutput(" kick <playerid>	| Kick a player.");
			ConsoleOutput(" ban <ip_mask>		| Ban an IP mask.");
			ConsoleOutput(" maxplayers <num>	| Set maximum player count.");
			ConsoleOutput(" password [newpass]	| Set server password.");
			ConsoleOutput("\n");
		}
		else
		{
			ConsoleOutput("Unknown command.");
		}
	}

	free(cmd);
}


void CRcon::OutputDeathMessage(int iDeathType,BYTE byteWhoDied,BYTE byteWhoKilled)
{
	CPlayerPool *pPool = pNetGame->GetPlayerPool();
	CPlayer *pPlayerWhoDied;
	CPlayer *pPlayerWhoKilled;

	if(iDeathType == TEAM_KILL || iDeathType == VALID_KILL) {
		pPlayerWhoDied = pPool->GetAt(byteWhoDied);
		pPlayerWhoKilled = pPool->GetAt(byteWhoKilled);

		ConsolePrintf("[kill] %s(%u) killed %s(%u)",
			pPool->GetPlayerName(byteWhoKilled),
			pPlayerWhoKilled->GetTeam(),
			pPool->GetPlayerName(byteWhoDied),
			pPlayerWhoDied->GetTeam());
	}
	else if(iDeathType == SELF_KILL) {
		pPlayerWhoDied = pPool->GetAt(byteWhoDied);

		ConsolePrintf("[death] %s(%u) died",
			pPool->GetPlayerName(byteWhoDied),
			pPlayerWhoDied->GetTeam());
	}
}

void CRcon::OutputChatMessage(char * szNick, char * szText)
{
	ConsolePrintf("[chat] %s: %s",szNick,szText);
}
