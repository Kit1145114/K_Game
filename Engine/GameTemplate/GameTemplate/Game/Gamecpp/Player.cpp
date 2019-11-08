#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	Gmodel.Init(L"Assets/modelData/Gmodel.cmo");		//�v���C���[�̕`��
	g_animClip[0].Load(L"Assets/animData/G_idle.tka");	//�ҋ@�̃��[�h
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/G_walk.tka");	//�����̃��[�h
	g_animClip[1].SetLoopFlag(true);
	g_animClip[2].Load(L"Assets/animData/G_ATK.tka");	//����̃��[�h
	g_animClip[2].SetLoopFlag(false);
	g_anim.Init(
		Gmodel,			//�A�j���[�V�����𗬂��X�L�����f��
						//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t������B
		g_animClip,		//�A�j���[�V�����N���b�v�̔z��B
		3				//�A�j���[�V�����N���b�v�̐�
	);
	m_charaCon.Init(40.0f, 10.0f, m_position);			//�L�����R���̐ݒ�i���a�A�����A�����ʒu�B�j
	HP = 1000.0f;		//�v���C���[�̏����̗́B
	ATK = 80.0f;		//�v���C���[�̍U���́B
	DEF = 50.0f;		//�v���C���[�̖h��́B
	plClip = plAnimClip_idle;
}

Player::~Player()
{
}
//�v���C���[�̃A�b�v�f�[�g�B
void Player::Update()
{
	//�v���C���[�̍X�V�������ɋL�q�B
	Draw();							//�v���C���[�̕`����ĂԁB
	Move();							//�v���C���[�̈ړ����ĂԁB
	PlayerAttack();					//�v���C���[�̍U����
	PlayerState();					//�v���C���[�̏�Ԃ��ĂԁB
	Rotation();						//�v���C���[�̉�]���ĂԁB
	g_anim.Update(1.0f / 30.0f);	//�A�j���[�V�������t���[���P�ʂŕ`��B
}
//�v���C���[�̕`�揈���B
void Player::Draw()
{
	Gmodel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}
//�v���C���[�̈ړ�����
void Player::Move()
{
	//m_moveSpeed�̓v���C���[��class�̃����o�ϐ��B
	//�L�����N�^�[�R���g���[���[�ɂP�t���[���̌o�ߕb���A���ԃx�[�X�̈ړ����x��n���Ă���B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	m_charaCon.SetPosition(m_position);		//�L�����R���ɍ��W��n���B
		//���[���h�s��̍X�V�B
	Gmodel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
}
//�v���C���[�̉�]����
void Player::Rotation()
{
	float None = 0.0f;		//�}�W�b�N�i���o�[��h��0����ꂽ��
	float Rot = atan2(m_moveSpeed.x, m_moveSpeed.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	Gmodel.SetRotation(qRot);
	//�����A�����Ă������]������B
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		Gmodel.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		Gmodel.SetRotation(m_rotation);
	}
	Gmodel.SetRotation(m_rotation);
}
//�v���C���[�̏�ԏ����B
void Player::PlayerState()
{
	//�����A�����Ă��Ȃ��Ƃ�����ł킩��₷���������ɕύX�\��B
	//�v���C���[�̏�Ԃŏ����̕ύX�B
	switch (plClip)
	{
	case plAnimClip_idle:	//�ҋ@���
		MoveOperation();
		if (m_moveSpeed.x != 0 || m_moveSpeed.z != 0)
		{
			plClip = plAnimClip_Walk;	//�����Ƀ`�F���W�B
		}
		g_anim.Play(0);
		break;
	case plAnimClip_Walk:	//������ԁB
		MoveOperation();
		if (m_moveSpeed.x == 0.0f && m_moveSpeed.z == 0)
		{
			plClip = plAnimClip_idle;
		}
		g_anim.Play(1);
		break;
	case plAnimClip_Atk:	//�U����ԁB
		m_moveSpeed.z = 0.0f;
		m_moveSpeed.x = 0.0f;
		g_anim.Play(2);
		if (g_anim.IsPlaying() == false)
		{
			plClip = plAnimClip_idle;
		}
		break;
	}
}
//�v���C���[�̍U���ށB
void Player::PlayerAttack()
{
	if (g_pad[0].IsTrigger(enButtonB))
	{
		plClip = plAnimClip_Atk;
	}
}
//�v���C���[�̈ړ��ށB
void Player::MoveOperation()
{
	m_moveSpeed.x = g_pad[0].GetLStickXF() * 300.0f;	//X�����ւ̈ړ������B
	m_moveSpeed.z = g_pad[0].GetLStickYF() * 300.0f;	//Y�����ւ̈ړ������B
		//�p�b�h��ABUTTON���͂ŃW�����v����B
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_moveSpeed.y += 400.0f;
	}
	else
	{
		//Y�����ւ̏d�͂�����B
		m_moveSpeed.y -= 100.0f; //* (1.0f / 60.0f);
	}
}