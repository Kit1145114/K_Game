#include "stdafx.h"
#include "Titan.h"

//�G������čŏ��ɌĂԏ����B
Titan::Titan()
{
	Model.Init(L"Assets/modelData/RobbotEnemy1.cmo");		//���f���̌Ăяo���B
	//���f���̃A�j���[�V�����̃��[�h�B
	animClip[0].Load(L"Assets/animData/RE1_idle.tka");	//�ҋ@�����[�h�B
	animClip[0].SetLoopFlag(true);
	animClip[1].Load(L"Assets/animData/RE1_walk.tka");	//���������[�h�B
	animClip[1].SetLoopFlag(true);
	animClip[2].Load(L"Assets/animData/RE1_death.tka");	//���S�����[�h�B
	animClip[2].SetLoopFlag(false);
	animClip[3].Load(L"Assets/animData/RE1_ATK.tka");	//�U�������[�h�B
	animClip[3].SetLoopFlag(true);
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
	m_position = e_pos3;
	m_charaCon.Init(150.0f, 500.0f, m_position);			//����̑傫��
	e_state = esIdle;									//�ŏ��Ȃ̂őҋ@�B
}
//�G�̍U�������B
void Titan::Attack()
{
	m_player->Damage(m_ATK);
}
//DAMAGE�󂯂鏈��
void Titan::Damage(int Damage)
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
void Titan::Update()
{
	Draw();
	EnemyState();
	Rotation();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}
//�G�̕`�揈���B
void Titan::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Titan::Death()
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
void Titan::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		Rotation();
		anim.Play(0);
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
		anim.Play(3);
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
