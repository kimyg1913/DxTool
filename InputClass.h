#pragma once

#define DIRECTINPUT_VERTSION 0x0800

#include <dinput.h>
#include "Base.h"
#include "TemplateSingleton.h"

class InputClass : public TemplateSingleton<InputClass>
{
public:
	InputClass();
	~InputClass();

	bool initialze(HINSTANCE, HWND[], int, int);
	void Shutdown();
	bool Update();

	bool IsEscapePressed();
	void GetMouseMove(int &X, int &Y);
	void GetMouseLocation(int &, int &);
	void GetMouseAddPos(int &, int &);
	bool GetMouseWindowPosition(int &, int &);

	bool IsMouseLeftClick();
	bool IsMouseRightClick();
	bool IsMouseMoved();
	bool IsWPressed();
	bool IsAPressed();
	bool IsSPressed();
	bool IsDPressed();
	bool IsQPressed();
	bool IsEPressed();
	bool IsFPressed();
	bool IsRPressed();
	bool IsNumber1Pressed();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8 *m_directInput;
	IDirectInputDevice8 *m_keyboard;
	IDirectInputDevice8 *m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	float m_tButton1;
	float m_tPrevButton1;

	HWND m_directXViewHwnd;
};

