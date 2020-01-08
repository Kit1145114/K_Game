#include "stdafx.h"
#include "Titan.h"

//�G������čŏ��ɌĂԏ����B
Titan::Titan()
{
	tModel.Init(L"Assets/modelData/RobbotEnemy1.cmo");		//���f���̌Ăяo���B
//���f���̃A�j���[�V�����̃��[�h�B
	t_animClip[0].Load(L"Assets/animData/RE1_idle.tka");	//�ҋ@�����[�h�B
	t_animClip[0].SetLoopFlag(true);
	t_animClip[1].Load(L"Assets/animData/RE1_walk.tka");	//���������[�h�B
	t_animClip[1].SetLoopFlag(true);
	t_animClip[2].Load(L"Assets/animData/RE1_death.tka");	//���S�����[�h�B
	t_animClip[2].SetLoopFlag(false);
	t_animClip[3].Load(L"Assets/animData/RE1_ATK.tka");	//�U�������[�h�B
	t_animClip[3].SetLoopFlag(true);
	t_anim.Init(
		tModel,
		t_animClip,
		m_AnimClipNum
	);
	t_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 300;										//�����B
	m_position = e3_pos;
	m_charaCon.Init(150.0f, 10.0f, m_position);
	e_state = esIdle;
}
//�G�̍U�������B
void Titan::Attack()
{
	//player->SetDamage(m_ATK);
}
//DAMAGE�󂯂鏈��
void Titan::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//�v���C���[�̌����鏈���B
void Titan::Search()
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
//�G�̍X�V���e�B
void Titan::Update()
{
	Draw();
	EnemyState();
	Rotation();
	m_moveSpeed.y -= gravity;
	t_anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	tModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}
//�G�̕`�揈���B
void Titan::Draw()
{
	tModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Titan::Death()
{
	t_anim.Play(2);
	if (t_anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}
//�G�l�~�[�̃A�j���[�V������Ԃŕς��Ă��
void Titan::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		Rotation();
		t_anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		t_anim.Play(1);
		break;
	case Enemys::esAttack:
		Search();
		Rotation();
		t_anim.Play(3);
		break;
	case Enemys::esDeath:
		Death();
	}
}
//�G�l�~�[���i�ޏ����B
void Titan::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//�G�l�~�[�̉�]�����B
void Titan::Rotation()
{
	float None = 0.0f;		//�}�W�b�N�i���o�[��h��0����ꂽ��
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	tModel.SetRotation(qRot);
	//�����A�����Ă������]������B
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		tModel.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		tModel.SetRotation(m_rotation);
	}
	tModel.SetRotation(m_rotation);
}
//�A�j���[�V�����C�x���g
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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
