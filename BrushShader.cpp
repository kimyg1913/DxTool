#include "stdafx.h"
#include "BrushShader.h"



BrushShader::BrushShader() :
	m_pVertexDeclaration(nullptr),
	m_pBrushShader(nullptr)
{
}


BrushShader::~BrushShader()
{

}

bool BrushShader::Initialize(LPDIRECT3DDEVICE9 device)
{
	if (!InitializeShader(device, L"Shaders/BrushShader.fx"))
	{
		return false;
	}

	return true;
}

bool BrushShader::RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj,D3DXVECTOR3 vPos , LPDIRECT3DTEXTURE9 depthTex, LPDIRECT3DTEXTURE9 decalTex, int vertexSize, int indexSize)
{
	bool result;

	result = SetShaderParameters(world, view, proj, vPos, depthTex, decalTex);

	m_pBrushShader->SetTechnique("Brush");

	if (!result)
		return false;

	UINT numPasses = 0;
	m_pBrushShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pBrushShader->BeginPass(i);
			{
				device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexSize, 0, indexSize);
			}
			m_pBrushShader->EndPass();
		}
	}
	m_pBrushShader->End();

	return true;
}

void BrushShader::ShutDown()
{
	if (m_pVertexDeclaration)
	{
		m_pVertexDeclaration->Release();
		m_pVertexDeclaration = nullptr;
	}

	if (m_pBrushShader)
	{
		m_pBrushShader->Release();
		m_pBrushShader = nullptr;
	}
}

bool BrushShader::InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName)
{
	LPD3DXBUFFER pError = nullptr;

	DWORD dwShaderFlags = 0;

	if (FAILED(D3DXCreateEffectFromFile(device, fileName, NULL, NULL, dwShaderFlags, NULL,
		&m_pBrushShader, &pError)))
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

bool BrushShader::SetShaderParameters(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, D3DXVECTOR3 vPos, LPDIRECT3DTEXTURE9 depthTex, LPDIRECT3DTEXTURE9 decalTex)
{
	m_pBrushShader->SetMatrix("gWorldMatrix", world);
	m_pBrushShader->SetMatrix("gViewMatrix", view);
	m_pBrushShader->SetMatrix("gProjectionMatrix", proj);

	D3DXVECTOR4 vCameraRT = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	D3DXMATRIX InvProj;
	D3DXMatrixInverse(&InvProj, NULL, proj);

	D3DXVec4Transform(&vCameraRT, &vCameraRT, &InvProj);
	vCameraRT = vCameraRT / vCameraRT.w;

	D3DXMATRIX InvView;
	D3DXMatrixInverse(&InvView, NULL, view);

	m_pBrushShader->SetMatrix("InvView", &InvView);
	m_pBrushShader->SetVector("CameraRightTop", &vCameraRT);
	m_pBrushShader->SetTexture("Depth_Tex", depthTex);
	m_pBrushShader->SetTexture("Decal_Tex", decalTex);

	return true;
}
