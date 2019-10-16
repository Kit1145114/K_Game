#include "stdafx.h"
#include "StoneEnemy.h"


StoneEnemy::StoneEnemy()
{ 
	seModel.Init(L"Assets/modelData/Enemy3.cmo");		//モデルの呼び出し。
	//モデルのアニメーションのロード。
	s_animClip[0].Load(L"Assets/animData/E3_idle.tka");	//アニメーションをロード。
	s_animClip[0].SetLoopFlag(true);
	s_anim.Init(
		seModel,

		s_animClip,
		3
	);
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 10;										//速さ。
	prm.model = seModel;
}

void StoneEnemy::Attack()
{
	player->SetDamage(m_ATK);
}

void StoneEnemy::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
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
