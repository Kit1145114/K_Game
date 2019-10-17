#include "stdafx.h"
#include "Titan.h"


Titan::Titan()
{
	tModel.Init(L"Assets/modelData/Enemy1.cmo");		//モデルの呼び出し。
//モデルのアニメーションのロード。
	//t_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//アニメーションをロード。
	//t_animClip[0].SetLoopFlag(true);
	t_anim.Init(
		tModel,

		t_animClip,
		3
	);
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 10;										//速さ。
	prm.model = tModel;
	m_position = e3_pos;
}

void Titan::Attack()
{
	//player->SetDamage(m_ATK);
}

void Titan::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
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
	//m_position.y -= 10.0f;
	tModel.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
}

void Titan::Draw()
{
	tModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}