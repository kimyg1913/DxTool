#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace std;
using namespace DirectX;

#include "DxMacro.h"

#define DX_BEGIN	namespace DxToolEngine {
#define DX_END		}
#define DX_USING	using namespace DxToolEngine;

#ifdef DXENGINE_EXPORT
#define DX_DLL __declspec(dllexport)
#else
#define DX_DLL __declspec(dllimport)
#endif