#include "stdafx.h"
#include "LightClass.h"


LightClass::LightClass()
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}

void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

void LightClass::SetViewMatirx()
{
	D3DXVECTOR3 vLook = m_direction - m_position;
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3 vRight;
	D3DXVECTOR3 vY;
	//D3DXVec3Cross(&vRight, &vLook, &vUp);
	//D3DXVec3Cross(&vY, &vLook, &vRight);

	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_direction, &vUp);
}

void LightClass::SetProjMatirx()
{
	D3DXMatrixPerspectiveFovLH(&m_projMatrix, FOV, 1, 1, 500);
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

D3DXVECTOR3 LightClass::GetPosition()
{
	return m_position;
}
