#include "stdafx.h"
#include "Boss.h"

//敵が作られて最初に呼ぶ処理。
Boss::Boss()
{
	Model.Init(L"Assets/modelData/RobbotBoss.cmo");		//モデルの呼び出し。
	//モデルのアニメーションのロード。
	animClip[0].Load(L"Assets/animData/RB_idle.tka");	//待機をロード。
	animClip[0].SetLoopFlag(true);
	animClip[1].Load(L"Assets/animData/RE1_walk.tka");	//歩きをロード。
	animClip[1].SetLoopFlag(true);
	animClip[2].Load(L"Assets/animData/RB_death.tka");	//死亡をロード。
	animClip[2].SetLoopFlag(false);
	animClip[3].Load(L"Assets/animData/RB_shortATK.tka");	//攻撃をロード。
	animClip[3].SetLoopFlag(true);
	animClip[4].Load(L"Assets/animData/RB_BigATK.tka");	//攻撃をロード。
	animClip[4].SetLoopFlag(true);
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
	prm.ATK = 80;										//攻撃力
	prm.DEF = 40;										//防御力
	prm.SPD = 350;										//速さ。
	m_scale = { 1.5f,1.5f,1.5f };						//エネミーの大きさ
	m_charaCon.Init(100.0f, 600.0f, m_position);		//判定の大きさ
	boss_State = bsIdle;								//最初なので待機。
	Mode = SmallATK;									//何攻撃をするか。
}
//敵の更新内容。
void Boss::Update()
{
	Draw();
	VectorAcquisition();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}
//敵の描画処理。
void Boss::Draw()
{
	Model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
//敵の攻撃処理。
void Boss::Attack()
{
	m_player->Damage(prm.ATK);
	if (Mode == SmallATK)
	{
		Mode = BigATK;
	}
	else if(Mode == BigATK)
	{
		Mode = SmallATK;
	}
}
//エネミーが進む処理。
void Boss::EMove()
{
	Move.Normalize();
	m_moveSpeed = Move * prm.SPD;
}
//DAMAGE受ける処理
void Boss::Damage(int Dam)
{
	prm.HP -= (Dam - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		boss_State = bsDeath;
	}
}
//プレイヤーの見つける処理。
void Boss::Search()
{
	ViewingAngle();
	//もし距離内かつ視野角内なら。
	if (diff.Length() <= Track && fabsf(angle) < CMath::PI * 0.25f)
	{
		//追いかける。
		Move = m_player->GetPosition() - m_position;
		boss_State = bsTracking;
		if (diff.Length() <= Kyori && Mode == SmallATK)
		{
			//距離内に近づいたら攻撃。
			m_moveSpeed.x = ZERO;
			m_moveSpeed.z = ZERO;
			boss_State = bsSmallAttack;
		}
		else if (diff.Length() <= Kyori && Mode == BigATK)
		{
			m_moveSpeed.x = ZERO;
			m_moveSpeed.z = ZERO;
			boss_State = bsBigAttack;
		}
	}
	else if (diff.Length() >= Track)
	{
		boss_State = bsIdle;
		Move = CVector3::Zero();
	}
}
//倒されたときに呼ぶ処理。
void Boss::Death()
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
void Boss::EnemyState()
{
	switch (boss_State)
	{
	case Enemys::bsIdle:
		Search();
		Rotation();
		anim.Play(0);
		break;
	case Enemys::bsTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(1);
		break;
	case Enemys::bsDeath:
		Death();
		break;
	case Enemys::bsSmallAttack:
		Search();
		Rotation();
		anim.Play(3);
		break;
	case Enemys::bsBigAttack:
		Search();
		Rotation();
		anim.Play(4);
	}
}
//エネミーの回転処理。
void Boss::Rotation()
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
void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (m_player->GetIsDead() == false) {
		if (boss_State == bsSmallAttack 
			&& eventName
			&& Move.Length() <= Kyori)
		{
			Attack();
		}
		else if (boss_State == bsBigAttack 
			&& eventName
			&& Move.Length() <= Kyori)
		{
			prm.ATK * 2;
			Attack();
		}
	}
}
//エネミーの視野角
void Boss::ViewingAngle()
{
	diff = m_player->GetPosition() - m_position;
	m_toPlayer = m_player->GetPosition() - m_position;
	m_toPlayer.Normalize();
	angle = acosf(m_toPlayer.Dot(m_forward));
}
//エネミーのベクトルを取得。
void Boss::VectorAcquisition()
{
	auto mRot = CMatrix::Identity();
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = mRot.m[2][0];
	m_forward.y = mRot.m[2][1];
	m_forward.z = mRot.m[2][2];
}