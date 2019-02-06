#pragma once
#include "Base.h"
#include "InputClass.h"

class Camera
{
	DECLAER_SINGLE(Camera);

public:

	bool Initialize();
	bool ShutDown();

	void Update(InputClass * input);

private:
	D3DXVECTOR3				m_vEye;		//카메라의 현재 위치
	D3DXVECTOR3				m_vLookat;	//카메라의 시선 위치
	D3DXVECTOR3				m_vUp;		//카메라의 상방 벡터

	D3DXVECTOR3		m_vView;		/// 카메라가 향하는 단위방향벡터
	D3DXVECTOR3		m_vCross;		/// 카마레의 측면벡터 cross( view, up )

public:
	D3DXMATRIX	m_matView;		/// 카메라 행렬

	/// 카메라 행렬을 얻어낸다.
	D3DXMATRIX*	GetViewMatrix() { return &m_matView; }
	
	/// 카메라 행렬을 생성하기위한 기본 벡터값들을 설정한다.
	D3DXMATRIX*			SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp);

	/// 카메라의 위치값을 설정한다.
	void			SetEye(D3DXVECTOR3* pv) { m_vEye = *pv; }

	/// 카메라의 위치값을 얻어낸다.
	D3DXVECTOR3*	GetEye() { return &m_vEye; }

	/// 카메라의 시선값을 설정한다.
	void			SetLookat(D3DXVECTOR3* pv) { m_vLookat = *pv; }

	/// 카메라의 시선값을 얻어낸다.
	D3DXVECTOR3*	GetLookat() { return &m_vLookat; }

	/// 카메라의 상방벡터값을 설정한다.
	void			SetUp(D3DXVECTOR3* pv) { m_vUp = *pv; }

	/// 카메라의 상방벡터값을 얻어낸다.
	D3DXVECTOR3*	GetUp() { return &m_vUp; }

	/// 카메라 좌표계의 X축으로 angle만큼 회전한다.
	D3DXMATRIX*	RotateLocalX(float angle);

	/// 카메라 좌표계의 Y축으로 angle만큼 회전한다.
	D3DXMATRIX*	RotateLocalY(float angle);

	/// 월드좌표계의 *pv값의 위치로 이동한다.
	D3DXMATRIX*	MoveTo(D3DXVECTOR3* pv);

	/// 카메라 좌표계의 X축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIX*	MoveLocalX(float dist);

	/// 카메라 좌표계의 Y축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIX*	MoveLocalY(float dist);

	/// 카메라 좌표계의 Z축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIX*	MoveLocalZ(float dist);
};

