#include "stdafx.h"
#include "D3DClass.h"


D3DClass::D3DClass()
{
}


D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	return true;
}

HRESULT D3DClass::InitD3D(HWND hWnd)
{
	// ����̽��� �����ϱ����� D3D��ü ����
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// ����̽��� ������ ����ü
	// ������ ������Ʈ�� �׸����̱⶧����, �̹����� Z���۰� �ʿ��ϴ�.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	/// ����̽� ����
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	/// �⺻�ø�, CCW
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	/// Z���۱���� �Ҵ�.
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


	g_pd3dDevice->CreateVertexBuffer(3 * sizeof(MyStruct::CUSTOMVERTEX), 0,
		CUSTOMFVF, D3DPOOL_DEFAULT, &g_pVB, NULL);

	void* pVertices = nullptr;
	g_pVB->Lock(0, 0, (void**)&pVertices, 0);

	RECT rc;

	GetClientRect(hWnd,&rc);

	rc.right -= MENU_X_SIZE;

	int xSize = 400;
	int ySize = 400;

	MyStruct::CUSTOMVERTEX v[] =
	{
		{rc.right/2, rc.bottom/2 - ySize/2, 1.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255),},						 //ȭ�� �߾� �� ���ؽ�
		{rc.right/2 + xSize/2, rc.bottom /2 + ySize/2, 1.0f, 1.0f, D3DCOLOR_XRGB(0, 255, 0),},				//ȭ�� �����ʾƷ� ���ؽ�
		{rc.right/2 - xSize/2, rc.bottom /2 + ySize/2, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0),},				//ȭ�� ���ʾƷ� ���ؽ�
	};

	memcpy(pVertices, v, sizeof(v));

	g_pVB->Unlock();


	return TRUE;
}

void D3DClass::Shutdown()
{
	g_pVB->Release();
	g_pd3dDevice->Release();    // close and release the 3D device
	g_pD3D->Release();
}

void D3DClass::BeginScene(int r, int g, int b, int a)
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	g_pd3dDevice->BeginScene();
	g_pd3dDevice->SetFVF(CUSTOMFVF);

	// select the vertex buffer to display
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(MyStruct::CUSTOMVERTEX));

	// copy the vertex buffer to the back buffer
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void D3DClass::EndScene()
{
}

