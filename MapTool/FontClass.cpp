#include "stdafx.h"
#include "FontClass.h"


FontClass::FontClass()
{
}


FontClass::~FontClass()
{
}

void FontClass::Shutdown()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

}

bool FontClass::Initialize(LPDIRECT3DDEVICE9 device,wstring str, RECT rt, D3DCOLOR fontColor)
{
	// 폰트를 로딩
	if (FAILED(D3DXCreateFont(device, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
		L"Arial", &m_pFont)))
	{
		return false;
	}

	SetStr(str);
	SetRect(rt);
	SetFontColor(fontColor);

	return true;
}

void FontClass::Update()
{
	m_pFont->DrawText(NULL, m_str.c_str(), -1, &m_rt, 0, m_fontColor);
}

void FontClass::SetStr(wstring str)
{
	m_str = str;
}

void FontClass::SetRect(RECT rt)
{
	m_rt = rt;
}

void FontClass::SetFontColor(D3DCOLOR fontColor)
{
	m_fontColor = fontColor;
}