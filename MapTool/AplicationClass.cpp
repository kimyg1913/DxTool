#include "stdafx.h"
#include "AplicationClass.h"
#include "InputClass.h"
#include "Camera.h"
#include "D3DClass.h"
#include "FpsClass.h"

ApplicationClass::ApplicationClass() :
	m_pInput(nullptr), m_pCamera(nullptr),m_pDirect3D(nullptr), m_pFps(nullptr)
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

	m_pCamera = new Camera;

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

	return true;
}

void ApplicationClass::Shutdown()
{

	if (m_pFps)
	{
		delete m_pFps;
		m_pFps = NULL;
	}

	if (m_pDirect3D)
	{
		m_pDirect3D->Shutdown();
		delete m_pDirect3D;
		m_pDirect3D = NULL;
	}

	if (m_pCamera)
	{
		m_pCamera->ShutDown();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pInput)
	{
		m_pInput->Shutdown();
		delete m_pInput;
		m_pInput = nullptr;
	}

}

bool ApplicationClass::Frame()
{
	bool result = true;

	if (m_pInput->IsEscapePressed())
	{
		return false;
	}

	m_pFps->Frame();

	printf("%d \n", m_pFps->GetFps());

	if (m_pInput->Frame())
	{
		m_pCamera->Frame(m_pInput);
	}

	m_pDirect3D->RenderBegin(m_pCamera);

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

