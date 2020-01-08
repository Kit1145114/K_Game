#include "stdafx.h"
#include "Player.h"
#include"GameConst.h"
#include"Game.h"

Player::Player()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	Gmodel.Init(L"Assets/modelData/Player.cmo");		//�v���C���[�̕`��
	g_animClip[0].Load(L"Assets/animData/P_idle.tka");	//�ҋ@�̃��[�h
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/P_walk.tka");	//�����̃��[�h
	g_animClip[1].SetLoopFlag(true);
	g_animClip[2].Load(L"Assets/animData/P_ATK.tka");	//����̃��[�h
	g_animClip[2].SetLoopFlag(false);
	//g_animClip[3].Load(L"Assets/animData/P_walk.tka");	//�����̃��[�h
	//g_animClip[3].SetLoopFlag(true);
	g_anim.Init(
		Gmodel,			//�A�j���[�V�����𗬂��X�L�����f��
						//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t������B
		g_animClip,		//�A�j���[�V�����N���b�v�̔z��B
		m_AnimClipNum		//�A�j���[�V�����N���b�v�̐�
	);
	//�悵�A���Ⴀ�A�j���[�V�����C�x���g�Ԃ�����ł�邺�B
	g_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});

	m_charaCon.Init(40.0f, 10.0f, m_position);			//�L�����R���̐ݒ�i���a�A�����A�����ʒu�B�j
	HP = 100.0f;		//�v���C���[�̏����̗́B
	ATK = 80.0f;		//�v���C���[�̍U���́B
	DEF = 0.0f;		//�v���C���[�̖h��́B
	playerState = pl_idle;
}

Player::~Player()
{
}
//�v���C���[�̃A�b�v�f�[�g�B
void Player::Update()
{
	//�v���C���[�̍X�V�������ɋL�q�B
	Draw();							//�v���C���[�̕`����ĂԁB
	if (playerState != pl_Death) {
		Move();							//�v���C���[�̈ړ����ĂԁB
		PlayerAttack();					//�v���C���[�̍U����
		PlayerState();					//�v���C���[�̏�Ԃ��ĂԁB
		Rotation();						//�v���C���[�̉�]���ĂԁB
		//Track();						//�v���C���[���G��T���B
		Forward();						//�v���C���[�̑O�x�N�g���擾�B
	}
	g_anim.Update(0.025f * NSpeed);	//�A�j���[�V�������t���[���P�ʂŕ`��B
			//���[���h�s��̍X�V�B
	Gmodel.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
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
	//�i�ޑ����̒l���߁B
	float Speed = 500.0f;
	//���X�e�B�b�N�󂯎��}�V�[��
	float lStick_x = (g_pad[0].GetLStickXF());
	float lStick_y = (g_pad[0].GetLStickYF());
	//�J�����̑O�������ƉE�������擾�B
	CVector3 cameraForward = g_camera3D.GetForward();
	CVector3 cameraRight = g_camera3D.GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = None;
	cameraForward.Normalize();
	cameraRight.y = None;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A�B
	m_moveSpeed.x = None;
	m_moveSpeed.z = None;
	if (g_pad[0].IsPress(enButtonX) && playerState == pl_Walk) {
		//����
		m_moveSpeed += cameraForward * lStick_y * Speed * SPeed2;	//�������ւ̈ړ����x�����Z�B
		m_moveSpeed += cameraRight * lStick_x * Speed * SPeed2;		//�E�����ւ̈ړ����x�����Z�B
	}
	else
	{
		//�����B
		m_moveSpeed += cameraForward * lStick_y * Speed * NSpeed;	//�������ւ̈ړ����x�����Z�B
		m_moveSpeed += cameraRight * lStick_x * Speed * NSpeed;		//�E�����ւ̈ړ����x�����Z�B
	}
	//�L�����N�^�[�R���g���[���[�ɂP�t���[���̌o�ߕb���A���ԃx�[�X�̈ړ����x��n���Ă���B
	m_charaCon.SetPosition(m_position);		//�L�����R���ɍ��W��n���B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
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
	switch (playerState)
	{
	case pl_idle:	//�ҋ@���
		MoveOperation();
		if (m_moveSpeed.x != None || m_moveSpeed.z != None)
		{
			playerState = pl_Walk;	//�����Ƀ`�F���W�B
		}
		g_anim.Play(0);
		break;
	case pl_Walk:	//������ԁB
		MoveOperation();
		if (m_moveSpeed.x == None && m_moveSpeed.z == None)
		{
			playerState = pl_idle;
		}
		g_anim.Play(1);
		break;
	case pl_Atk:	//�U����ԁB
		m_moveSpeed.z = None;
		m_moveSpeed.x = None;
		g_anim.Play(2);
		if (g_anim.IsPlaying() == false)
		{
			playerState = pl_idle;
		}
		break;
	case pl_Death:	//���S���
		g_anim.Play(0);
		break;
	}
}
//�v���C���[�̍U���ށB
void Player::PlayerAttack()
{
	//����B�{�^���������ꂽ��A�p���`�B
	//�U�����[�V�����ƃS�[�X�g�̓����蔻��𐶐��B
	if (g_pad[0].IsTrigger(enButtonB))
	{
		playerState = pl_Atk;
		m_PhyGhostObj.CreateBox(m_forward, m_rotation, m_scale);
	}
}
//�v���C���[�̈ړ��ށB
void Player::MoveOperation()
{
		//�p�b�h��ABUTTON���͂ŃW�����v����B
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_moveSpeed.y += JumpPower;
	}
	else
	{
		//Y�����ւ̏d�͂�����B
		m_moveSpeed.y -= gravity; //* (1.0f / 60.0f);
	}
}
//�A�j���[�V�����C�x���g
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	float Kyori = 500.0f;
	for (auto enemy : m_goList) {
		if (enemy->GetIsDead() == false) {
			CVector3 diff = m_position - enemy->GetPosition();
			if (diff.Length() <= Kyori && eventName)
			{
				MessageBox(NULL, TEXT("Hit114514"), TEXT("�߂���"), MB_OK);
				enemy->Damage(ATK);
			}
		}
	}

}
//�G�Ƃ̋����v���ƃL���B
void Player::Track()
{
}
//�O�x�N�g��
void Player::Forward()
{
	Rot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = Rot.m[2][0];
	m_forward.y = Rot.m[2][1];
	m_forward.z = Rot.m[2][2];
}
//DAMAGE�̏����B
void Player::Damage(float Damage)
{
	HP -= (Damage - DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (HP <= 0.0f)
	{
		playerState = pl_Death;
		MessageBox(NULL, TEXT("�͂��G�����O�O"), TEXT("�߂���"), MB_OK);
	}
}