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
#include "BrushClass.h"
#include <assert.h>
#include <iostream>

#ifdef  _DEBUG
//#pragma comment( linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif //  


ApplicationClass::ApplicationClass() :
	m_pInput(nullptr), m_pCamera(nullptr), m_pDirect3D(nullptr), m_pFont(nullptr), m_vPicked(987654321, 987654321, 987654321)
{

}

ApplicationClass::ApplicationClass(const ApplicationClass &other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd[], int screenWidth, int screenHeight)
{
	srand((unsigned)time(0));

	bool result;

	m_pInput = InputClass::GetInstance();

	result = m_pInput->initialze(hinstance, hwnd, screenWidth, screenWidth);

	if (!result)
	{
		MessageBox(hwnd[0], L"Could not initialize InputClass.", L"Error", MB_OK);
		return false;
	}

	m_pCamera = Camera::GetInstance();

	result = m_pCamera->Initialize();

	if (!result)
	{
		MessageBox(hwnd[0], L"Could not initialize Camera.", L"Error", MB_OK);
		return false;
	}

	m_pDirect3D = D3DClass::GetInstance();

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
	rc.bottom = DIRECT_WND_HEIGHT / 3;

	m_pFont->Initialize(m_pDirect3D->GetDevice(), wstring(), rc, D3DCOLOR_ARGB(255, 255, 255, 255));

	BrushClass::GetInstance()->Initialize(0);

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

	BrushClass::DestroyInstance();

	D3DClass::DestroyInstance();
	m_pDirect3D = nullptr;

	Camera::DestroyInstance();
	m_pCamera = nullptr;

	InputClass::DestroyInstance();
	m_pInput = nullptr;

	Time::DestroyInstance();

	ErosionClass::DestroyInstance();

	TerrainClass::DestroyInstance();
}

bool ApplicationClass::Update()
{

	Time::GetInstance()->update();

	bool result = true;

	if (m_pInput->IsEscapePressed())
	{
		return false;
	}

	
	int iFps = Time::GetInstance()->getFps();
	wstring strFps = to_wstring(iFps);
	wstring str(L"FPS : ");
	str += strFps;

	m_pFont->SetStr(str);
	//std::wcout << str << std::endl;

	if (m_pInput->Update())
	{
		m_pCamera->Update(m_pInput);

		if (m_pInput->IsNumber1Pressed())
			m_pDirect3D->ToggleWireFrame();
		if (m_pInput->IsFPressed())
			m_pCamera->MoveToStart();

		int temp, temp1;

		if (TerrainClass::GetInstance()->IsCreated())
		{
			if (m_pInput->IsRPressed())
				ErosionClass::GetInstance()->ToggleIsRain();

			ErosionClass::GetInstance()->Update(Time::GetInstance()->getTick(), TerrainClass::GetInstance()->GetVertexBuffer());

			if (m_pInput->GetMouseWindowPosition(temp, temp1))
			{
				if (TerrainClass::GetInstance()->Picking(m_pDirect3D->GetDevice(), m_pDirect3D->GetHWND(), &m_pDirect3D->GetWorldMatrix(), m_pCamera->GetViewMatrix(), &m_pDirect3D->GetWorldProjection(), &m_vPicked, m_isBrushOrErosion))
				{
					if (m_pInput->IsMouseLeftClick() && m_isBrushOrErosion == BE_BRUSH)
					{
						TerrainClass::GetInstance()->ModifyYValue(m_vPicked);
					}
					else if (m_pInput->IsMouseLeftClick() && m_isBrushOrErosion == BE_EROSION)
					{
						ErosionClass::GetInstance()->AddRainDrop(m_vPicked);
					}

					//BrushClass::GetInstance()->UpdateTransform(m_vPicked);

				}
			}
		}
	}

	m_pDirect3D->RenderBegin(m_pFont);

	return true;
}

bool ApplicationClass::InitTerrain(int xN, int zN, int xS, int zS, bool isLoadMap, LPCWSTR str)
{
	if (TerrainClass::GetInstance()->IsCreated())
		return true;

	
	assert(((xN % 2 == 1 && zN % 2 == 1) || isLoadMap) && "지형사이즈는 2^n + 1이 돼야합니다.");
	if (TerrainClass::GetInstance()->Initialize(m_pDirect3D->GetDevice(), xN, zN, xS, zS, isLoadMap, str, 0.03f))
		return true;

	return false;
}

bool ApplicationClass::LoadHeightMap(LPCWSTR str)
{
	if(TerrainClass::GetInstance()->IsCreated())
		TerrainClass::DestroyInstance();

	TerrainClass::GetInstance()->Initialize(D3DClass::GetInstance()->GetDevice(), 0, 0, 0, 0, 1, str, 0.03);

	return true;
}

void ApplicationClass::SetBrush(int radius, float strength)
{
	if (TerrainClass::GetInstance()->IsCreated())
	{
		TerrainClass::GetInstance()->SetBrush(radius, strength);
	}
}

void ApplicationClass::SetDrawMode(DRAWMODE mode)
{
	if (TerrainClass::GetInstance()->IsCreated())
	{
		TerrainClass::GetInstance()->SetDrawMode(mode);
	}
}

void ApplicationClass::SetBrushOrErosion(BRUSHOREROSION brMode)
{
	m_isBrushOrErosion = brMode;
}

void ApplicationClass::SetRainRadius(float radius)
{
	if (TerrainClass::GetInstance()->IsCreated())
		ErosionClass::GetInstance()->SetRainRadius(radius);
}

void ApplicationClass::SetRainAmount(float amount)
{
	if (TerrainClass::GetInstance()->IsCreated())
		ErosionClass::GetInstance()->SetRainAmount(amount);
}

void ApplicationClass::ClearTerrain()
{
	TerrainClass::DestroyInstance();
}

