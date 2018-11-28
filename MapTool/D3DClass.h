#pragma once

#include "Base.h"

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass &);
	~D3DClass();

	bool D3DClass::Initialize(int screenWidth,
		int screenHeight,
		bool vsync,
		HWND hwnd,
		bool fullscreen,
		float screenDepth,
		float screenNear);
	HRESULT InitD3D(HWND hWnd);
	void Shutdown();

	void BeginScene(int r, int g, int b, int a);
	void EndScene();

	LPDIRECT3D9             g_pD3D = NULL; // D3D 디바이스를 생성할 D3D객체변수
	LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // 렌더링에 사용될 D3D디바이스
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9	g_pIB = NULL;
private:

};