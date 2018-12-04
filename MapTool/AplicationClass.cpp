#include "stdafx.h"
#include "AplicationClass.h"
#include "InputClass.h"
#include "Camera.h"
#include "D3DClass.h"


ApplicationClass::ApplicationClass() :
	m_pInput(nullptr), m_pCamera(nullptr),m_pDirect3D(nullptr)
{
}

ApplicationClass::ApplicationClass(const ApplicationClass &other)
{
}

ApplicationClass::~ApplicationClass()
{
}

// hwnd[0] = DirectXView->Hwnd, hwnd[1] = MainFrm->Hwnd
bool ApplicationClass::Initialize(HINSTANCE hinstance,
	HWND hwnd[],
	int screenWidth,
	int screenHeight)
{
	bool result;

	m_pInput = new InputClass;

	result = m_pInput->initialze(hinstance, hwnd, screenWidth, screenWidth);

	if (!result)
	{
		MessageBox(hwnd[0], L"Could not initialize InputClass.", L"Error", MB_OK);
		return false;
	}

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

bool ApplicationClass::InitTerrain(int xN, int zN, int xS, int zS)
{
	if(m_pDirect3D->InitTerrain(m_pDirect3D->GetDevice(), xN, zN, xS, zS))
		return true;

	return false;
}

