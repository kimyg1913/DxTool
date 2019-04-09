#include "stdafx.h"
#include "BrushClass.h"
#include "D3DClass.h"
#include "BrushShader.h"

#include "Camera.h"

BrushClass::BrushClass() : m_pBrushShader(nullptr), m_IsCreated(false), m_pDepthRenderTarget(nullptr),m_pDepthStencil(nullptr)
{
}


BrushClass::~BrushClass()
{
	if (m_pBrushShader)
	{
		m_pBrushShader->ShutDown();
		delete m_pBrushShader;
		m_pBrushShader = nullptr;
	}

	if (m_pDepthRenderTarget)
	{
		m_pDepthRenderTarget->Release();
	}

	if (m_pDecalTexture)
	{
		m_pDecalTexture->Release();
	}

	if (m_Verticies)
		delete[] m_Verticies;

	if (m_Indicies)
		delete[] m_Indicies;
}

void BrushClass::Initialize(float fRadius)
{

	//D3DXVECTOR4 vertex[8] = { 
	//						D3DXVECTOR4(-0.5f, 0.5f, 0.5f, 1.0f),		//0
	//						D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),		//1
	//						D3DXVECTOR4(0.5f, 0.5f, -0.5f, 1.0f),		//2
	//						D3DXVECTOR4(-0.5f, 0.5f, -0.5f, 1.0f),		//3
	//						D3DXVECTOR4(-0.5f, -0.5f, 0.5f, 1.0f),		//4
	//						D3DXVECTOR4(0.5f, -0.5f, 0.5f, 1.0f),		//5
	//						D3DXVECTOR4(0.5f, -0.5f, -0.5f, 1.0f),		//6
	//						D3DXVECTOR4(-0.5f, -0.5f, -0.5f, 1.0f)		//7
	//};

	//int indicies[36] = { 
	//				0,1,2, 
	//				0,2,3, 
	//				4,6,5, 
	//				4,7,6, 
	//				0,3,7, 
	//				0,7,4, 
	//				1,5,6, 
	//				1,6,2, 
	//				3,2,6, 
	//				3,6,7, 
	//				0,4,5, 
	//				0,5,1
	//};

	VertexType v1, v2,v3,v4;

	v1.pos = D3DXVECTOR4(-(300), (200), 0, 1.0f);
	v2.pos = D3DXVECTOR4((300), (200), 0, 1.0f);
	v3.pos = D3DXVECTOR4(-(300), -(200), 0, 1.0f);
	v4.pos = D3DXVECTOR4((300), -(200), 0, 1.0f);
	v1.tex = D3DXVECTOR2(0.0, 0.0);
	v2.tex = D3DXVECTOR2(1.0, 0.0);
	v3.tex = D3DXVECTOR2(0.0, 1.0);
	v1.tex = D3DXVECTOR2(1.0, 1.0);

	VertexType vertex[4] = {
		v1, v2,v3,v4
	};

	int indicies[6] = {
					0,1,2,
					2,1,3
	};

	if (FAILED(D3DClass::GetInstance()->GetDevice()->CreateVertexBuffer(sizeof(vertex), 0,
		D3DFVF_XYZW, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{

	}

	void * pVerticies;

	m_pVB->Lock(0, sizeof(vertex), (void**)&pVerticies, 0);

	memcpy(pVerticies, vertex, sizeof(vertex));

	m_pVB->Unlock();

	if (FAILED(D3DClass::GetInstance()->GetDevice()->CreateIndexBuffer(sizeof(indicies),
		0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{

	}

	void * pIndices;
	
	m_pIB->Lock(0, sizeof(indicies), (void**)&pIndices, 0);

	memcpy(pIndices, indicies, sizeof(indicies));

	m_pIB->Unlock();

	m_pBrushShader = new BrushShader();

	m_pBrushShader->Initialize(D3DClass::GetInstance()->GetDevice());

	m_IsCreated = true;


	D3DClass::GetInstance()->GetDevice()->CreateTexture(MAIN_WND_WIDTH, MAIN_WND_HEIGHT,
		0, D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&m_pDepthRenderTarget,
		NULL);

	if (FAILED(D3DXCreateTextureFromFile(D3DClass::GetInstance()->GetDevice(), L"Textures/sand1.jpg", &m_pDecalTexture)))
	{
		
	}

}

void BrushClass::UpdateTransform(D3DXVECTOR3 vPos)
{
	m_vPos = vPos;
}

void BrushClass::Render(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 depthTex)
{
	//LPDIRECT3DSURFACE9 pZBuffer = NULL;
	//device->GetDepthStencilSurface(&pZBuffer);

	//m_pDepthRenderTarget->GetSurfaceLevel(0, &pZBuffer);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetFVF(D3DFVF_XYZW | D3DFVF_TEX0);
	device->SetStreamSource(0, m_pVB, 0, sizeof(VertexType));

	device->SetIndices(m_pIB);

	device->SetTexture(0, depthTex);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	device->SetTexture(1, m_pDecalTexture);
	device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	D3DXMATRIX transMatrix;

	//D3DXMatrixTranslation(&transMatrix, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixTranslation(&transMatrix, 0, 0, 0);

	m_pBrushShader->RenderShader(device,
			&transMatrix, Camera::GetInstance()->GetViewMatrix(),
			&D3DClass::GetInstance()->GetWorldProjection(), m_vPos, depthTex, m_pDecalTexture, 8, 2);
}
