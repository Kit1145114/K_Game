#include "stdafx.h"
#include "Titan.h"

//敵が作られて最初に呼ぶ処理。
Titan::Titan()
{
	tModel.Init(L"Assets/modelData/RobbotEnemy1.cmo");		//モデルの呼び出し。
//モデルのアニメーションのロード。
	t_animClip[0].Load(L"Assets/animData/RE1_idle.tka");	//待機をロード。
	t_animClip[0].SetLoopFlag(true);
	t_animClip[1].Load(L"Assets/animData/RE1_walk.tka");	//歩きをロード。
	t_animClip[1].SetLoopFlag(true);
	t_animClip[2].Load(L"Assets/animData/RE1_death.tka");	//死亡をロード。
	t_animClip[2].SetLoopFlag(false);
	t_animClip[3].Load(L"Assets/animData/RE1_ATK.tka");	//攻撃をロード。
	t_animClip[3].SetLoopFlag(true);
	t_anim.Init(
		tModel,
		t_animClip,
		m_AnimClipNum
	);
	t_anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	prm.ATK = 60;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 300;										//速さ。
	m_position = e3_pos;
	m_charaCon.Init(150.0f, 10.0f, m_position);
	e_state = esIdle;
}
//敵の攻撃処理。
void Titan::Attack()
{
	//player->SetDamage(m_ATK);
}
//DAMAGE受ける処理
void Titan::Damage(float Damage)
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
	Move = m_player->GetPosition() - m_position;
	if (Move.Length() <= Track)
	{
		e_state = esTracking;
		if (Move.Length() <= 200.0f)
		{
			e_state = esAttack;
		}
	}
	else if (Move.Length() >= Track)
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
	t_anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	tModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}
//敵の描画処理。
void Titan::Draw()
{
	tModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
//倒されたときに呼ぶ処理。
void Titan::Death()
{
	t_anim.Play(2);
	if (t_anim.IsPlaying() == false)
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
		t_anim.Play(0);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		t_anim.Play(1);
		break;
	case Enemys::esAttack:
		Search();
		Rotation();
		t_anim.Play(3);
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
	tModel.SetRotation(qRot);
	//もし、動いていたら回転させる。
	if (m_moveSpeed.x != None || m_moveSpeed.z != None)
	{
		m_rotation = qRot;
		tModel.SetRotation(m_rotation);
	}
	if (m_moveSpeed.x == None && m_moveSpeed.z == None)
	{
		tModel.SetRotation(m_rotation);
	}
	tModel.SetRotation(m_rotation);
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
