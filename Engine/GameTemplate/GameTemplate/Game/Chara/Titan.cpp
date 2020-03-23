#include "stdafx.h"
#include "Titan.h"

//敵が作られて最初に呼ぶ処理。
Titan::Titan()
{
	Model.Init(L"Assets/modelData/RobbotEnemy1.cmo");		//モデルの呼び出し。
	//モデルのアニメーションのロード。
	animClip[0].Load(L"Assets/animData/RE1_idle.tka");	//待機をロード。
	animClip[0].SetLoopFlag(true);
	animClip[1].Load(L"Assets/animData/RE1_walk.tka");	//歩きをロード。
	animClip[1].SetLoopFlag(true);
	animClip[2].Load(L"Assets/animData/RE1_death.tka");	//死亡をロード。
	animClip[2].SetLoopFlag(false);
	animClip[3].Load(L"Assets/animData/RE1_ATK.tka");	//攻撃をロード。
	animClip[3].SetLoopFlag(true);
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
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 300;										//速さ。
	m_scale = { 30.0f,30.0f,30.0f };					//エネミーの大きさ
	m_position = e_pos3;
	m_charaCon.Init(150.0f, 500.0f, m_position);			//判定の大きさ
	e_state = esIdle;									//最初なので待機。
}
//敵の攻撃処理。
void Titan::Attack()
{
	m_player->Damage(m_ATK);
}
//DAMAGE受ける処理
void Titan::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}
//プレイヤーの見つける処理。
void Titan::Search()
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
void Titan::Update()
{
	Draw();
	EnemyState();
	Rotation();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}
//敵の描画処理。
void Titan::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}
//倒されたときに呼ぶ処理。
void Titan::Death()
{
	anim.Play(2);
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}
//エネミーのアニメーション状態で変えてるよ
void Titan::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		Rotation();
		anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(1);
		break;
	case Enemys::esAttack:
		Search();
		Rotation();
		anim.Play(3);
		break;
	case Enemys::esDeath:
		Death();
	}
}
//エネミーが進む処理。
void Titan::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//エネミーの回転処理。
void Titan::Rotation()
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
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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
