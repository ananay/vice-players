//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: jenksta, adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "../RakNet/TCPInterface.h"
#include "../RakNet/HTTPConnection.h"
#include <string>
class CMasterList
{
private:
	RakNet::TCPInterface * m_pTcpInterface;
	RakNet::HTTPConnection * m_pHttpConnection;
	DWORD m_dwLastPulse;
	std::string m_sPostPath;

public:
	CMasterList(char * szAddress, char * szPath, int iServerPort);
	~CMasterList();
	
	void Pulse();
};
