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
	g_animClip[2].Load(L"Assets/animData/P_FlyMove.tka");	//�u�[�X�g�̃��[�h
	g_animClip[2].SetLoopFlag(true);
	g_animClip[3].Load(L"Assets/animData/P_ATK.tka");	//����̃��[�h
	g_animClip[3].SetLoopFlag(false);
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

	m_charaCon.Init(100.0f, 200.0f, m_position);			//�L�����R���̐ݒ�i���a�A�����A�����ʒu�B�j
	HP = 100.0f;		//�v���C���[�̏����̗́B
	ATK = 100.0f;		//�v���C���[�̍U���́B
	DEF = 500.0f;		//�v���C���[�̖h��́B
	ENERGY = 300.0f;	//�v���C���[�̃u�[�X�g�ʁB
	playerState = pl_idle;
	playerENE = ene_Full;
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
		Energy();
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
	float Speed = 750.0f;
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
	if (g_pad[0].IsPress(enButtonX) && playerENE == ene_Full) {
		//����
		m_moveSpeed += cameraForward * lStick_y * Speed * SPeed2;	//�������ւ̈ړ����x�����Z�B
		m_moveSpeed += cameraRight * lStick_x * Speed * SPeed2;		//�E�����ւ̈ړ����x�����Z�B
	}
	else if(!g_pad[0].IsPress(enButtonX)|| playerENE == ene_Charge)
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
		g_anim.Play(0);
		break;
	case pl_Walk:	//������ԁB
		MoveOperation();
		g_anim.Play(1);
		break;
	case pl_FlyMove:
		MoveOperation();
		g_anim.Play(2);
		break;
	case pl_Atk:	//�U����ԁB
		m_moveSpeed.z = ZERO;
		m_moveSpeed.x = ZERO;
		g_anim.Play(3);
		if (g_anim.IsPlaying() == false)
		{
			playerState = pl_idle;
		}
		break;
	case pl_Death:	//���S���
		g_anim.Play(0);
		MessageBox(NULL, TEXT("�͂��G�����O�O"), TEXT("�߂���"), MB_OK);
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
		CVector3 A = m_position + (m_forward * 150.0f);
		A.y += 150.0f;
		m_PhyGhostObj.CreateBox(A, m_rotation, box_scale);
	}
}
//�v���C���[�̈ړ��ށB
void Player::MoveOperation()
{
		//�p�b�h��ABUTTON���͂ŏ㏸����B
	if (g_pad[0].IsPress(enButtonA) && playerENE == ene_Full)
	{
		m_moveSpeed.y += JumpPower;
	}
	else 
	{
		//�܂��㏸�������̃X�s�[�h���[���ցB
		if (!g_pad[0].IsPress(enButtonA) && m_moveSpeed.y >= ZERO) {
			//Y�����ւ̏d�͂�����B
			m_moveSpeed.y = ZERO; //* (1.0f / 60.0f);
		}
		//�����ŗ���
		if(!g_pad[0].IsPress(enButtonA) || playerENE == ene_Charge)
		{
			m_moveSpeed.y -= gravity * 1.5;
		}
	}
	//�����ĂȂ��Ƃ��͑ҋ@�ɁB
	if (m_moveSpeed.x == ZERO && m_moveSpeed.z == ZERO)
	{
		playerState = pl_idle;
	}
	//���V�ړ��Ƃ��B
	else if (m_moveSpeed.x != ZERO && m_moveSpeed.z != ZERO 
		&& g_pad[0].IsPress(enButtonX)&& playerENE == ene_Full)
	{
		playerState = pl_FlyMove;
	}
	//�����̎��B
	else if (m_moveSpeed.x != ZERO && m_moveSpeed.z != ZERO || playerENE == ene_Charge)
	{
		playerState = pl_Walk;
	}
}
//�A�j���[�V�����C�x���g
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//float Kyori = 500.0f;
	//for (auto enemy : m_goList) {
	//	if (enemy->GetIsDead() == false) {
	//		PhysicsWorld().ContactTest(enemy->GetCharaCon(), [&](const btCollisionObject& contactObject) {
	//			if (m_PhyGhostObj.IsSelf(contactObject) == true && eventName){
	//					MessageBox(NULL, TEXT("Hit114514"), TEXT("�߂���"), MB_OK);
	//					enemy->Damage(ATK);
	//			}
	//		});
	//	}
	//}
	float Kyori = 500.0f;
	for (auto enemy : m_goList) {
		if (enemy->GetIsDead() == false){
			CVector3 diff = m_position - enemy->GetPosition();
			if (diff.Length() <= Kyori && eventName)
			{
				MessageBox(NULL, TEXT("Hit114514"), TEXT("�߂���"), MB_OK);
				enemy->Damage(ATK);
			}
		}
	}
}
//�G�Ƃ̋����v���ƃL���B(���g�p)
void Player::Track()
{
}
//�O�x�N�g��
void Player::Forward()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}
//DAMAGE�̏����B
void Player::Damage(int Damage)
{
	if (Damage - DEF >= None) {
		HP -= (Damage - DEF);
	}
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (HP <= 0.0f)
	{
		m_death = true;
	}
	if (m_death)
	{
		playerState = pl_Death;
	}
}
//�G�l���M�[�Ɋւ��鏈���B
void Player::Energy()
{
	//�G�i�W�[��0�ɂȂ����烂�[�h�ύX�B
	if (ENERGY == MINENERGY)
	{
		playerENE = ene_Charge;
	}
	//charge���[�h���G�i�W�[���}�b�N�X�ɂȂ�܂�
	if (playerENE == ene_Charge && ENERGY < MAXENERGY)
	{
		ENERGY += ENERGYFLUCT / 3;
	}
	//�G�i�W�[���t���ɂȂ�����ʏ탂�[�h
	if (ENERGY == MAXENERGY)
	{
		playerENE = ene_Full;
	}
	if (ENERGY > MINENERGY && playerENE == ene_Full)
	{
		//MIN�K��l�ȉ������V�ړ����B
		if (playerState == pl_FlyMove)
		{
			ENERGY -= ENERGYFLUCT;
		}
		if (g_pad[0].IsPress(enButtonA))
		{
			ENERGY -= ENERGYFLUCT;
		}
		//MAX�K��l�ȉ����ҋ@���B
		if (playerState == pl_idle && ENERGY < MAXENERGY && !g_pad[0].IsPress(enButtonA))
		{
			ENERGY += ENERGYFLUCT;
		}
		//MAX�K��l�ȉ������������n��ɂ���Ƃ��B
		if (playerState == pl_Walk && ENERGY < MAXENERGY && !g_pad[0].IsPress(enButtonA))
		{
			ENERGY += ENERGYFLUCT;
		}
	}

}