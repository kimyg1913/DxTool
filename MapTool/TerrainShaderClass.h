#pragma once
#include "Base.h"

class TerrainShaderClass
{
public:
	TerrainShaderClass();
	~TerrainShaderClass();

	bool Initialize(LPDIRECT3DDEVICE9 device);
	bool RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, int vertexSize, int indexSize, LPDIRECT3DTEXTURE9 diffuseTex);

	bool SetShaderParameters(D3DXMATRIX *world, D3DXMATRIX *view, D3DXMATRIX *proj, LPDIRECT3DTEXTURE9 diffuseTex);
	void ShutDown();

private:
	bool InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName);

private:
	LPD3DXEFFECT		m_pTerrainShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pTerrainDeclaration;

};

