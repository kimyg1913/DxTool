#pragma once

#include "D3D_Core.h"

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
	bool Frame();

private:
	class	D3DClass *m_Direct3D;

	MyStruct::TERRAIN_DESC terrainDesc;
};