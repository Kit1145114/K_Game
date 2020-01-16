#include "stdafx.h"
#include "Boss.h"

//�G������čŏ��ɌĂԏ����B
Boss::Boss()
{
	Model.Init(L"Assets/modelData/RobbotBoss.cmo");		//���f���̌Ăяo���B
	//���f���̃A�j���[�V�����̃��[�h�B
	animClip[0].Load(L"Assets/animData/RB_idle.tka");	//�ҋ@�����[�h�B
	animClip[0].SetLoopFlag(true);
	animClip[1].Load(L"Assets/animData/RE1_walk.tka");	//���������[�h�B
	animClip[1].SetLoopFlag(true);
	animClip[2].Load(L"Assets/animData/RB_death.tka");	//���S�����[�h�B
	animClip[2].SetLoopFlag(false);
	animClip[3].Load(L"Assets/animData/RB_shortATK.tka");	//�U�������[�h�B
	animClip[3].SetLoopFlag(true);
	animClip[4].Load(L"Assets/animData/RB_BigATK.tka");	//�U�������[�h�B
	animClip[4].SetLoopFlag(true);
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 300;										//�����B
	m_scale = { 30.0f,30.0f,30.0f };					//�G�l�~�[�̑傫��
	m_charaCon.Init(150.0f, 10.0f, m_position);			//����̑傫��
	e_state = esIdle;									//�ŏ��Ȃ̂őҋ@�B
}
//�G�̍X�V���e�B
void Boss::Update()
{

}
//�G�̕`�揈���B
void Boss::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
//�G�̍U�������B
void Boss::Attack()
{
	m_player->Damage(m_ATK);
}
//�G�l�~�[���i�ޏ����B
void Boss::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//DAMAGE�󂯂鏈��
void Boss::Damage(float Dam)
{
	prm.HP -= (Dam - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//�v���C���[�̌����鏈���B
void Boss::Search()
{
	float Track = 500.0f;
	Move = m_player->GetPosition() - m_position;
	if (Move.Length() <= Track)
	{
		e_state = esTracking;
		if (Move.Length() <= 200.0f)
		{
			e_state = esAttack;
		}
	}
	else if (Move.Length() >= Track)
	{
		e_state = esIdle;
		Move = CVector3::Zero();
	}
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Boss::Death()
{
	//anim.Play(2);
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}
//�G�l�~�[�̃A�j���[�V������Ԃŕς��Ă��
void Boss::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		Rotation();
		//anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(1);
		break;
	case Enemys::esAttack:
		Search();
		Rotation();
		//anim.Play(3);
		break;
	case Enemys::esDeath:
		Death();
	}
}
//�G�l�~�[�̉�]�����B
void Boss::Rotation()
{
	float None = 0.0f;		//�}�W�b�N�i���o�[��h��0����ꂽ��
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	Model.SetRotation(qRot);
	//�����A�����Ă������]������B
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		Model.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		Model.SetRotation(m_rotation);
	}
	Model.SetRotation(m_rotation);
}
//�A�j���[�V�����C�x���g
void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}
