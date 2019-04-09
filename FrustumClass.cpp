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

	// 투영행렬까지 거치면 모든 3차원 월드좌표의 점은 (-1,-1,0) ~ (1,1,1)사이의 값으로 바뀐다.
	// m_vtx에 이 동차공간의 경계값을 넣어둔다.
	m_vectorFrustum[0].x = -1.0f;	m_vectorFrustum[0].y = -1.0f;	m_vectorFrustum[0].z = 0.0f;
	m_vectorFrustum[1].x =  1.0f;	m_vectorFrustum[1].y = -1.0f;	m_vectorFrustum[1].z = 0.0f;
	m_vectorFrustum[2].x =  1.0f;	m_vectorFrustum[2].y = -1.0f;	m_vectorFrustum[2].z = 1.0f;
	m_vectorFrustum[3].x = -1.0f;	m_vectorFrustum[3].y = -1.0f;	m_vectorFrustum[3].z = 1.0f;
	m_vectorFrustum[4].x = -1.0f;	m_vectorFrustum[4].y =  1.0f;	m_vectorFrustum[4].z = 0.0f;
	m_vectorFrustum[5].x =  1.0f;	m_vectorFrustum[5].y =  1.0f;	m_vectorFrustum[5].z = 0.0f;
	m_vectorFrustum[6].x =  1.0f;	m_vectorFrustum[6].y =  1.0f;	m_vectorFrustum[6].z = 1.0f;
	m_vectorFrustum[7].x = -1.0f;	m_vectorFrustum[7].y =  1.0f;	m_vectorFrustum[7].z = 1.0f;

	D3DXMatrixInverse(&matInv, NULL, &matInv);

	// Vertex_최종 = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj 인데,
	// Vertex_world = Vertex_local * Matrix_world이므로,
	// Vertex_최종 = Vertex_world * Matrix_view * Matrix_Proj 이다.
	// Vertex_최종 = Vertex_world * ( Matrix_view * Matrix_Proj ) 에서
	// 역행렬( Matrix_view * Matrix_Proj )^-1를 양변에 곱하면
	// Vertex_최종 * 역행렬( Matrix_view * Matrix_Proj )^-1 = Vertex_World 가 된다.
	// 그러므로, m_vtx * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vectorFrustum[i], &m_vectorFrustum[i], &matInv);

	// 0번과 5번은 프러스텀중 near평면의 좌측상단과 우측하단이므로, 둘의 좌표를 더해서 2로 나누면
	// 카메라의 좌표를 얻을 수 있다.(정확히 일치하는 것은 아니다.)
	m_vCameraPos = (m_vectorFrustum[0] + m_vectorFrustum[5]) / 2.0f;

	// 얻어진 월드좌표로 프러스텀 평면을 만든다
	// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.
//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// 상 평면(top)
//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// 하 평면(bottom)
//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&m_planeFrustum[3], m_vectorFrustum + 2, m_vectorFrustum + 6, m_vectorFrustum + 7);	// 원 평면(far)
	D3DXPlaneFromPoints(&m_planeFrustum[4], m_vectorFrustum,	 m_vectorFrustum + 3, m_vectorFrustum + 7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&m_planeFrustum[5], m_vectorFrustum + 1, m_vectorFrustum + 5, m_vectorFrustum + 6);	// 우 평면(right)

	return true;
}

bool FrustumClass::IsIn(D3DXVECTOR3 pV)
{
	float fDist;
				
	fDist = D3DXPlaneDotCoord(&m_planeFrustum[3], &pV);
	
	if (fDist > PLANE_EPSILON) 
		return false;	// plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
	
	fDist = D3DXPlaneDotCoord(&m_planeFrustum[4], &pV);
	
	if (fDist > PLANE_EPSILON) 
		return false;	// plane의 normal벡터가 left로 향하고 있으므로 양수이면 프러스텀의 왼쪽
	
	fDist = D3DXPlaneDotCoord(&m_planeFrustum[5], &pV);
	
	if (fDist > PLANE_EPSILON) 
		return false;	// plane의 normal벡터가 right로 향하고 있으므로 양수이면 프러스텀의 오른쪽
	

	return true;
}

bool FrustumClass::IsInSphere(D3DXVECTOR3 pV, float radius)
{
	float		fDist;

	fDist = D3DXPlaneDotCoord(&m_planeFrustum[3], &pV);

	if (fDist > (radius + PLANE_EPSILON)) 
		return false;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	fDist = D3DXPlaneDotCoord(&m_planeFrustum[4], &pV);

	if (fDist > (radius + PLANE_EPSILON)) 
		return false;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	fDist = D3DXPlaneDotCoord(&m_planeFrustum[5], &pV);

	if (fDist > (radius + PLANE_EPSILON)) 
		return false;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	return true;
}
