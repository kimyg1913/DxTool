#include "stdafx.h"
#include "TerrainShaderClass.h"
#include "Camera.h"


TerrainShaderClass::TerrainShaderClass() :
	m_pTerrainShader(nullptr)
{
}


TerrainShaderClass::~TerrainShaderClass()
{
}

void TerrainShaderClass::ShutDown()
{
	if (m_pTerrainShader)
	{
		m_pTerrainShader->Release();
		m_pTerrainShader = nullptr;
	}
}
bool TerrainShaderClass::Initialize(LPDIRECT3DDEVICE9 device)
{
	if (!InitializeShader(device, L"Shaders/TerrainShader.fx"))
	{
		return false;
	}

	return true;
}

bool TerrainShaderClass::RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, int vertexSize, int indexSize, LPDIRECT3DTEXTURE9 diffuseTex, D3DXVECTOR4 * vLightDiffuse, D3DXVECTOR3 * vLightDir
	, D3DXMATRIX * lightView, D3DXMATRIX * lightProj , LPDIRECT3DTEXTURE9 shadowTex)
{

	if (!SetShaderParameters(world, view, proj, diffuseTex, vLightDiffuse, vLightDir, lightView, lightProj , shadowTex))
	{
		return false;
	}

	UINT numPasses = 0;

	m_pTerrainShader->SetTechnique("Terrain");
	
	m_pTerrainShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pTerrainShader->BeginPass(i);
			{
				device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexSize, 0, indexSize);
			}
			m_pTerrainShader->EndPass();
		}
	}
	m_pTerrainShader->End();

	return true;
}

bool TerrainShaderClass::SetShaderParameters(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, LPDIRECT3DTEXTURE9 diffuseTex, D3DXVECTOR4 * vLightDiffuse, D3DXVECTOR3 * vLightPosition
	, D3DXMATRIX * lightView, D3DXMATRIX * lightProj , LPDIRECT3DTEXTURE9 shadowTex)
{
	m_pTerrainShader->SetMatrix("gWorldMatrix", world);
	m_pTerrainShader->SetMatrix("gViewMatrix", view);
	m_pTerrainShader->SetMatrix("gProjectionMatrix", proj);
	m_pTerrainShader->SetMatrix("gLightViewMatrix", lightView);
	m_pTerrainShader->SetMatrix("gLightProjectionMatrix", lightProj);

	D3DXVECTOR4 vLP = D3DXVECTOR4(vLightPosition->x, vLightPosition->y, vLightPosition->z, 1.0f);
	m_pTerrainShader->SetVector("lightPosition", &vLP);

	D3DXVECTOR3 * vCP = Camera::GetInstance()->GetEye();
	D3DXVECTOR4 vCP4 = D3DXVECTOR4(vCP->x, vCP->y, vCP->z, 1.0f);
	m_pTerrainShader->SetVector("cameraPosition", &vCP4);

	m_pTerrainShader->SetTexture("DiffuseMap_Tex", diffuseTex);

	if(shadowTex)
		m_pTerrainShader->SetTexture("ShadowMap_Tex", shadowTex);

	return true;
}


bool TerrainShaderClass::InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName)
{

	LPD3DXBUFFER pError = nullptr;

	DWORD dwShaderFlags = 0;

	if (FAILED(D3DXCreateEffectFromFile(device, fileName, NULL, NULL, dwShaderFlags, NULL,
		&m_pTerrainShader, &pError)))
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
