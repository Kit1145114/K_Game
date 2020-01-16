#include "stdafx.h"
#include "Golem.h"

//敵が作られて最初に呼ぶ処理。
Golem::Golem()
{
	gModel.Init(L"Assets/modelData/Enemy2.cmo");		//モデルの呼び出し。
//モデルのアニメーションのロード。
	g_animClip[0].Load(L"Assets/animData/E2_idle.tka");	//待機アニメーションをロード。
	g_animClip[0].SetLoopFlag(true);
	g_animClip[1].Load(L"Assets/animData/E2_Death.tka");//死ぬアニメーションをロード。
	g_animClip[1].SetLoopFlag(false);
	g_animClip[2].Load(L"Assets/animData/E2_ATK.tka");	//攻撃アニメーションをロード。
	g_animClip[2].SetLoopFlag(true);
	g_anim.Init(
		gModel,
		g_animClip,
		m_AnimClipNum
	);
	//アニメーションイベントを呼ぶ。
	g_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 200;										//速さ。
	m_scale = { 5.0f,5.0f,5.0f };							//大きさ
	m_position = e_pos1;
	m_charaCon.Init(150.0f, 10.0f, m_position);			//判定の大きさ。
	e_state = esIdle;									//最初に待機状態。
}
//敵の攻撃処理。
void Golem::Attack()
{
	m_player->Damage(prm.ATK);
}
//DAMAGE受ける処理
void Golem::Damage(float Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//プレイヤーの見つける処理。
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
//敵の更新内容。
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
//敵の描画処理。
void Golem::Draw()
{
	gModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
//倒されたときに呼ぶ処理。
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
//エネミーが進む処理。
void Golem::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//エネミーのアニメーション状態で変えてるよ
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
//エネミーの回転処理。
void Golem::Rotation()
{
	float None = 0.0f;		//マジックナンバーを防ぐ0を入れた数
	float Rot = atan2(Move.x, Move.z);
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisY(), Rot);
	gModel.SetRotation(qRot);
	//もし、動いていたら回転させる。
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
//アニメーションイベント
void Golem::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	float Kyori = 500.0f;
	if (m_player->GetIsDead() == false) {
		CVector3 diff = m_position - m_player->GetPosition();
		if (diff.Length() <= Kyori && eventName)
		{
			//MessageBox(NULL, TEXT("Hit114514"), TEXT("めっせ"), MB_OK);
			m_player->Damage(prm.ATK);
		}
	}
}
