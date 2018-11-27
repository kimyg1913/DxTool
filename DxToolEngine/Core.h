#pragma once

#include "ToolEngine.h"

DX_BEGIN

class DX_DLL Core
{
private:
	class CEngineWindow*	m_pWindow;
	static bool				m_bLoop;

public:
	bool Init(HINSTANCE hInst, const wchar_t* pTitle, const wchar_t* pClass,
		int iIconID, RESOLUTION_TYPE eType, bool bWindowMode);
	bool Init(HWND hWnd, RESOLUTION_TYPE eType, bool bWindowMode);
	int Run();
	int RunTool();

private:
	void Logic();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	DECLARE_SINGLE(Core)
};

DX_END