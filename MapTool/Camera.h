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
	D3DXVECTOR3				m_vEye;		//ī�޶��� ���� ��ġ
	D3DXVECTOR3				m_vLookat;	//ī�޶��� �ü� ��ġ
	D3DXVECTOR3				m_vUp;		//ī�޶��� ��� ����

	D3DXVECTOR3		m_vView;		/// ī�޶� ���ϴ� �������⺤��
	D3DXVECTOR3		m_vCross;		/// ī������ ���麤�� cross( view, up )

public:
	D3DXMATRIX	m_matView;		/// ī�޶� ���

	/// ī�޶� ����� ����.
	D3DXMATRIX*	GetViewMatrix() { return &m_matView; }
	
	/// ī�޶� ����� �����ϱ����� �⺻ ���Ͱ����� �����Ѵ�.
	D3DXMATRIX*			SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp);

	/// ī�޶��� ��ġ���� �����Ѵ�.
	void			SetEye(D3DXVECTOR3* pv) { m_vEye = *pv; }

	/// ī�޶��� ��ġ���� ����.
	D3DXVECTOR3*	GetEye() { return &m_vEye; }

	/// ī�޶��� �ü����� �����Ѵ�.
	void			SetLookat(D3DXVECTOR3* pv) { m_vLookat = *pv; }

	/// ī�޶��� �ü����� ����.
	D3DXVECTOR3*	GetLookat() { return &m_vLookat; }

	/// ī�޶��� ��溤�Ͱ��� �����Ѵ�.
	void			SetUp(D3DXVECTOR3* pv) { m_vUp = *pv; }

	/// ī�޶��� ��溤�Ͱ��� ����.
	D3DXVECTOR3*	GetUp() { return &m_vUp; }

	/// ī�޶� ��ǥ���� X������ angle��ŭ ȸ���Ѵ�.
	D3DXMATRIX*	RotateLocalX(float angle);

	/// ī�޶� ��ǥ���� Y������ angle��ŭ ȸ���Ѵ�.
	D3DXMATRIX*	RotateLocalY(float angle);

	/// ������ǥ���� *pv���� ��ġ�� �̵��Ѵ�.
	D3DXMATRIX*	MoveTo(D3DXVECTOR3* pv);

	/// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIX*	MoveLocalX(float dist);

	/// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIX*	MoveLocalY(float dist);

	/// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIX*	MoveLocalZ(float dist);
};

