#pragma once
#include "Base.h"
#include "TemplateSingleton.h"

#define PLANE_EPSILON 5.0f

class FrustumClass : public TemplateSingleton<FrustumClass>
{
public:
	FrustumClass();
	~FrustumClass();

public:
	bool MakeVectorAndPlane();
	bool IsIn(D3DXVECTOR3 pV);
	bool IsInSphere(D3DXVECTOR3 pV, float radius);

	D3DXVECTOR3 * GetCameraPos() { return &m_vCameraPos; }
private:
	D3DXVECTOR3 m_vectorFrustum[8];
	D3DXPLANE	m_planeFrustum[6];
	D3DXVECTOR3 m_vCameraPos;
};

