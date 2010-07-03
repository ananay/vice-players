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
		rcon.h
	desc:
		Remote Console Header.
	author:
		spookie

*/

#pragma once
#ifndef VCMPSRV_RCON_H
#define VCMPSRV_RCON_H

class CRcon
{
private:
	RakNet::RakPeerInterface* m_pRak;
public:
	CRcon(WORD iPort, char* szPass, WORD iMaxAdmins);
	~CRcon();

	RakNet::RakPeerInterface* GetRakPeer() { return m_pRak; };

	void Process();

	void ConsoleOutput(char* szOutput);
	void ConsolePrintf( char* szFormat, ... );

	void Packet_NewIncomingConnection(RakNet::Packet* pPacket);
	void Packet_DisconnectionNotification(RakNet::Packet* pPacket);
	void Packet_ConnectionLost(RakNet::Packet* pPacket);
	void Packet_ConnectionBanned(RakNet::Packet* pPacket);
	void Packet_RconCommand(RakNet::Packet* pPacket);

	void OutputChatMessage(char * szNick, char * szText);
	void OutputDeathMessage(int iDeathType,BYTE byteWhoDied,BYTE byteWhoKilled);


};

#endif