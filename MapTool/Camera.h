#pragma once
#include "Base.h"
#include "InputClass.h"

class Camera
{
public:
	Camera();
	~Camera();

public:
	bool Initialize();
	bool ShutDown();

	void Frame(InputClass * input);

private:
	D3DXVECTOR3				m_vEye;		//ī�޶��� ���� ��ġ
	D3DXVECTOR3				m_vLookat;	//ī�޶��� �ü� ��ġ
	D3DXVECTOR3				m_vUp;		//ī�޶��� ��� ����

	D3DXVECTOR3		m_vView;		/// ī�޶� ���ϴ� �������⺤��
	D3DXVECTOR3		m_vCross;		/// ī������ ���麤�� cross( view, up )

public:
	D3DXMATRIXA16	m_matView;		/// ī�޶� ���

	/// ī�޶� ����� ����.
	D3DXMATRIXA16*	GetViewMatrix() { return &m_matView; }
	
	/// ī�޶� ����� �����ϱ����� �⺻ ���Ͱ����� �����Ѵ�.
	D3DXMATRIXA16*			SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp);

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
	D3DXMATRIXA16*	RotateLocalX(float angle);

	/// ī�޶� ��ǥ���� Y������ angle��ŭ ȸ���Ѵ�.
	D3DXMATRIXA16*	RotateLocalY(float angle);

	/// ������ǥ���� *pv���� ��ġ�� �̵��Ѵ�.
	D3DXMATRIXA16*	MoveTo(D3DXVECTOR3* pv);

	/// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalX(float dist);

	/// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalY(float dist);

	/// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalZ(float dist);
};

