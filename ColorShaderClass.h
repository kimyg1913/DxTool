#pragma once

#include "Base.h"

class ColorShaderClass
{
public:
	ColorShaderClass();
	~ColorShaderClass();

	bool Initialize(LPDIRECT3DDEVICE9 device, HWND hwnd);
	bool RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj);
	
	bool SetShaderParameters(D3DXMATRIX *world, D3DXMATRIX *view, D3DXMATRIX *proj);
	void ShutDown();

private:
	bool InitializeShader(LPDIRECT3DDEVICE9 device, HWND hwnd, LPCWSTR fileName);
	
private:
	LPD3DXEFFECT		m_pColorShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;

};

