#include "stdafx.h"
#include "GameCamera.h"
#include"Chara/Player.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 

GameCamera::GameCamera()
{
	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	c_state = toPlayer;
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	m_player->AddEventListener(this);
	return true;
}

void GameCamera::Update()
{
	State();
}

void GameCamera::CameraRotate()
{
	m_playerPosition = m_player->GetPosition();
	CVector3 stickR;
	stickR.x = -g_pad[0].GetRStickXF();	//�A�i���O�X�e�B�b�N��x�̓��͗ʂ��擾�B
	stickR.y = g_pad[0].GetRStickYF();	//�A�i���O�X�e�B�b�N��x�̓��͗ʂ��擾�B
	stickR.z = 0.0f;

	m_playerPosition.z -= 400.0f;
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
	if (m_degreey <= 5.0f) {
		m_degreey = 5.0f;
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
	//if (damage_flag) {
		//return;
	//}
	if (!damage_flag) {
		m_target = m_player->GetPosition();
		m_target.y += 140.0f;
	}
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
	if (!damage_flag) {
		m_position = m_target + m_toPos;
	}
	m_toPos *= 1.5f;
}

void GameCamera::CameraLookEnemys()
{
	//�v���C���[�̍��W�A�G�l�~�[�̍��W�擾�B
	m_playerPosition = m_player->GetPosition();
	m_enemyPosition = m_player->GetRookEnemyPos();
	//�����_�̓G�l�~�[�̍��W�ɂ��܂��B
	m_target = m_enemyPosition;
	//�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂܂��B
	CVector3 pos = m_playerPosition - m_enemyPosition;
	//�J�����̍����͈��ɂ������̂ŁAy������0�ɂ��܂��B
	pos.y = 0.0f;
	//�x�N�g���𐳋K�����܂��B
	pos.Normalize();
	//�X�J���[��������B
	pos *= 200.0f;
	//�v���C���[�̍��W�ɋ��߂��x�N�g���𑫂��āA�J�����̍��W�Ƃ���B
	m_position = m_playerPosition + pos;
	m_position.y += 500.0f;
	g_camera3D.SetTarget(m_target);
	//���W
	g_camera3D.SetPosition(m_position);
	//�J�����̍X�V�B
	g_camera3D.Update();
}
//�J�����̂̏��
void GameCamera::State()
{
	switch (c_state)
	{
	case GameCamera::toPlayer:
		CameraRotate();
		PlayerDamageRot();
		break;
	case GameCamera::toEnemys:
		CameraLookEnemys();
		break;
	}
}
//�_���[�W���󂯂��Ƃ��ɃJ�������ςɂȂ�B
void GameCamera::PlayerDamageRot()
{
	if (damage_flag) {
		if (timer == 0.0f) {
			m_right = g_camera3D.GetRight();
		}
		timer += GameTime().GetFrameDeltaTime();
		//���Ԃł����ɕς��Ă܂��B
		if (timer < 0.05f)
		{
			m_target += (m_right * 10.0f);
			m_position += (m_right * 10.0f);
		}
		else if (timer >=0.05f && timer <=0.10f)
		{
			m_target -= (m_right * 10.0f);
			m_position -= (m_right * 10.0f);
		}
		else if (timer >= 0.10f && timer <= 0.15f)
		{
			m_target +=(m_right * 10.0f);
			m_position += (m_right * 10.0f);
		}
		else if (timer >= 0.15f && timer <= 0.20f)
		{
			m_target -= (m_right * 10.0f);
			m_position -= (m_right * 10.0f);
		}
		else if (timer > 0.20f)
		{
			//�����œ����Ȃ������Ƃ���ɖ߂�̂�
			timer = 0.0f;
			damage_flag = false;
		}
	}
}
//
void GameCamera::OnStartLockOn(Player* pl)
{
	//�J�����̃^�[�Q�b�g��G�ցB
	c_state = toEnemys;
}
//
void GameCamera::OnEndLockOn(Player* pl)
{
	//�J�����̃^�[�Q�b�g���v���C���[�ցB
	c_state = toPlayer;
}
//
void GameCamera::OnDamage(Player* pl)
{
	damage_flag = true;
}