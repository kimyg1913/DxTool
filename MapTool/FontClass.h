#pragma once
#include "Base.h"

class FontClass
{
public:
	FontClass();
	~FontClass();

	void Shutdown();
	bool Initialize(LPDIRECT3DDEVICE9 device, wstring str, RECT rt, D3DCOLOR fontColor);
	void Update();

	void SetStr(wstring str);
	wstring GetStr() { return m_str; }
	void SetRect(RECT rt);
	RECT GetRct() { return m_rt; }
	void SetFontColor(D3DCOLOR fontColor);

private:
	wstring			m_str;
	RECT			m_rt;
	D3DCOLOR		m_fontColor;
	ID3DXFont*		m_pFont;
};

