#pragma once

#include "Base.h"

class ColorShaderClass
{
public:
	ColorShaderClass();
	~ColorShaderClass();

	bool Initialize(LPDIRECT3DDEVICE9 device, HWND hwnd);
	bool RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIXA16 * world, D3DXMATRIXA16 * view, D3DXMATRIXA16 * proj);
	
	bool SetShaderParameters(D3DXMATRIXA16 *world, D3DXMATRIXA16 *view, D3DXMATRIXA16 *proj);
	void ShutDown();

private:
	bool InitializeShader(LPDIRECT3DDEVICE9 device, HWND hwnd, LPCWSTR fileName);
	
private:
	LPD3DXEFFECT		m_pColorShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;

	LPDIRECT3DVERTEXSHADER9			m_pColorVertexShader;
	LPDIRECT3DPIXELSHADER9			m_pColorPixelShader;
};

