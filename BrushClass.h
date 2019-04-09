#pragma once
#include "Base.h"
#include "TemplateSingleton.h"

class BrushClass : public TemplateSingleton<BrushClass>
{
public:
	struct VertexType
	{
		D3DXVECTOR4 pos;
		D3DXVECTOR2 tex;
	};

	BrushClass();
	~BrushClass();

	void Initialize(float fRadius);
	void UpdateTransform(D3DXVECTOR3 vPos);
	void Render(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 depthTex);

	bool GetIsCreated() { return m_IsCreated; }
private:
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 * m_Verticies;
	int * m_Indicies;
	bool m_IsCreated;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	LPDIRECT3DTEXTURE9	m_pDepthRenderTarget;
	LPDIRECT3DSURFACE9	m_pDepthStencil;
	LPDIRECT3DTEXTURE9	m_pDecalTexture;

	class BrushShader * m_pBrushShader;

	D3DXVECTOR4 m_vCameraRight;

};

