#include "stdafx.h"
#include "InputClass.h"

DEFINITION_SINGLE(InputClass)

InputClass::InputClass():
	m_directInput(nullptr), m_keyboard(nullptr), m_mouse(nullptr), m_tButton1(0.0f)
{
}

InputClass::~InputClass()
{
}

bool InputClass::initialze(HINSTANCE hinstance, HWND hwnd[], 
	int screenWidth, int screenHeight)
{
	HRESULT result;

	m_directXViewHwnd = hwnd[0];

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	result = DirectInput8Create(hinstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void **)&m_directInput,
		NULL);

	if (FAILED(result))
		return false;

	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
		return false;

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	/*result = m_keyboard->SetCooperativeLevel(hwnd[1], DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;
*/
	result = m_keyboard->Acquire();
	if (FAILED(result))
		return false;

	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
		return false;

	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	/*result = m_mouse->SetCooperativeLevel(hwnd[1], DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;
*/
	result = m_mouse->Acquire();
	if (FAILED(result))
		return false;
	
	return true;
}

void InputClass::Shutdown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = NULL;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = NULL;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = NULL;
	}
}

bool InputClass::Update()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
		return false;

	result = ReadMouse();
	if (!result)
		return false;

	ProcessInput();

	return true;
}

bool InputClass::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

void InputClass::GetMouseLocation(int &mouseX, int &mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool InputClass::IsMouseLeftClick()
{
	if (m_mouseState.rgbButtons[0] & 0x80)
		return true;

	return false;
}

bool InputClass::IsMouseRightClick()
{
	if (m_mouseState.rgbButtons[1] & 0x80)
		return true;

	return false;
}

bool InputClass::IsMouseMoved()
{
	if (m_mouseState.lX != 0 && m_mouseState.lY != 0)
		return true;

	return false;
}

bool InputClass::IsWPressed()
{
	if (m_keyboardState[DIK_W] & 0x80)
		return true;

	return false;
}

bool InputClass::IsAPressed()
{
	if (m_keyboardState[DIK_A] & 0x80)
		return true;

	return false;
}

bool InputClass::IsSPressed()
{
	if (m_keyboardState[DIK_S] & 0x80)
		return true;

	return false;
}

bool InputClass::IsDPressed()
{
	if (m_keyboardState[DIK_D] & 0x80)
		return true;

	return false;
}

bool InputClass::IsQPressed()
{
	if (m_keyboardState[DIK_Q] & 0x80)
		return true;

	return false;
}

bool InputClass::IsEPressed()
{
	if (m_keyboardState[DIK_E] & 0x80)
		return true;

	return false;
}

bool InputClass::IsNumber1Pressed()
{
	if (m_keyboardState[DIK_1] & 0x80)
	{
		float nowT = timeGetTime();

		if (((nowT - m_tButton1)/ 1000.0f) >= 0.3)
		{
			m_tButton1 = nowT;
			return true;
		}

	}
	return false;
}

void InputClass::GetMouseAddPos(int &mouseAddX, int &mouseAddY)
{
	mouseAddX = m_mouseX;
	mouseAddY = m_mouseY;
}

void InputClass::GetMouseMove(int &X, int &Y)
{
	X = m_mouseState.lX;
	Y = m_mouseState.lY;
}

//return: 현재 커서와 direct단 윈도우 충돌 여부
bool InputClass::GetMouseWindowPosition(int &x, int &y)
{
	POINT ptPosition;

	GetCursorPos(&ptPosition);
	ScreenToClient(m_directXViewHwnd, &ptPosition);

	x = ptPosition.x;
	y = ptPosition.y;

	if (x < 0 || DIRECT_WND_WIDTH < x) return false;
	if (y < 0 || DIRECT_WND_HEIGHT < y) return false;

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_mouse->Acquire();
		else
			return false;
	}

	return true;
}

void InputClass::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;

	if (m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
	if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;

	return;
}
