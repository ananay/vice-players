//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Author(s): adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------
class CText 
{
private:
	EntityId m_iID;

	std::string m_szText;
	CD3DFont * m_pFont;
	float m_fPosX;
	float m_fPosY;
	DWORD m_dwColor;

	bool m_bShow;
public:
	CText(EntityId TextID, DWORD color, char * szFontName, int iSize, float posX, float posY, char * szText);
	~CText();

	void SetText(const char * szText) { m_szText = szText; };
	void SetPosition(float fPosX, float fPosY) { m_fPosX = fPosX; m_fPosY = fPosY; };
	void SetColor(DWORD color) { m_dwColor = color; };
	void Process();

	void Show(bool show) { m_bShow = show; };
};