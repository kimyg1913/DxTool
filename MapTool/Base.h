#pragma once
#pragma once

#include <atlstr.h>
#include <vector>
#include <atlconv.h>
#include <fstream>
#include <iostream>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define MENU_X_SIZE 256;


using namespace std;

namespace MyStruct
{
	struct TERRAIN_DESC
	{
		int nCell;
		int nTile;
		int nTexture;

		char *baseTextureName;
		std::vector<std::string> *textureNames;

		int textureCurSel;	// Cursor Select
	};

	struct CUSTOMVERTEX
	{
		FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
		DWORD color;    // from the D3DFVF_DIFFUSE flag
	};
}