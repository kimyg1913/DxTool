#include "stdafx.h"
#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "Camera.h"
#include "TerrainClass.h"
#include "BrushClass.h"
#include "ErosionClass.h"
#include "FontClass.h"
#include "Time.h"

D3DClass::D3DClass() :
	m_bNowWireFrame(false)
{
	
}


D3DClass::~D3DClass()
{
	
}

bool D3DClass::Initialize(HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX * matView, bool fullscreen)
{
	if (FAILED(InitD3D(hwnd, screenWidth, screenHeight, matView)))
	{
		return false;
	}

	m_hWnd = hwnd;

	return true;
}

HRESULT D3DClass::InitD3D(HWND hWnd, int screenWidth, int screenHeight, D3DXMATRIX * matView)
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
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// 
	d3dpp.hDeviceWindow = hWnd;


	/// 디바이스 생성
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice)))
	{
		return E_FAIL;
	}

	D3DXMatrixIdentity(&m_matWorld);
	m_matView = *matView;

	D3DXMatrixPerspectiveFovLH(&m_matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAL_PLANE);


	return TRUE;
}

bool D3DClass::InitTerrain(LPDIRECT3DDEVICE9 pDevice, int xNumber, int zNumber, int xSize, int zSize, bool isLoadMap, LPCWSTR str)
{

	return true;
}

void D3DClass::Shutdown()
{
	m_pd3dDevice->Release();    // close and release the 3D device
	m_pD3D->Release();
}

void D3DClass::RenderBegin(FontClass * pFont)
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	m_matView = *Camera::GetInstance()->GetViewMatrix();

	m_pd3dDevice->BeginScene();

	// 실제 렌더링 하는 부분
	RenderScene(0, 0, 255, 255);
	
//	pFont->Update();

	m_pd3dDevice->EndScene();

	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void D3DClass::RenderEnd()
{

}

void D3DClass::ToggleWireFrame()
{
	m_bNowWireFrame = !m_bNowWireFrame;
}

void D3DClass::RenderScene(int r, int g, int b, int a)
{
	if (m_bNowWireFrame)
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	LPDIRECT3DTEXTURE9 DepthTex = nullptr;

	if (TerrainClass::GetInstance()->IsCreated())
	{
		TerrainClass::GetInstance()->Render(m_pd3dDevice, &m_matWorld, &m_matView, &m_matProjection);
	//	DepthTex = TerrainClass::GetInstance()->GetShdowTex();

	//	if (BrushClass::GetInstance()->GetIsCreated())
		//	BrushClass::GetInstance()->Render(m_pd3dDevice, DepthTex);

	}
	
}

