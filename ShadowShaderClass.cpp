#include "stdafx.h"
#include "ShadowShaderClass.h"


ShadowShaderClass::ShadowShaderClass() : m_pShadowShader(nullptr)
{
}


ShadowShaderClass::~ShadowShaderClass()
{
}

bool ShadowShaderClass::Initialize(LPDIRECT3DDEVICE9 device)
{
	if (!InitializeShader(device, L"Shaders/ShadowShader.fx"))
	{
		return false;
	}

	return true;
}

bool ShadowShaderClass::RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, int vertexSize, int indexSize)
{

	//현재 백버퍼와 깊이버퍼를 얻어온다.
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHDepthStencilBuffer = NULL;
	device->GetRenderTarget(0, &pHWBackBuffer);
	device->GetDepthStencilSurface(&pHDepthStencilBuffer);

	//그림자맵의 렌더타깃과 깊이 버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		device->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = nullptr;
	}

	device->SetDepthStencilSurface(m_pShadowDepthStencil);

	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);
	
	if (!SetShaderParameters(world, view, proj))
	{
		return false;
	}

	{
		UINT numPasses = 0;
		m_pShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShadowShader->BeginPass(i);
				{
					// 그림자를 그린다.
					device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexSize, 0, indexSize);
				}
				m_pShadowShader->EndPass();
			}
		}
		m_pShadowShader->End();
	}

	device->SetRenderTarget(0, pHWBackBuffer);
	device->SetDepthStencilSurface(pHDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHDepthStencilBuffer->Release();
	pHDepthStencilBuffer = NULL;

	return true;
}

bool ShadowShaderClass::SetShaderParameters(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj)
{
	m_pShadowShader->SetMatrix("gWorldMatrix", world);
	m_pShadowShader->SetMatrix("gLightViewMatrix", view);
	m_pShadowShader->SetMatrix("gLightProjectionMatrix", proj);

	return true;
}

void ShadowShaderClass::ShutDown()
{
	if (m_pShadowDepthStencil)
	{
		m_pShadowDepthStencil->Release();
		m_pShadowDepthStencil = nullptr;
	}

	if (m_pShadowRenderTarget)
	{
		m_pShadowRenderTarget->Release();
		m_pShadowRenderTarget = nullptr;
	}

	if (m_pShadowShader)
	{
		m_pShadowShader->Release();
		m_pShadowShader = nullptr;
	}
}

bool ShadowShaderClass::InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName)
{
	LPD3DXBUFFER pError = nullptr;

	DWORD dwShaderFlags = 0;

	//렌더타깃을 만든다
	const int shadowMapSize = 2048;

	if (FAILED(device->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL)))
	{
		return false;
	}

	if (FAILED(device->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowDepthStencil, NULL)))
	{
		return false;
	}

	if (FAILED(D3DXCreateEffectFromFile(device, fileName, NULL, NULL, dwShaderFlags, NULL,
		&m_pShadowShader, &pError)))
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
