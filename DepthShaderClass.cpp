#include "stdafx.h"
#include "DepthShaderClass.h"

DepthShaderClass::DepthShaderClass() : m_pDepthShader(nullptr), m_pDepthRenderTarget(nullptr), m_pDepthStencil(nullptr)
{
}


DepthShaderClass::~DepthShaderClass()
{
}

bool DepthShaderClass::Initialize(LPDIRECT3DDEVICE9 device)
{
	if (!InitializeShader(device, L"Shaders/DepthShader.fx"))
	{
		return false;
	}

	return true;
}

bool DepthShaderClass::RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, int vertexSize, int indexSize)
{

	//현재 백버퍼와 깊이버퍼를 얻어온다.
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHDepthStencilBuffer = NULL;
	device->GetRenderTarget(0, &pHWBackBuffer);
	device->GetDepthStencilSurface(&pHDepthStencilBuffer);

	//그림자맵의 렌더타깃과 깊이 버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(m_pDepthRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		device->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = nullptr;
	}

	device->SetDepthStencilSurface(m_pDepthStencil);

	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	if (!SetShaderParameters(world, view, proj))
	{
		return false;
	}


	m_pDepthShader->SetTechnique("Depth");

	UINT numPasses = 0;

	m_pDepthShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pDepthShader->BeginPass(i);
			{
				// 그림자를 그린다.
				device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexSize, 0, indexSize);
			}
			m_pDepthShader->EndPass();
		}
	}
	m_pDepthShader->End();
	

	device->SetRenderTarget(0, pHWBackBuffer);
	device->SetDepthStencilSurface(pHDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHDepthStencilBuffer->Release();
	pHDepthStencilBuffer = NULL;

	return true;
}

bool DepthShaderClass::SetShaderParameters(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj)
{
	D3DXMATRIX gMVP = *world * *view * *proj;
	D3DXMATRIX gMV = *world * *view;
	D3DXVECTOR4 gVFar = D3DXVECTOR4((float)FAL_PLANE, (float)FAL_PLANE, (float)FAL_PLANE, (float)FAL_PLANE);
	//float gFar = (float)FAL_PLANE;

	m_pDepthShader->SetMatrix("gMVP", &gMVP);
	m_pDepthShader->SetMatrix("gMV", &gMV);
	m_pDepthShader->SetVector("gVFar", &gVFar);

	return true;
}

void DepthShaderClass::ShutDown()
{
	if (m_pDepthStencil)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = nullptr;
	}

	if (m_pDepthRenderTarget)
	{
		m_pDepthRenderTarget->Release();
		m_pDepthRenderTarget = nullptr;
	}

	if (m_pDepthShader)
	{
		m_pDepthShader->Release();
		m_pDepthShader = nullptr;
	}
}

bool DepthShaderClass::InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName)
{
	LPD3DXBUFFER pError = nullptr;

	DWORD dwShaderFlags = 0;

	//렌더타깃을 만든다
	const int shadowMapSize = 2048;

	if (FAILED(device->CreateTexture(DIRECT_WND_WIDTH, DIRECT_WND_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pDepthRenderTarget, NULL)))
	{
		return false;
	}

	if (FAILED(device->CreateDepthStencilSurface(DIRECT_WND_WIDTH, DIRECT_WND_HEIGHT, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pDepthStencil, NULL)))
	{
		return false;
	}

	if (FAILED(D3DXCreateEffectFromFile(device, fileName, NULL, NULL, dwShaderFlags, NULL,
		&m_pDepthShader, &pError)))
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			TCHAR * str = new TCHAR[size];
			wsprintf(str, (const TCHAR*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}

		return false;
	}



	return true;
}
