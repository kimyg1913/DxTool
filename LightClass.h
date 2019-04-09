#pragma once

#include "Base.h"

class LightClass
{
public:
	LightClass();
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetPosition(float, float, float);
	void SetViewMatirx();
	void SetProjMatirx();

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR3 GetPosition();

	D3DXMATRIX * GetViewMatrix() { return &m_viewMatrix; };
	D3DXMATRIX * GetProjMatrix() { return &m_projMatrix; };

private:
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
	D3DXVECTOR3 m_position;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projMatrix;
};

