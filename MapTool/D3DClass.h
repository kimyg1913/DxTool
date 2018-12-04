#pragma once
#include "Base.h"

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass &);
	~D3DClass();

	bool Initialize(HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIXA16 * matView, bool fullscreen = false);
	HRESULT InitD3D(HWND hWnd, int screenWidth, int screenHeight, D3DXMATRIXA16 * matView);
	bool InitTerrain(LPDIRECT3DDEVICE9 pDevice, int xNumber, int zNumber, int xSize, int zSize);
	
	void Shutdown();

	LPDIRECT3DDEVICE9	GetDevice() { return m_pd3dDevice; }
	void RenderBegin(class Camera * pCamera);
	void RenderEnd();
private:
	void RenderScene(int r, int g, int b, int a);

private:
	LPDIRECT3D9             m_pD3D = NULL; // D3D 디바이스를 생성할 D3D객체변수
	LPDIRECT3DDEVICE9       m_pd3dDevice = NULL; // 렌더링에 사용될 D3D디바이스

	D3DXMATRIXA16			m_matWorld;
	D3DXMATRIXA16			m_matView;
	D3DXMATRIXA16			m_matProjection;
	
	class TerrainClass *	m_pTerrain;
	class ColorShaderClass * m_pColorShader;

	float mRotationY = 0.0f;
};