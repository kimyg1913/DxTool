#include "Core.h"

DX_USING

DEFINITION_SINGLE(Core)

bool Core::m_bLoop = true;

Core::Core() :
	m_pWindow(NULL)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(76057);


#if defined(DEBUG) | defined(_DEBUG)
	AllocConsole();
#endif
}

Core::~Core()
{
	SAFE_RELEASE(m_pWindow);

#if defined(DEBUG) | defined(_DEBUG)
	FreeConsole();
#endif
}

bool Core::Init(HINSTANCE hInst, const wchar_t* pTitle, const wchar_t* pClass,
	int iIconID, RESOLUTION_TYPE eType, bool bWindowMode)
{
	m_pWindow = new CEngineWindow;

	if (!m_pWindow->Init(hInst, pTitle, pClass, iIconID, eType, Core::WndProc))
		return false;

	return Init(m_pWindow->m_hWnd, eType, bWindowMode);
}

bool Core::Init(HWND hWnd, RESOLUTION_TYPE eType, bool bWindowMode)
{
	if (!GET_SINGLE(CDevice)->Init(hWnd, eType, bWindowMode))
		return false;

	srand(time(NULL));

	return true;
}

int Core::Run()
{
	MSG msg;

	// 기본 메시지 루프입니다.
	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

int Core::RunTool()
{
	Logic();

	return 0;
}

void Core::Logic()
{

	Render(fTime);
}

void Core::Input(float fTime)
{
	
}

int Core::Update(float fTime)
{

	return sc;
}

int Core::LateUpdate(float fTime)
{
	

	return 0;
}

void Core::Collision(float fTime)
{
	
}

void Core::Render(float fTime)
{
	GET_SINGLE(CDevice)->Clear();

	//GET_SINGLE(CSceneManager)->Render(fTime);

	
	GET_SINGLE(CDevice)->Present();
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}