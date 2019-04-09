#pragma once


#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>

float VectorLength(D3DXVECTOR3 t)
{
	return sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
}

D3DXVECTOR3 VectorNormalize(D3DXVECTOR3 t)
{
	float length = VectorLength(t);

	return t / length;
}

void VectorNormalize(D3DXVECTOR3 * t)
{
	float length = VectorLength(*t);

	*t /= length;
}

