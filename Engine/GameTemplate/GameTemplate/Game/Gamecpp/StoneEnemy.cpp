#include "stdafx.h"
#include "StoneEnemy.h"


StoneEnemy::StoneEnemy()
{ 
	seModel.Init(L"Assets/modelData/Enemy3.cmo");		//���f���̌Ăяo���B
	//���f���̃A�j���[�V�����̃��[�h�B
	s_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//�A�j���[�V���������[�h�B
	s_animClip[0].SetLoopFlag(true);
	s_anim.Init(
		seModel,

		s_animClip,
		3
	);
	//�t���O��true��
	//�p�����[�^�[
	prm.HP = 100;										//HP
	prm.ATK = 60;										//�U����
	prm.DEF = 30;										//�h���
	prm.SPD = 10;										//�����B
	prm.model = seModel;
}

void StoneEnemy::Attack()
{
	player->SetDamage(m_ATK);
}

void StoneEnemy::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//�����AHP��0�ȉ��Ȃ玀�S�����B
	if (prm.HP <= 0.0f)
	{
		Death();
	}
}

void StoneEnemy::Search()
{
	//player = FindGO<Player>();
}

void StoneEnemy::Update()
{
	Draw();
	s_anim.Play(0);
	seModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
}

void StoneEnemy::Draw()
{
	seModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
