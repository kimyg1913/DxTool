#pragma once

#include <atlstr.h>
#include <vector>
#include <list>
#include <set>
#include <atlconv.h>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <queue>
#include <string>
#include <math.h>
#include <mmsystem.h>  //timegettime ÇÔ¼ö

//#include "MyMath.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )


#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define TERRAINFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)

#define MENU_X_SIZE 256

#define MAIN_WND_WIDTH		1600
#define MAIN_WND_HEIGHT		800

#define DIRECT_WND_WIDTH	1024
#define DIRECT_WND_HEIGHT	735

#define PI				3.14159265f
#define FOV				(PI/4.0f)
#define ASPECT_RATIO	(float)(DIRECT_WND_WIDTH/DIRECT_WND_HEIGHT)
#define NEAR_PLANE		(float)0.1
#define FAL_PLANE		1000

enum DRAWMODE
{
	DM_LIFT = 0,
	DM_FALL
};

using namespace std;

namespace MyStruct
{
	/*struct TERRAINVERTEX
	{
		D3DXVECTOR3 point;
		D3DXVECTOR2 texture;
	};
*/
	struct TERRAINVERTEX
	{
		D3DXVECTOR3 point;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texture;
	};

	struct MYINDEX
	{
		DWORD i_0, i_1, i_2;
	};

	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;    // from the D3DFVF_XYZRHW flag
		DWORD color;    // from the D3DFVF_DIFFUSE flag
	};

}


//namespace MyFuc
//{
//	float VectorLength(D3DXVECTOR3 t)
//	{
//		return sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
//	};
//
//	D3DXVECTOR3 VectorNormalize(D3DXVECTOR3 t)
//	{
//		float length = VectorLength(t);
//
//		return t / length;
//	};
//}