#include "stdafx.h"
#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "Camera.h"
#include "TerrainClass.h"

D3DClass::D3DClass() :
	m_pTerrain(nullptr),
	m_pColorShader(nullptr)
{
	
}


D3DClass::~D3DClass()
{
	if (m_pColorShader)
	{
		m_pColorShader->ShutDown();
		delete m_pColorShader;
		m_pColorShader = nullptr;
	}

	if (m_pTerrain)
	{
		m_pTerrain->ShutDown();
		delete m_pTerrain;
		m_pTerrain = nullptr;
	}
}

bool D3DClass::Initialize(HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIXA16 * matView, bool fullscreen)
{
	if (FAILED(InitD3D(hwnd, screenWidth, screenHeight, matView)))
	{
		return false;
	}

	m_pColorShader = new ColorShaderClass;

	if (!m_pColorShader->Initialize(m_pd3dDevice, hwnd))
	{
		m_pColorShader->ShutDown();
		delete m_pColorShader;
		m_pColorShader = nullptr;
		return false;
	}

	return true;
}

HRESULT D3DClass::InitD3D(HWND hWnd, int screenWidth, int screenHeight, D3DXMATRIXA16 * matView)
{
	// 디바이스를 생성하기위한 D3D객체 생성
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// 디바이스를 생성할 구조체
	// 복잡한 오브젝트를 그릴것이기때문에, 이번에는 Z버퍼가 필요하다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth = screenWidth;
	d3dpp.BackBufferHeight = screenHeight;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


	/// 디바이스 생성
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice)))
	{
		return E_FAIL;
	}

	D3DXMatrixIdentity(&m_matWorld);
	//m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	
	m_matView = *matView;

	//m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAL_PLANE);
	//m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);


	return TRUE;
}

bool D3DClass::InitTerrain(LPDIRECT3DDEVICE9 pDevice, int xNumber, int zNumber, int xSize, int zSize, bool isLoadMap, LPCWSTR str)
{
	if (m_pTerrain)
	{
		m_pTerrain->ShutDown();
		delete m_pTerrain;
		m_pTerrain = nullptr;
	}

	m_pTerrain = new TerrainClass;

	if (!m_pTerrain->Initialize(m_pd3dDevice, xNumber, zNumber,xSize,zSize, isLoadMap, str))
	{
		m_pTerrain->ShutDown();
		delete m_pTerrain;
		m_pTerrain = nullptr;
		return false;
	}

	return true;
}

bool D3DClass::LoadHeightMap(LPCWSTR fileName)
{
	if (m_pTerrain)
	{
		m_pTerrain->ShutDown();
		delete m_pTerrain;
		m_pTerrain = nullptr;
	}

	m_pTerrain = new TerrainClass;

	if (m_pTerrain->LoadHeightMap(m_pd3dDevice,fileName))
	{
		return false;
	}

	return true;
}

void D3DClass::Shutdown()
{
	m_pd3dDevice->Release();    // close and release the 3D device
	m_pD3D->Release();
}

void D3DClass::RenderBegin(Camera * pCamera)
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	m_matView = *pCamera->GetViewMatrix();

	m_pd3dDevice->BeginScene();

	// 실제 렌더링 하는 부분
	{
		RenderScene(0, 0, 255, 255);
	}

	m_pd3dDevice->EndScene();

	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	
	RenderEnd();
}

void D3DClass::RenderEnd()
{
}


void D3DClass::RenderScene(int r, int g, int b, int a)
{

	if(m_pTerrain)
		m_pTerrain->Render(m_pd3dDevice, &m_matWorld, &m_matView, &m_matProjection);

	// copy the vertex buffer to the back buffer
	
}


