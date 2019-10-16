#include "stdafx.h"
#include "Golem.h"


Golem::Golem()
{
	gModel.Init(L"Assets/modelData/Enemy2.cmo");		//���f���̌Ăяo���B
//���f���̃A�j���[�V�����̃��[�h�B
	//g_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//�A�j���[�V���������[�h�B
	//g_animClip[0].SetLoopFlag(true);
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
	prm.model = gModel;
}

void Golem::Attack()
{
	player->SetDamage(m_ATK);
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
	gModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
}

void Golem::Draw()
{
	gModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}