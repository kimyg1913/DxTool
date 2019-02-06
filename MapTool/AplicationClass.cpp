#include "stdafx.h"
#include "AplicationClass.h"
#include "InputClass.h"
#include "Camera.h"
#include "D3DClass.h"
#include "FpsClass.h"
#include "Time.h"
#include "FontClass.h"
#include "TerrainClass.h"
#include "PerlinClass.h"
#include "ErosionClass.h"

#include <iostream>

#pragma comment( linker, "/entry:WinMainCRTStartup /subsystem:console")


ApplicationClass::ApplicationClass() :
	m_pInput(nullptr), m_pCamera(nullptr),m_pDirect3D(nullptr), m_pFps(nullptr), m_pFont(nullptr), m_vPicked(987654321,987654321,987654321)
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

	m_pInput = GET_SINGLE(InputClass);

	result = m_pInput->initialze(hinstance, hwnd, screenWidth, screenWidth);

	if (!result)
	{
		MessageBox(hwnd[0], L"Could not initialize InputClass.", L"Error", MB_OK);
		return false;
	}

	m_pFps = new FpsClass;

	//fpsClass initialize
	m_pFps->Initialize();

	m_pCamera = GET_SINGLE(Camera);

	result = m_pCamera->Initialize();
	
	if(!result)
	{
		MessageBox(hwnd[0], L"Could not initialize Camera.", L"Error", MB_OK);
		return false;
	}

	m_pDirect3D = GET_SINGLE(D3DClass);
	
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

	DESTROY_SINGLE(D3DClass);
	m_pDirect3D = nullptr;

	DESTROY_SINGLE(Camera);
	m_pCamera = nullptr;

	DESTROY_SINGLE(InputClass);
	m_pInput = nullptr;

	DESTROY_SINGLE(Time);

	DESTROY_SINGLE(ErosionClass);

	DESTROY_SINGLE(TerrainClass);
}

bool ApplicationClass::Update()
{

	GET_SINGLE(Time)->update();

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
	std::wcout << str << std::endl;

	if (m_pInput->Update())
	{
		m_pCamera->Update(m_pInput);

		if (m_pInput->IsNumber1Pressed())
			m_pDirect3D->ToggleWireFrame();

		int temp, temp1;

		if (GET_SINGLE(TerrainClass)->IsCreated())
		{
			GET_SINGLE(ErosionClass)->Update(GET_SINGLE(Time)->getTick() * 50, GET_SINGLE(TerrainClass)->GetVertexBuffer());
			
			if (m_pInput->GetMouseWindowPosition(temp, temp1))
			{
				if (GET_SINGLE(TerrainClass)->Picking(m_pDirect3D->GetDevice(), m_pDirect3D->GetHWND(), &m_pDirect3D->GetWorldMatrix(), m_pCamera->GetViewMatrix(), &m_pDirect3D->GetWorldProjection(), &m_vPicked, m_isBrushOrErosion))
				{
					if (m_pInput->IsMouseLeftClick() && m_isBrushOrErosion == BE_BRUSH)
					{
						GET_SINGLE(TerrainClass)->ModifyYValue(m_vPicked);
					}
					else if (m_pInput->IsMouseLeftClick() && m_isBrushOrErosion == BE_EROSION)
					{
						GET_SINGLE(ErosionClass)->AddRainDrop(m_vPicked);
					}
				}
			}
		}
	}

	m_pDirect3D->RenderBegin(m_pFont);

	if (!result)
		return false;

	m_pDirect3D->RenderEnd();

	return true;
}

bool ApplicationClass::InitTerrain(int xN, int zN, int xS, int zS, bool isLoadMap, LPCWSTR str)
{
	if (GET_SINGLE(TerrainClass)->IsCreated())
		return true;

	if(GET_SINGLE(TerrainClass)->Initialize(m_pDirect3D->GetDevice(), xN, zN, xS, zS, isLoadMap, str))
		return true;

	return false;
}

bool ApplicationClass::LoadHeightMap(LPCWSTR str)
{
	

	return false;
}

void ApplicationClass::SetBrush(int radius, float strength)
{
	if (GET_SINGLE(TerrainClass)->IsCreated())
	{
		GET_SINGLE(TerrainClass)->SetBrush(radius, strength);
	}
}

void ApplicationClass::SetDrawMode(DRAWMODE mode)
{
	if (GET_SINGLE(TerrainClass)->IsCreated())
	{
		GET_SINGLE(TerrainClass)->SetDrawMode(mode);
	}
}

void ApplicationClass::SetBrushOrErosion(BRUSHOREROSION brMode)
{
	m_isBrushOrErosion = brMode;
}

void ApplicationClass::SetRainRadius(float radius)
{
	if(GET_SINGLE(TerrainClass)->IsCreated())
		GET_SINGLE(ErosionClass)->SetRainRadius(radius);
}

void ApplicationClass::SetRainAmount(float amount)
{
	if (GET_SINGLE(TerrainClass)->IsCreated())
		GET_SINGLE(ErosionClass)->SetRainAmount(amount);
}

void ApplicationClass::ClearTerrain()
{
	DESTROY_SINGLE(TerrainClass);
}

