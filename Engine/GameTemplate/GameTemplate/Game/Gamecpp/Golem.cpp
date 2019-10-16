#include "stdafx.h"
#include "Golem.h"


Golem::Golem()
{
	gModel.Init(L"Assets/modelData/Enemy2.cmo");		//モデルの呼び出し。
//モデルのアニメーションのロード。
	//g_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//アニメーションをロード。
	//g_animClip[0].SetLoopFlag(true);
	g_anim.Init(
		gModel,

		g_animClip,
		3
	);
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 10;										//速さ。
	prm.model = gModel;
}

void Golem::Attack()
{
	player->SetDamage(m_ATK);
}

void Golem::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
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