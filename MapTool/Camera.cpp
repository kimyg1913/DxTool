#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

bool Camera::Initialize()
{
	m_vEye = D3DXVECTOR3(0.0f, 50.0f, -30.0f);
	m_vLookat = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);

	return true;
}

bool Camera::ShutDown()
{
	return true;
}

D3DXMATRIXA16 * Camera::SetView(D3DXVECTOR3 * pvEye, D3DXVECTOR3 * pvLookat, D3DXVECTOR3 * pvUp)
{
	return nullptr;
}

D3DXMATRIXA16 * Camera::RotateLocalX(float angle)
{
	return nullptr;
}

D3DXMATRIXA16 * Camera::RotateLocalY(float angle)
{
	return nullptr;
}

D3DXMATRIXA16 * Camera::MoveTo(D3DXVECTOR3 * pv)
{
	return nullptr;
}

D3DXMATRIXA16 * Camera::MoveLocalX(float dist)
{
	return nullptr;
}

D3DXMATRIXA16 * Camera::MoveLocalY(float dist)
{
	return nullptr;
}

D3DXMATRIXA16 * Camera::MoveLocalZ(float dist)
{
	return nullptr;
}
