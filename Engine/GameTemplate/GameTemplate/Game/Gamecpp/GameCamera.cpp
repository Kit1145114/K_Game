#include "stdafx.h"
#include "GameCamera.h"
#include"Player.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 

GameCamera::GameCamera()
{
	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	c_State = toPlayer;
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	if (g_pad[0].IsPress(enButtonRB2) && c_State == toPlayer)
	{
		c_State == toEnemys;
	}
	else if (g_pad[0].IsPress(enButtonRB2) && c_State == toEnemys)
	{
		c_State == toPlayer;
	}

	switch (c_State)
	{
	case GameCamera::toPlayer:
		CameraRotate();
		break;
	case GameCamera::toEnemys:
		CameraLookEnemys();
		break;
	}
	//CVector3 targetPos = m_player->GetPosition();	//�v���C���[�̃|�W�V�������擾�B
	//targetPos.y += 100.0f;							//�ォ�猩�����̂�100.0f����B
	//CVector3 position = targetPos;					//�|�W�V�����Ƀv���C���[�̃|�W�V����������B
	//position.z -= 400.0f;
	////�p�b�h�̓��͗ʂŃJ�������񂷁B
	//float x = g_pad[0].GetRStickXF();	//X�����ւ̈ړ������B
	//float z = g_pad[0].GetRStickYF();	//Y�����ւ̈ړ������B
	//	//Y������̉�]
	////CQuaternion qRot;
	//m_rotate.SetRotationDeg(CVector3::AxisY(), 200.0f * x);
	//m_rotate.SetRotationDeg(CVector3::AxisY(), 200.0f * z);
	////��납�猩���̂�400.0f����
	//g_camera3D.SetTarget(targetPos);				//�v���C���[�̃|�W�V�������^�[�Q�b�g�ɁB
	//g_camera3D.SetPosition(position);				//�J�����̃|�W�V������position��
	//g_camera3D.SetRotation(m_rotate);
	//g_camera3D.Update();							//�A�b�v�f�[�g�B
}

void GameCamera::CameraRotate()
{
	m_playerposition = m_player->GetPosition();
	CVector3 stickR;
	stickR.x = -g_pad[0].GetRStickXF();	//�A�i���O�X�e�B�b�N��x�̓��͗ʂ��擾�B
	stickR.y = g_pad[0].GetRStickYF();	//�A�i���O�X�e�B�b�N��x�̓��͗ʂ��擾�B
	stickR.z = 0.0f;

	m_playerposition.z -= 400.0f;
	//�E�X�e�B�b�N�̓���
	//�E�X�e�B�b�N
	m_sdegreexz = -stickR.x * 5.0f;
	m_sdegreey = -stickR.y*5.0f;

	//��]�x���Z
	m_degreexz += m_sdegreexz;
	m_degreey += m_sdegreey;
	//�㉺�����̃J�����ړ��𐧌�
	if (m_degreey >= 70.0f) {
		m_degreey = 70.0f;
	}
	if (m_degreey <= -15.0f) {
		m_degreey = -15.0f;
	}
	//�p�x�����W�A���P�ʂɒ���
	m_radianx = M_PI / 180 * m_degreexz;
	m_radiany = M_PI / 180 * m_degreey;
	Hutu();
	//���_
	g_camera3D.SetTarget(m_target);
	//���W
	g_camera3D.SetPosition(m_position);
	//�J�����̍X�V�B
	g_camera3D.Update();
}

void GameCamera::Hutu()
{
	m_target = { 0.0f,0.0f,0.0f };
	m_target.y += 140.0f;

	m_target += m_player->GetPosition();
	//�����_���v�Z����B
	//target.y += 200.0f;
	//Y������ɉ�]������B
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), m_radianx);
	m_toPos = { 0.0f, 0.0f, 1.0f };
	qRot.Multiply(m_toPos);
	//�㉺�̉�]�B
	//�܂��͉񂷎����v�Z����B
	CVector3 rotAxis;
	rotAxis.Cross(m_toPos, CVector3::AxisY());
	//�x�N�g���𐳋K������B
	rotAxis.Normalize();
	qRot.SetRotation(rotAxis, m_radiany);
	qRot.Multiply(m_toPos);
	m_toPos *= m_r;
	m_position = m_target + m_toPos * 2.0f;

	m_toPos *= 4;
	m_target -= m_toPos;
	m_toPos *= 1.5f;
}

void GameCamera::CameraLookEnemys()
{
	for (auto enemy : m_goList) {
		if (enemy->GetIsDead() == false) {
			CVector3 diff = enemy->GetPosition();
			if (diff.Length() >= 400)
			{
				m_target = enemy->GetPosition();
			}
		}
	}
	g_camera3D.SetTarget(m_target);
	//���W
	g_camera3D.SetPosition(m_position);
	//�J�����̍X�V�B
	g_camera3D.Update();
}