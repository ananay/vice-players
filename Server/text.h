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

class CText
{
private:
	EntityId m_iID;

	DWORD m_dwColour;
	const char * m_szFont;
	int m_iSize;
	float m_fPosX;
	float m_fPosY;
	const char * m_szText;

public:
	CText(DWORD color, const char * szFontName, int iSize, float posX, float posY, const char * szText);
	~CText();

	void ShowForPlayer(EntityId playerId);
	void HideForPlayer(EntityId playerId);

	void SetID(EntityId id);

	void InitForPlayer(EntityId playerId);
	void InitForWorld();
};
