#include "stdafx.h"
#include "TerrainShaderClass.h"


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

bool TerrainShaderClass::RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, int vertexSize, int indexSize, LPDIRECT3DTEXTURE9 diffuseTex)
{
	if (!SetShaderParameters(world, view, proj, diffuseTex))
	{
		return false;
	}

//	m_pTerrainShader->SetTechnique("Color");


	UINT numPasses = 0;

	m_pTerrainShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pTerrainShader->BeginPass(i);
			{
				//device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
				device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexSize, 0, indexSize);
			}
			m_pTerrainShader->EndPass();
		}
	}
	m_pTerrainShader->End();

	return true;
}

bool TerrainShaderClass::SetShaderParameters(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj, LPDIRECT3DTEXTURE9 diffuseTex)
{
	m_pTerrainShader->SetMatrix("gWorldMatrix", world);
	m_pTerrainShader->SetMatrix("gViewMatrix", view);
	m_pTerrainShader->SetMatrix("gProjectionMatrix", proj);
	m_pTerrainShader->SetTexture("DiffuseMap_Tex", diffuseTex);
	//m_pTerrainShader->set

	return true;
}


bool TerrainShaderClass::InitializeShader(LPDIRECT3DDEVICE9 device, LPCWSTR fileName)
{
	/*D3DVERTEXELEMENT9 declaration[] =
	{
		{0,0,D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0,12,D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	device->CreateVertexDeclaration(declaration, &m_pTerrainDeclaration);
*/
	LPD3DXBUFFER pError = nullptr;

	DWORD dwShaderFlags = 0;

	if (FAILED(D3DXCreateEffectFromFile(device, fileName, NULL, NULL, dwShaderFlags, NULL,
		&m_pTerrainShader, &pError)))
	{
		return false;
	}


	return true;
}
