#pragma once

#include "Base.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass &);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND[], int, int);
	void Shutdown();
	bool Update();
	bool InitTerrain(int xN, int zN, int xS, int zS, bool isLoadMap, LPCWSTR str);
	bool LoadHeightMap(LPCWSTR str);
	void SetBrush(int radius, float strength);

private:
	class   InputClass *	m_pInput;
	class   Camera *		m_pCamera;
	class	D3DClass *		m_pDirect3D;
	class   FpsClass *		m_pFps;
	
};