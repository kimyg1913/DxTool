#pragma once

#include "Base.h"
#include "Camera.h"

#include "ColorShaderClass.h"

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass &);
	~D3DClass();

	bool D3DClass::Initialize(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen = false);
	HRESULT InitD3D(HWND hWnd, int screenWidth, int screenHeight);
	void Shutdown();

	LPDIRECT3DDEVICE9	GetDevice() { return m_pd3dDevice; }
	void RenderBegin();
	void RenderEnd();
private:
	void RenderScene(int r, int g, int b, int a);

private:
	LPDIRECT3D9             m_pD3D = NULL; // D3D ����̽��� ������ D3D��ü����
	LPDIRECT3DDEVICE9       m_pd3dDevice = NULL; // �������� ���� D3D����̽�
	LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9	m_pIB = NULL;

	D3DXMATRIXA16			m_matWorld;
	D3DXMATRIXA16			m_matView;
	D3DXMATRIXA16			m_matProjection;
	

	class Camera *			m_pCamera;
	class ColorShaderClass * m_pColorShader;
};