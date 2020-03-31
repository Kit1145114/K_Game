#include "stdafx.h"
#include "StoneEnemy.h"

//�G������čŏ��ɌĂԏ����B
StoneEnemy::StoneEnemy()
{ 
	seModel.Init(L"Assets/modelData/Enemy3.cmo");		//���f���̌Ăяo���B
	//���f���̃A�j���[�V�����̃��[�h�B
	s_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//�A�j���[�V���������[�h�B
	s_animClip[0].SetLoopFlag(true);
	s_animClip[1].Load(L"Assets/animData/E3_walk.tka");	//�A�j���[�V���������[�h�B
	s_animClip[1].SetLoopFlag(true);
	s_animClip[2].Load(L"Assets/animData/E3_ATK.tka");	//�A�j���[�V���������[�h�B
	s_animClip[2].SetLoopFlag(true);
	s_anim.Init(
		seModel,
		s_animClip,
		m_AnimClipNum
	);
	s_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 400;										//�����B
	m_scale = { 3.0f,3.0f,3.0f };						//���f���̑傫���B
	m_position = e_pos2;
	m_charaCon.Init(150.0f, 500.0f, m_position);			//����̑傫��
	e_state = esIdle;									//�ŏ��Ȃ̂őҋ@�B
}
//�G�̍U�������B
void StoneEnemy::Attack()
{
	m_player->Damage(prm.ATK);
}
//DAMAGE�󂯂鏈��
void StoneEnemy::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		Death();
	}
}
//�v���C���[�̌����鏈���B
void StoneEnemy::Search()
{
	float Track = 500.0f;
	CVector3 diff = m_player->GetPosition() - m_position;
	if (diff.Length() <= Track)
	{
		Move = m_player->GetPosition() - m_position;
		e_state = esTracking;
		if (diff.Length() <= 200.0f)
		{
			m_moveSpeed.x = ZERO;
			m_moveSpeed.z = ZERO;
			e_state = esAttack;
		}
	}
	else if (diff.Length() >= Track)
	{
		e_state = esIdle;
		Move = CVector3::Zero();
	}
}
//�G�̍X�V���e�B
void StoneEnemy::Update()
{
	Draw();
	EnemyState();
	Rotation();
	s_anim.Play(0);
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	seModel.UpdateWorldMatrix(m_position,m_rotation, m_scale);
	s_anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
}
//�G�̕`�揈���B
void StoneEnemy::Draw()
{
	seModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void StoneEnemy::Death()
{
	this->SetActive(false);
	m_charaCon.RemoveRigidBoby();
	isDeath = true;
}
//�G�l�~�[���i�ޏ����B
void StoneEnemy::EMove() 
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//�G�l�~�[�̃A�j���[�V������Ԃŕς��Ă��
void StoneEnemy::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		s_anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		s_anim.Play(1);
		break;
	case Enemys::esAttack:
		break;
	}
}
//�G�l�~�[�̉�]�����B
void StoneEnemy::Rotation()
{
	float None = 0.0f;		//�}�W�b�N�i���o�[��h��0����ꂽ��
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	seModel.SetRotation(qRot);
	//�����A�����Ă������]������B
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		seModel.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		seModel.SetRotation(m_rotation);
	}
	seModel.SetRotation(m_rotation);
}
//�A�j���[�V�����C�x���g
void StoneEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	float Kyori = 500.0f;
	if (m_player->GetIsDead() == false) {
		CVector3 diff = m_position - m_player->GetPosition();
		if (diff.Length() <= Kyori && eventName)
		{
			//MessageBox(NULL, TEXT("Hit114514"), TEXT("�߂���"), MB_OK);
			m_player->Damage(prm.ATK);
		}
	}
}