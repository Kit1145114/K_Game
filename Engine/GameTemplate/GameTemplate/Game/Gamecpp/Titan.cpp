#include "stdafx.h"
#include "Titan.h"


Titan::Titan()
{
	tModel.Init(L"Assets/modelData/Enemy1.cmo");		//���f���̌Ăяo���B
//���f���̃A�j���[�V�����̃��[�h�B
	t_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//�A�j���[�V���������[�h�B
	t_animClip[0].SetLoopFlag(true);
	t_anim.Init(
		tModel,

		t_animClip,
		3
	);
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 10;										//�����B
//	prm.model = tModel;
	m_position = e3_pos;
	m_charaCon.Init(150.0f, 10.0f, m_position);
}

void Titan::Attack()
{
	//player->SetDamage(m_ATK);
}

void Titan::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		Death();
	}
}

void Titan::Search()
{
	//player = FindGO<Player>();
}

void Titan::Update()
{
	Draw();
	t_anim.Play(0);
	m_moveSpeed.y -= 10.0f;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//t_anim.Update(0.2f);
	tModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	m_charaCon.SetPosition(m_position);
}

void Titan::Draw()
{
	tModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}