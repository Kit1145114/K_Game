#include "stdafx.h"
#include "Boss.h"

//�G������čŏ��ɌĂԏ����B
Boss::Boss()
{
	m_soundEngine.Init();
	m_se.Init(L"Assets/sound/BossAttack1.wav");
	//m_se[1].Init(L"Assets/sound/BossAttack2.wav");
	Model.Init(L"Assets/modelData/RobbotBoss.cmo");		//���f���̌Ăяo���B
	//���f���̃A�j���[�V�����̃��[�h�B
	animClip[bsIdle].Load(L"Assets/animData/RB_idle.tka");	//�ҋ@�����[�h�B
	animClip[bsIdle].SetLoopFlag(true);
	animClip[bsWalkTracking].Load(L"Assets/animData/RB_walk.tka");	//���������[�h�B
	animClip[bsWalkTracking].SetLoopFlag(true);
	animClip[bsFlyTracking].Load(L"Assets/animData/RB_FlyMove.tka");	//���������[�h�B
	animClip[bsFlyTracking].SetLoopFlag(true);
	animClip[bsDeath].Load(L"Assets/animData/RB_death.tka");	//���S�����[�h�B
	animClip[bsDeath].SetLoopFlag(false);
	animClip[bsSmallAttack].Load(L"Assets/animData/RB_shortATK.tka");	//�U�������[�h�B
	animClip[bsSmallAttack].SetLoopFlag(true);
	animClip[bsBigAttack].Load(L"Assets/animData/RB_BigATK.tka");	//�U�������[�h�B
	animClip[bsBigAttack].SetLoopFlag(true);
	animClip[bsHitMe].Load(L"Assets/animData/RB_HitDamage.tka");	//�U�������[�h�B
	animClip[bsHitMe].SetLoopFlag(false);
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
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 80;										//�U����
	prm.DEF = 80;										//�h���
	prm.SPD = 350;										//�����B
	m_scale = { 1.5f,1.5f,1.5f };						//�G�l�~�[�̑傫��
	m_charaCon.Init(100.0f, 600.0f,m_position);			//����̑傫��
	boss_State = bsIdle;								//�ŏ��Ȃ̂őҋ@�B
	Mode = SmallATK;									//���U�������邩�B
}
//�G�̍X�V���e�B
void Boss::Update()
{
	Draw();
	HitMe();
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
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//�G�̍U�������B
void Boss::Attack()
{

	m_player->Damage(prm.ATK);
}
//�U���ł���͈͂����ׂ鏈��
void Boss::AttackRange()
{
	if (diff.Length() <= attackDistance && Mode == SmallATK
		&& isTracking)
	{
		//�������ɋ߂Â�����U���B
		boss_State = bsSmallAttack;
	}
	else if (diff.Length() <= attackDistance && Mode == BigATK
		&& isTracking)
	{
		boss_State = bsBigAttack;
	}

}
//�G�l�~�[���i�ޏ����B
void Boss::EMove()
{
	Move.Normalize();
	if (boss_State == bsIdle)
	{
		m_moveSpeed = CVector3::Zero();
	}
	else if (boss_State == bsBigAttack || boss_State == bsSmallAttack)
	{
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
	}
	else if (boss_State == bsWalkTracking) {
		m_moveSpeed = Move * prm.SPD;
	}
	else if (boss_State == bsFlyTracking)
	{
		m_moveSpeed = Move * prm.SPD * 2;
	}
}
//DAMAGE�󂯂鏈��
void Boss::Damage(int Dam)
{
	prm.HP -= (Dam - prm.DEF);
}
//�v���C���[�̌����鏈���B
void Boss::Search()
{
	ViewingAngle();
	//�̗�MAX��
	if (prm.HP == m_MaxHP) {
		//�͈͊O������p�O�Ȃ�
		if (diff.Length() >= track || fabsf(angle) > CMath::PI * 0.40f)
		{
			boss_State = bsIdle;
			isTracking = false;	
		}
		//�͈͓�������p���Ȃ�
		else if (diff.Length() <= track && fabsf(angle) < CMath::PI * 0.40f)
		{
			Move = m_player->GetPosition() - m_position;
			isTracking = true;
			//��s�������Ȃ�
			if (diff.Length() >= flyDistance)
			{
				Move = m_player->GetPosition() - m_position;
				boss_State = bsFlyTracking;
			}
			//���s�������Ȃ�
			else if (diff.Length() >= walkingDistance)
			{
				Move = m_player->GetPosition() - m_position;
				boss_State = bsWalkTracking;
			}
		}
	}
	//�̗͂�MAX����Ȃ��Ƃ��B�Ђ�����ǂ�������B
	else if (prm.HP < m_MaxHP)
	{
		Move = m_player->GetPosition() - m_position;
		isTracking = true;
		//��s�������Ȃ�
		if (diff.Length() >= flyDistance)
		{
			Move = m_player->GetPosition() - m_position;
			boss_State = bsFlyTracking;
		}
		//���s�������Ȃ�
		else if (diff.Length() >= walkingDistance)
		{
			Move = m_player->GetPosition() - m_position;
			boss_State = bsWalkTracking;
		}
	}
	//�U���͈̔͌v�Z�B
	AttackRange();
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Boss::Death()
{
	anim.Play(bsDeath);
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
		EMove();
		Rotation();
		anim.Play(bsIdle);
		break;
	case Enemys::bsWalkTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(bsWalkTracking);
		break;
	case Enemys::bsFlyTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(bsFlyTracking);
	case Enemys::bsDeath:
		Death();
		break;
	case Enemys::bsSmallAttack:
		Search();
		EMove();
		Rotation();
		anim.Play(bsSmallAttack);
		break;
	case Enemys::bsBigAttack:
		Search();
		EMove();
		Rotation();
		anim.Play(bsBigAttack);
		break;
	case Enemys::bsHitMe:
		anim.Play(bsHitMe);
		break;
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
			&& eventName)
		{
			Attack();
			Mode = BigATK;
			m_se.Play(false);
		}
		else if (boss_State == bsBigAttack 
			&& eventName)
		{
			prm.ATK * 2;
			Attack();
			Mode = SmallATK;
			m_se.Play(false);
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
//DAMAGE�󂯂��Ƃ���...
void Boss::HitMe()
{
	if (isHitMe)
	{
		boss_State = bsHitMe;
		//�����AHP��0�ȉ��Ȃ玀�S�����B
		if (prm.HP <= 0.0f)
		{
			boss_State = bsDeath;
		}
		else if (!anim.IsPlaying() && prm.HP >= 0.0f)
		{
			isHitMe = false;
			boss_State = bsIdle;
		}
	}
}