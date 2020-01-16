#include "stdafx.h"
#include "Golem.h"

//�G������čŏ��ɌĂԏ����B
Golem::Golem()
{
	gModel.Init(L"Assets/modelData/Enemy2.cmo");		//���f���̌Ăяo���B
//���f���̃A�j���[�V�����̃��[�h�B
	g_animClip[0].Load(L"Assets/animData/E2_idle.tka");	//�ҋ@�A�j���[�V���������[�h�B
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/E2_Death.tka");//���ʃA�j���[�V���������[�h�B
	g_animClip[1].SetLoopFlag(false);
	g_animClip[2].Load(L"Assets/animData/E2_ATK.tka");	//�U���A�j���[�V���������[�h�B
	g_animClip[2].SetLoopFlag(true);
	g_anim.Init(
		gModel,
		g_animClip,
		m_AnimClipNum
	);
	//�A�j���[�V�����C�x���g���ĂԁB
	g_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 200;										//�����B
	m_scale = { 5.0f,5.0f,5.0f };							//�傫��
	m_position = e_pos1;
	m_charaCon.Init(150.0f, 10.0f, m_position);			//����̑傫���B
	e_state = esIdle;									//�ŏ��ɑҋ@��ԁB
}
//�G�̍U�������B
void Golem::Attack()
{
	m_player->Damage(prm.ATK);
}
//DAMAGE�󂯂鏈��
void Golem::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//�v���C���[�̌����鏈���B
void Golem::Search()
{
	float Track = 0.0f;
	Move = m_player->GetPosition() - m_position;
	if (Move.Length() <= 1500.0f)
	{
		e_state = esTracking;
	}
	else if (Move.Length() >= 1500.0f)
	{
		e_state = esIdle;
		Move = CVector3::Zero();
	}
}
//�G�̍X�V���e�B
void Golem::Update()
{
	Draw();
	EnemyState();
	Rotation();
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	g_anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
	gModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//�G�̕`�揈���B
void Golem::Draw()
{
	gModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
//�|���ꂽ�Ƃ��ɌĂԏ����B
void Golem::Death()
{
	g_anim.Play(1);
	if (g_anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}
//�G�l�~�[���i�ޏ����B
void Golem::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//�G�l�~�[�̃A�j���[�V������Ԃŕς��Ă��
void Golem::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		g_anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		g_anim.Play(0);
		break;
	case Enemys::esAttack:
		g_anim.Play(2);
		break;
	case Enemys::esDeath:
		Death();
	}
}
//�G�l�~�[�̉�]�����B
void Golem::Rotation()
{
	float None = 0.0f;		//�}�W�b�N�i���o�[��h��0����ꂽ��
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	gModel.SetRotation(qRot);
	//�����A�����Ă������]������B
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		gModel.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		gModel.SetRotation(m_rotation);
	}
	gModel.SetRotation(m_rotation);
}
//�A�j���[�V�����C�x���g
void Golem::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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
