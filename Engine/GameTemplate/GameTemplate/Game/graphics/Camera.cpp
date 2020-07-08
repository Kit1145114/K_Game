#include "stdafx.h"
#include "Camera.h"

Camera g_camera3D;		//3D�J�����B

void Camera::Update()
{
	//�r���[�s����v�Z�B
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);

	CVector3 dir = m_position - m_target;
	CVector3 nDir = dir;
	nDir.Normalize();
	m_viewMatrix2.MakeLookAt(
		m_position + nDir * 300.0f,
		m_position,
		m_up
	);
	//�r���[�s��̋t�s����v�Z
	//����ŃJ�����s��ɂȂ�B
	m_viewMatrixInv.Inverse(m_viewMatrix);

	m_forward.Set(m_viewMatrixInv.m[2][0], m_viewMatrixInv.m[2][1], m_viewMatrixInv.m[2][2]);
	m_right.Set(m_viewMatrixInv.m[0][0], m_viewMatrixInv.m[0][1], m_viewMatrixInv.m[0][2]);
	//�v���W�F�N�V�����s����v�Z�B
	m_projMatrix.MakeProjectionMatrix(
		m_viewAngle,					//��p�B
		FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
		m_near,
		m_far
	);
}