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
	m_vEye = D3DXVECTOR3(0.0f, 200.0f, -100.0f);
	m_vLookat = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	SetView(&m_vEye, &m_vLookat, &m_vUp);
	return true;
}

bool Camera::ShutDown()
{
	return true;
}

void Camera::Frame(InputClass * input)
{
	bool isKeyDown;
	int mouseMoveX, mouseMoveY;
	int mouseXpos, mouseYpos;

	POINT	pt;
	float	fDelta = 0.8f;	// 마우스의 민감도, 이 값이 커질수록 많이 움직인다.

	if (input->IsMouseRightClick() && input->GetMouseWindowPosition(mouseXpos,mouseYpos))
	{
		input->GetMouseMove(mouseMoveX, mouseMoveY);
		float yAngle=0.f, xAngle=0.f;

		if (mouseMoveX > 0)
			yAngle += mouseMoveX * 0.016 * fDelta;
		else if (mouseMoveX < 0)
			yAngle += mouseMoveX * 0.016 * fDelta;

		if (mouseMoveY > 0)
			xAngle += mouseMoveY * 0.016 * fDelta;
		else if (mouseMoveY < 0)
			xAngle += mouseMoveY * 0.016 * fDelta;

		RotateLocalX(xAngle);
		RotateLocalY(yAngle);
	}

	if (input->IsWPressed())
	{
		MoveLocalZ(fDelta*10);
	}

	if (input->IsSPressed())
	{
		MoveLocalZ(-fDelta * 10);
	}

	if (input->IsAPressed())
	{
		MoveLocalX(-fDelta * 10);
	}

	if (input->IsDPressed())
	{
		MoveLocalX(fDelta * 10);
	}
}

D3DXMATRIXA16* Camera::SetView(D3DXVECTOR3 * pvEye, D3DXVECTOR3 * pvLookat, D3DXVECTOR3 * pvUp)
{
	m_vEye =	*pvEye;
	m_vLookat = *pvLookat;
	m_vUp =		*pvUp;

	D3DXVec3Normalize(&m_vView, &(m_vLookat - m_vEye));
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);

	return nullptr;
}

D3DXMATRIXA16 * Camera::RotateLocalX(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vCross, angle);

	D3DXVECTOR3 vNewDst, vNewUp;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);	// view * rot로 새로운 dst vector를 구한다.
//	D3DXVec3Cross( &vNewUp, &vNewDst, &m_vCross );			// cross( dst, x축)으로 up vector를 구한다.
//	D3DXVec3Normalize( &vNewUp, &vNewUp );					// up vector를 unit vector로...
	vNewDst += m_vEye;										// 실제 dst position =  eye Position + dst vector

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16 * Camera::RotateLocalY(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vUp, angle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);	// view * rot로 새로운 dst vector를 구한다.
	vNewDst += m_vEye;										// 실제 dst position =  eye Position + dst vector

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16 * Camera::MoveTo(D3DXVECTOR3 * pv)
{
	m_vEye += *pv;

	return nullptr;
}

D3DXMATRIXA16 * Camera::MoveLocalX(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vCross);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16 * Camera::MoveLocalY(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vUp);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16 * Camera::MoveLocalZ(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vView);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}
