#include "stdafx.h"
#include "FrustumClass.h"
#include "D3DClass.h"
#include "Camera.h"

FrustumClass::FrustumClass()
{
}


FrustumClass::~FrustumClass()
{
}

bool FrustumClass::MakeVectorAndPlane()
{
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 500.0f);
	D3DXMATRIX	matInv = *(Camera::GetInstance()->GetViewMatrix()) * matProj;// D3DClass::GetInstance()->GetWorldProjection();

	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// m_vtx�� �� ���������� ��谪�� �־�д�.
	m_vectorFrustum[0].x = -1.0f;	m_vectorFrustum[0].y = -1.0f;	m_vectorFrustum[0].z = 0.0f;
	m_vectorFrustum[1].x =  1.0f;	m_vectorFrustum[1].y = -1.0f;	m_vectorFrustum[1].z = 0.0f;
	m_vectorFrustum[2].x =  1.0f;	m_vectorFrustum[2].y = -1.0f;	m_vectorFrustum[2].z = 1.0f;
	m_vectorFrustum[3].x = -1.0f;	m_vectorFrustum[3].y = -1.0f;	m_vectorFrustum[3].z = 1.0f;
	m_vectorFrustum[4].x = -1.0f;	m_vectorFrustum[4].y =  1.0f;	m_vectorFrustum[4].z = 0.0f;
	m_vectorFrustum[5].x =  1.0f;	m_vectorFrustum[5].y =  1.0f;	m_vectorFrustum[5].z = 0.0f;
	m_vectorFrustum[6].x =  1.0f;	m_vectorFrustum[6].y =  1.0f;	m_vectorFrustum[6].z = 1.0f;
	m_vectorFrustum[7].x = -1.0f;	m_vectorFrustum[7].y =  1.0f;	m_vectorFrustum[7].z = 1.0f;

	D3DXMatrixInverse(&matInv, NULL, &matInv);

	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
	// �׷��Ƿ�, m_vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vectorFrustum[i], &m_vectorFrustum[i], &matInv);

	// 0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
	m_vCameraPos = (m_vectorFrustum[0] + m_vectorFrustum[5]) / 2.0f;

	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// �� ���(top)
//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// �� ���(bottom)
//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// �� ���(near)
	D3DXPlaneFromPoints(&m_planeFrustum[3], m_vectorFrustum + 2, m_vectorFrustum + 6, m_vectorFrustum + 7);	// �� ���(far)
	D3DXPlaneFromPoints(&m_planeFrustum[4], m_vectorFrustum,	 m_vectorFrustum + 3, m_vectorFrustum + 7);	// �� ���(left)
	D3DXPlaneFromPoints(&m_planeFrustum[5], m_vectorFrustum + 1, m_vectorFrustum + 5, m_vectorFrustum + 6);	// �� ���(right)

	return true;
}

bool FrustumClass::IsIn(D3DXVECTOR3 pV)
{
	float fDist;
				
	fDist = D3DXPlaneDotCoord(&m_planeFrustum[3], &pV);
	
	if (fDist > PLANE_EPSILON) 
		return false;	// plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
	
	fDist = D3DXPlaneDotCoord(&m_planeFrustum[4], &pV);
	
	if (fDist > PLANE_EPSILON) 
		return false;	// plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
	
	fDist = D3DXPlaneDotCoord(&m_planeFrustum[5], &pV);
	
	if (fDist > PLANE_EPSILON) 
		return false;	// plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
	

	return true;
}

bool FrustumClass::IsInSphere(D3DXVECTOR3 pV, float radius)
{
	float		fDist;

	fDist = D3DXPlaneDotCoord(&m_planeFrustum[3], &pV);

	if (fDist > (radius + PLANE_EPSILON)) 
		return false;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	fDist = D3DXPlaneDotCoord(&m_planeFrustum[4], &pV);

	if (fDist > (radius + PLANE_EPSILON)) 
		return false;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	fDist = D3DXPlaneDotCoord(&m_planeFrustum[5], &pV);

	if (fDist > (radius + PLANE_EPSILON)) 
		return false;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	return true;
}
