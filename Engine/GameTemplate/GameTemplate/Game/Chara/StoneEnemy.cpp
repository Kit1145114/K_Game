#include "stdafx.h"
#include "StoneEnemy.h"

//敵が作られて最初に呼ぶ処理。
StoneEnemy::StoneEnemy()
{ 
	Model.Init(L"Assets/modelData/Enemy3.cmo");		//モデルの呼び出し。
	//モデルのアニメーションのロード。
	animClip[0].Load(L"Assets/animData/E3_idle.tka");	//アニメーションをロード。
	animClip[0].SetLoopFlag(true);
	animClip[1].Load(L"Assets/animData/E3_walk.tka");	//アニメーションをロード。
	animClip[1].SetLoopFlag(true);
	animClip[2].Load(L"Assets/animData/E3_ATK.tka");	//アニメーションをロード。
	animClip[2].SetLoopFlag(true);
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 20;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 400;										//速さ。
	m_scale = { 3.0f,3.0f,3.0f };						//モデルの大きさ。
	//m_position = e_pos2;
	m_charaCon.Init(150.0f, 400.0f, m_position, enCollisionAttr_Enemy);			//判定の大きさ
	e_state = esIdle;									//最初なので待機。
}
//敵の攻撃処理。
void StoneEnemy::Attack()
{
	m_player->Damage(prm.ATK);
}
//DAMAGE受ける処理
void StoneEnemy::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		Death();
	}
}
//プレイヤーの見つける処理。
void StoneEnemy::Search()
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
//敵の更新内容。
void StoneEnemy::Update()
{
	Draw();
	EnemyState();
	Rotation();
	anim.Play(0);
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position,m_rotation, m_scale);
	anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
}
//敵の描画処理。
void StoneEnemy::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//倒されたときに呼ぶ処理。
void StoneEnemy::Death()
{
	this->SetActive(false);
	m_charaCon.RemoveRigidBoby();
	isDeath = true;
}
//エネミーが進む処理。
void StoneEnemy::EMove() 
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//エネミーのアニメーション状態で変えてるよ
void StoneEnemy::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		anim.Play(1);
		break;
	case Enemys::esAttack:
		break;
	}
}
//エネミーの回転処理。
void StoneEnemy::Rotation()
{
	float None = 0.0f;		//マジックナンバーを防ぐ0を入れた数
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	Model.SetRotation(qRot);
	//もし、動いていたら回転させる。
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
//アニメーションイベント
void StoneEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (m_player->GetIsDead() == false) {
		CVector3 diff = m_position - m_player->GetPosition();
		if (diff.Length() <= m_Kyori && eventName)
		{
			Attack();
		}
	}
}