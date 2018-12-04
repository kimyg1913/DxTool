#pragma once
#pragma once

#include <atlstr.h>
#include <vector>
#include <atlconv.h>
#include <fstream>
#include <iostream>
#include <string>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define TERRAINFVF (D3DFVF_XYZW | D3DFVF_TEX2)

#define MENU_X_SIZE 256;

#define MAIN_WND_WIDTH		1920
#define MAIN_WND_HEIGHT		1600

#define DIRECT_WND_WIDTH	1600
#define DIRECT_WND_HEIGHT	1024

#define PI				3.14159265f
#define FOV				(PI/4.0f)
#define ASPECT_RATIO	(float)(DIRECT_WND_WIDTH/DIRECT_WND_HEIGHT)
#define NEAR_PLANE		1
#define FAL_PLANE		10000

using namespace std;

namespace MyStruct
{
	struct TERRAINVERTEX
	{
		D3DXVECTOR4 point;
		D3DXVECTOR2 texture;
	};

	struct MYINDEX
	{
		WORD i_0, i_1, i_2;
	};

	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;    // from the D3DFVF_XYZRHW flag
		DWORD color;    // from the D3DFVF_DIFFUSE flag
	};

}