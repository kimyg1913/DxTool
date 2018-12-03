#include "stdafx.h"
#include "D3DClass.h"

D3DClass::D3DClass() :
	m_pCamera(nullptr),
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

	if(m_pCamera)
	{
		m_pCamera->ShutDown();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

bool D3DClass::Initialize(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen)
{
	if (FAILED(InitD3D(hwnd, screenWidth, screenHeight)))
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

HRESULT D3DClass::InitD3D(HWND hWnd, int screenWidth, int screenHeight)
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

	/// 기본컬링, CCW
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	/// Z버퍼기능을 켠다.
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


	D3DXMatrixIdentity(&m_matWorld);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	
	m_pCamera = new Camera;
	if (!m_pCamera->Initialize())
	{
		delete m_pCamera;
		m_pCamera = nullptr;
		return false;
	}

	D3DXMatrixLookAtLH(&m_matView, m_pCamera->GetEye(), m_pCamera->GetLookat(), m_pCamera->GetUp());
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAL_PLANE);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);

	m_pd3dDevice->CreateVertexBuffer(3 * sizeof(MyStruct::CUSTOMVERTEX), 0,
	CUSTOMFVF, D3DPOOL_DEFAULT, &m_pVB, NULL);

	void* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	RECT rc;

	GetClientRect(hWnd,&rc);

	rc.right -= MENU_X_SIZE;

	int xSize = 400;
	int ySize = 400;

	MyStruct::CUSTOMVERTEX v[] =
	{
		{rc.right/2, rc.bottom/2 - ySize/2, 1.0f,  D3DCOLOR_XRGB(0, 0, 255)},					 //화면 중앙 위 버텍스
		{rc.right/2 + xSize/2, rc.bottom /2 + ySize/2, 1.0f, D3DCOLOR_XRGB(0, 255, 0)},				//화면 오른쪽아래 버텍스
		{rc.right/2 - xSize/2, rc.bottom /2 + ySize/2, 1.0f, D3DCOLOR_XRGB(255, 0, 0)},				//화면 왼쪽아래 버텍스
	};

	memcpy(pVertices, v, sizeof(v));

	m_pVB->Unlock();


	return TRUE;
}

void D3DClass::Shutdown()
{
	m_pVB->Release();
	m_pd3dDevice->Release();    // close and release the 3D device
	m_pD3D->Release();
}

void D3DClass::RenderBegin()
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	m_pd3dDevice->BeginScene();

	// 실제 렌더링 하는 부분
	{
		RenderScene(255, 255, 255, 255);
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
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pd3dDevice->SetFVF(CUSTOMFVF);
	// select the vertex buffer to display
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(MyStruct::CUSTOMVERTEX));

	m_pColorShader->RenderShader(m_pd3dDevice, m_matWorld, m_matView, m_matProjection);

	// copy the vertex buffer to the back buffer
	//m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	
}


