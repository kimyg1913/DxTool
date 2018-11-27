#include "stdafx.h"
#include "AplicationClass.h"
#include "D3DClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = NULL;
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


	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
		return false; m_Direct3D;

	result = m_Direct3D->InitD3D(hwnd[0]);
	if (!result)
	{
		MessageBox(hwnd[0], L"Could not initialize Direct3D.", L"Error", MB_OK);

		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = NULL;
	}

	return;
}

bool ApplicationClass::Frame()
{
	bool result = true;

	m_Direct3D->BeginScene(255, 255, 0, 0);

	if (!result)
		return false;


	return true;
}