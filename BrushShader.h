#pragma once
#include "Base.h"

class BrushShader
{
public:
	BrushShader();
	~BrushShader();
	bool Initialize(LPDIRECT3DDEVICE9 device);
	bool RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, D3DXVECTOR3 vPos, LPDIRECT3DTEXTURE9 depthTex, LPDIRECT3DTEXTURE9 decalTex, int vertexSize, int indexSize);

	bool SetShaderParameters(D3DXMATRIX *world, D3DXMATRIX *view, D3DXMATRIX *proj, D3DXVECTOR3 vPos, LPDIRECT3DTEXTURE9 depthTex, LPDIRECT3DTEXTURE9 decalTex);
	void ShutDown();

private:
	bool InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName);

private:
	LPD3DXEFFECT		m_pBrushShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;

	LPDIRECT3DVERTEXSHADER9			m_pBrushVertexShader;
	LPDIRECT3DPIXELSHADER9			m_pBrushPixelShader;
};
