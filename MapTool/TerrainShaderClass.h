#pragma once
#include "Base.h"

class TerrainShaderClass
{
public:
	TerrainShaderClass();
	~TerrainShaderClass();

	bool Initialize(LPDIRECT3DDEVICE9 device);
	bool RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIXA16 * world, D3DXMATRIXA16 * view, D3DXMATRIXA16 * proj, int vertexSize, int indexSize);

	bool SetShaderParameters(D3DXMATRIXA16 *world, D3DXMATRIXA16 *view, D3DXMATRIXA16 *proj);
	void ShutDown();

private:
	bool InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName);

private:
	LPD3DXEFFECT		m_pTerrainShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pTerrainDeclaration;

};

