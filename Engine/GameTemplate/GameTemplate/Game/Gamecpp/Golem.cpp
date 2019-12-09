#include "stdafx.h"
#include "Golem.h"


Golem::Golem()
{
	gModel.Init(L"Assets/modelData/Enemy2.cmo");		//���f���̌Ăяo���B
//���f���̃A�j���[�V�����̃��[�h�B
	g_animClip[0].Load(L"Assets/animData/E2_idle.tka");	//�A�j���[�V���������[�h�B
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/E2_Death.tka");	//�A�j���[�V���������[�h�B
	g_animClip[1].SetLoopFlag(false);
	g_animClip[2].Load(L"Assets/animData/E2_ATK.tka");	//�A�j���[�V���������[�h�B
	g_animClip[2].SetLoopFlag(true);
	g_anim.Init(
		gModel,

		g_animClip,
		3
	);
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 10;										//�����B
//	prm.model = gModel;
	m_position = e2_pos;
	m_charaCon.Init(150.0f, 10.0f, m_position);
}

void Golem::Attack()
{
	m_player->SetDamage(m_ATK);
}

void Golem::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		Death();
	}
}

void Golem::Search()
{
	//player = FindGO<Player>();
}

void Golem::Update()
{
	Draw();
	g_anim.Play(0);
	m_moveSpeed.y -= 10.0f;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	g_anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
	gModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), scale);
}

void Golem::Draw()
{
	gModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}