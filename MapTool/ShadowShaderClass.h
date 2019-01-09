#pragma once
#include "Base.h"

class ShadowShaderClass
{
public:
	ShadowShaderClass();
	~ShadowShaderClass();

	bool Initialize(LPDIRECT3DDEVICE9 device);
	bool RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, int vertexSize, int indexSize);

	bool SetShaderParameters(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj);
	void ShutDown();

	LPDIRECT3DTEXTURE9 GetShaderTex() { return m_pShadowRenderTarget; }

private:
	bool InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName);


private:
	LPD3DXEFFECT		m_pShadowShader;
	LPDIRECT3DTEXTURE9	m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9	m_pShadowDepthStencil;
};

