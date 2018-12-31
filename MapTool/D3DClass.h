#pragma once
#include "Base.h"

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass &);
	~D3DClass();

	bool Initialize(HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX * matView, bool fullscreen = false);
	HRESULT InitD3D(HWND hWnd, int screenWidth, int screenHeight, D3DXMATRIX * matView);
	bool InitTerrain(LPDIRECT3DDEVICE9 pDevice, int xNumber, int zNumber, int xSize, int zSize, bool isLoadMap, LPCWSTR str);
	bool LoadHeightMap(LPCWSTR fileName);

	void Shutdown();

	LPDIRECT3DDEVICE9	GetDevice() { return m_pd3dDevice; }
	void RenderBegin(class Camera * pCamera);
	void RenderEnd();

	void ToggleWireFrame();

	void Picking();

	void SetBrush(int radius, float strength);

private:
	void RenderScene(int r, int g, int b, int a);

	bool NowIsWireFrame() { return m_bNowWireFrame; }

private:
	LPDIRECT3D9             m_pD3D = NULL; // D3D 디바이스를 생성할 D3D객체변수
	LPDIRECT3DDEVICE9       m_pd3dDevice = NULL; // 렌더링에 사용될 D3D디바이스

	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProjection;
	
	HWND					m_hWnd;

	class TerrainClass *	m_pTerrain;
	class ColorShaderClass * m_pColorShader;

	float mRotationY = 0.0f;
	bool m_bNowWireFrame;
};