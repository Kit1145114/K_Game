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
	prm.ATK = 80;										//�U����
	prm.DEF = 40;										//�h���
	prm.SPD = 350;										//�����B
	m_scale = { 1.5f,1.5f,1.5f };						//�G�l�~�[�̑傫��
	m_charaCon.Init(100.0f, 600.0f, m_position);		//����̑傫��
	boss_State = bsIdle;								//�ŏ��Ȃ̂őҋ@�B
	Mode = SmallATK;									//���U�������邩�B
}
//�G�̍X�V���e�B
void Boss::Update()
{
	Draw();
	VectorAcquisition();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
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
	m_player->Damage(prm.ATK);
	if (Mode == SmallATK)
	{
		Mode = BigATK;
	}
	else if(Mode == BigATK)
	{
		Mode = SmallATK;
	}
}
//�G�l�~�[���i�ޏ����B
void Boss::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//DAMAGE�󂯂鏈��
void Boss::Damage(int Dam)
{
	prm.HP -= (Dam - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		boss_State = bsDeath;
	}
}
//�v���C���[�̌����鏈���B
void Boss::Search()
{
	ViewingAngle();
	//����������������p���Ȃ�B
	if (diff.Length() <= Track && fabsf(angle) < CMath::PI * 0.25f)
	{
		//�ǂ�������B
		Move = m_player->GetPosition() - m_position;
		boss_State = bsTracking;
		if (diff.Length() <= Kyori && Mode == SmallATK)
		{
			//�������ɋ߂Â�����U���B
			m_moveSpeed.x = ZERO;
			m_moveSpeed.z = ZERO;
			boss_State = bsSmallAttack;
		}
		else if (diff.Length() <= Kyori && Mode == BigATK)
		{
			m_moveSpeed.x = ZERO;
			m_moveSpeed.z = ZERO;
			boss_State = bsBigAttack;
		}
	}
	else if (diff.Length() >= Track)
	{
		boss_State = bsIdle;
		Move = CVector3::Zero();
	}
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Boss::Death()
{
	anim.Play(2);
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
	switch (boss_State)
	{
	case Enemys::bsIdle:
		Search();
		Rotation();
		anim.Play(0);
		break;
	case Enemys::bsTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(1);
		break;
	case Enemys::bsDeath:
		Death();
		break;
	case Enemys::bsSmallAttack:
		Search();
		Rotation();
		anim.Play(3);
		break;
	case Enemys::bsBigAttack:
		Search();
		Rotation();
		anim.Play(4);
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
	if (m_player->GetIsDead() == false) {
		if (boss_State == bsSmallAttack 
			&& eventName
			&& Move.Length() <= Kyori)
		{
			Attack();
		}
		else if (boss_State == bsBigAttack 
			&& eventName
			&& Move.Length() <= Kyori)
		{
			prm.ATK * 2;
			Attack();
		}
	}
}
//�G�l�~�[�̎���p
void Boss::ViewingAngle()
{
	diff = m_player->GetPosition() - m_position;
	m_toPlayer = m_player->GetPosition() - m_position;
	m_toPlayer.Normalize();
	angle = acosf(m_toPlayer.Dot(m_forward));
}
//�G�l�~�[�̃x�N�g�����擾�B
void Boss::VectorAcquisition()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}