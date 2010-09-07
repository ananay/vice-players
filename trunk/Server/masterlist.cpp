//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: jenksta, adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "masterlist.h"

CMasterList::CMasterList(char * szAddress, char * szPath, int iServerPort)
{
	m_dwLastPulse = 0;
	char port[5];
	itoa(iServerPort, port, 10);
	m_sPostPath = szPath;
	m_sPostPath += "/add.php?port=";
	m_sPostPath += port;

	m_pTcpInterface = new RakNet::TCPInterface;

	m_pTcpInterface->Start(0, 64);

	m_pHttpConnection = new RakNet::HTTPConnection;

	m_pHttpConnection->Init(m_pTcpInterface, szAddress);

	// TODO: Check that we can access the master list address?
	// TODO: Download the table for confirmation that it listed correctly?
	// TODO: If the result is 'banned' turn off the listed option?
}

CMasterList::~CMasterList()
{
	delete m_pHttpConnection;
	delete m_pTcpInterface;
}

void CMasterList::Pulse()
{
	DWORD dwTick = GetTickCount();
	RakNet::Packet * packet;
	for (packet=m_pTcpInterface->Receive(); packet; m_pTcpInterface->DeallocatePacket(packet), packet=m_pTcpInterface->Receive())
	{
		m_pHttpConnection->ProcessTCPPacket(packet);
	}

	if((dwTick - m_dwLastPulse) > 50000)
	{
		m_pHttpConnection->Post(m_sPostPath.c_str(), NULL);
		m_dwLastPulse = dwTick;
	}

	m_pHttpConnection->Update();
}
