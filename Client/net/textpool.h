//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2010 VC-Players Team
//
// File Author(s): bpeterson
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once

#define MAX_TEXTS 128

class CTextPool
{
public:
	CTextPool();
	~CTextPool();

	EntityId New(EntityId TextID, DWORD color, char * szFontName, int iSize, float posX, float posY, char * szText);
	BOOL Delete(EntityId TextID);

	CText* GetAt(EntityId TextID)
	{
		if(TextID > MAX_TEXTS) { return NULL; }
		return m_pTexts[TextID];
	};

	BOOL GetSlotState(EntityId TextID)
	{
		if(TextID > MAX_TEXTS) { return FALSE; }
		return m_bTextSlotState[TextID];
	};

	EntityId GetFreeSlot();

	void Process();

private:
	BOOL m_bTextSlotState[MAX_TEXTS];
	CText * m_pTexts[MAX_TEXTS];
};
