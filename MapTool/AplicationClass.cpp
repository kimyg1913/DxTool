#include "stdafx.h"
#include "AplicationClass.h"
#include "InputClass.h"
#include "Camera.h"
#include "D3DClass.h"
#include "FpsClass.h"
#include "Time.h"
#include "FontClass.h"

ApplicationClass::ApplicationClass() :
	m_pInput(nullptr), m_pCamera(nullptr),m_pDirect3D(nullptr), m_pFps(nullptr), m_pFont(nullptr)
{
}

ApplicationClass::ApplicationClass(const ApplicationClass &other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd[],	int screenWidth, int screenHeight)
{
	srand((unsigned)time(0));

	bool result;

	m_pInput = new InputClass;

	result = m_pInput->initialze(hinstance, hwnd, screenWidth, screenWidth);

	if (!result)
	{
		MessageBox(hwnd[0], L"Could not initialize InputClass.", L"Error", MB_OK);
		return false;
	}

	m_pFps = new FpsClass;

	//fpsClass initialize
	m_pFps->Initialize();

	m_pCamera = Camera::GetInstance();

	result = m_pCamera->Initialize();
	
	if(!result)
	{
		MessageBox(hwnd[0], L"Could not initialize Camera.", L"Error", MB_OK);
		return false;
	}

	m_pDirect3D = new D3DClass;
	
	result = m_pDirect3D->Initialize(hwnd[0], screenWidth, screenHeight, m_pCamera->GetViewMatrix());

	if (!result)
	{
		MessageBox(hwnd[0], L"Could not initialize Direct3D.", L"Error", MB_OK);

		return false;
	}


	m_pFont = new FontClass;

	RECT rc;
	rc.left = 5;
	rc.top = 5;
	rc.right = DIRECT_WND_WIDTH / 3;
	rc.bottom = DIRECT_WND_HEIGHT/3;

	m_pFont->Initialize(m_pDirect3D->GetDevice(), wstring(), rc, D3DCOLOR_ARGB(255, 255, 255, 255));

	return true;
}

void ApplicationClass::Shutdown()
{
	if (m_pFont)
	{
		m_pFont->Shutdown();
		delete m_pFont;
		m_pFont = nullptr;
	}

	if (m_pFps)
	{
		delete m_pFps;
		m_pFps = NULL;
	}

	if (m_pDirect3D)
	{
		m_pDirect3D->Shutdown();
		delete m_pDirect3D;
		m_pDirect3D = nullptr;
	}

	//if (m_pCamera)
	//{
	//	m_pCamera->ShutDown();
	//	delete m_pCamera;
	//	m_pCamera = nullptr;
	//}

	if (m_pInput)
	{
		m_pInput->Shutdown();
		delete m_pInput;
		m_pInput = nullptr;
	}

}

bool ApplicationClass::Update()
{

	Time::getInstance()->update();

	bool result = true;

	if (m_pInput->IsEscapePressed())
	{
		return false;
	}

	m_pFps->Update();

	int iFps = m_pFps->GetFps();
	wstring strFps = to_wstring(iFps);
	wstring str(L"FPS : ");
	str += strFps;

	m_pFont->SetStr(str);
	

	if (m_pInput->Update())
	{
		m_pCamera->Update(m_pInput);

		if (m_pInput->IsNumber1Pressed())
			m_pDirect3D->ToggleWireFrame();

		int temp, temp1;

		if (m_pInput->IsMouseLeftClick() && m_pInput->GetMouseWindowPosition(temp, temp1))
		{
			m_pDirect3D->Picking();
		}
	}

	m_pDirect3D->RenderBegin(m_pCamera, m_pFont);


	if (!result)
		return false;

	m_pDirect3D->RenderEnd();

	return true;
}

bool ApplicationClass::InitTerrain(int xN, int zN, int xS, int zS, bool isLoadMap, LPCWSTR str)
{
	if(m_pDirect3D->InitTerrain(m_pDirect3D->GetDevice(), xN, zN, xS, zS, isLoadMap, str))
		return true;

	return false;
}

bool ApplicationClass::LoadHeightMap(LPCWSTR str)
{
	if (m_pDirect3D->LoadHeightMap(str))
	{
		return true;
	}

	return false;
}

void ApplicationClass::SetBrush(int radius, float strength)
{
	if (m_pDirect3D)
	{
		m_pDirect3D->SetBrush(radius, strength);
	}
}

void ApplicationClass::SetDrawMode(DRAWMODE mode)
{
	if (m_pDirect3D)
	{
		m_pDirect3D->SetDrawMode(mode);
	}
}

