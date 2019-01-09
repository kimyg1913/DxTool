#include "stdafx.h"
#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass() :
	m_pVertexDeclaration(nullptr),
	m_pColorPixelShader(nullptr)
{
}


ColorShaderClass::~ColorShaderClass()
{

}

bool ColorShaderClass::Initialize(LPDIRECT3DDEVICE9 device, HWND hwnd)
{
	if (!InitializeShader(device, hwnd, L"Shader/ColorShader.fx"))
	{
		return false;
	}

	return true;
}

bool ColorShaderClass::RenderShader(LPDIRECT3DDEVICE9 device, D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj)
{
	bool result;

	result = SetShaderParameters(world, view, proj);
	m_pColorShader->SetTechnique("Color");

	if (!result)
		return false;

	UINT numPasses = 0;
	m_pColorShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pColorShader->BeginPass(i);
			{
				device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
			}
			m_pColorShader->EndPass();
		}
	}
	m_pColorShader->End();

	return true;
}

void ColorShaderClass::ShutDown()
{
	if (m_pVertexDeclaration)
	{
		m_pVertexDeclaration->Release();
		m_pVertexDeclaration = nullptr;
	}

	if (m_pColorShader)
	{
		m_pColorShader->Release();
		m_pColorShader = nullptr;
	}
}

bool ColorShaderClass::InitializeShader(LPDIRECT3DDEVICE9 device, HWND hwnd, LPCWSTR fileName)
{
	D3DVERTEXELEMENT9 declaration[] =
	{
		{0,0,D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0,12,D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	device->CreateVertexDeclaration(declaration, &m_pVertexDeclaration);

	LPD3DXBUFFER pError = nullptr;
	
	DWORD dwShaderFlags = 0;

	D3DXCreateEffectFromFile(device, L"Shader/ColorShader.fx", NULL, NULL, dwShaderFlags, NULL,
		&m_pColorShader, &pError);


	if (!m_pColorShader && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			TCHAR * str = new TCHAR[size];
		//	_stprintf(str, (TCHAR*)ack, size);
		//	OutputDebugString(str);
			delete[] str;
		}
		
		return false;
	}

	return true;
}

bool ColorShaderClass::SetShaderParameters(D3DXMATRIX * world, D3DXMATRIX * view, D3DXMATRIX * proj)
{
	m_pColorShader->SetMatrix("gWorldMatrix", world);
	m_pColorShader->SetMatrix("gViewMatrix", view);
	m_pColorShader->SetMatrix("gProjectionMatrix", proj);

	return true;
}
