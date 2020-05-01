#include "stdafx.h"
#include "Golem.h"

//敵が作られて最初に呼ぶ処理。
Golem::Golem()
{
	m_se[0].Init(L"Assets/sound/BossAttack1.wav");				//音の初期化。
	Model.Init(L"Assets/modelData/Enemy2.cmo");				//モデルの初期化。
//モデルのアニメーションのロード。
	animClip[esIdle].Load(L"Assets/animData/E2_idle.tka");	//待機アニメーションをロード。
	animClip[esIdle].SetLoopFlag(true);
	animClip[esTracking].Load(L"Assets/animData/E2_Walk.tka");//待機アニメーションをロード。
	animClip[esTracking].SetLoopFlag(true);
	animClip[esAttack].Load(L"Assets/animData/E2_ATK.tka");	//攻撃アニメーションをロード。
	animClip[esAttack].SetLoopFlag(true);
	animClip[esDeath].Load(L"Assets/animData/E2_Death.tka");	//死ぬアニメーションをロード。
	animClip[esDeath].SetLoopFlag(false);
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	//アニメーションイベントを呼ぶ。
	anim.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName)
	{
		OnAnimationEvent(clipName, eventName);
	});
	//フラグをtrueへ
	//パラメーター
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 200;										//速さ。
	m_scale = { 5.0f,5.0f,5.0f };							//大きさ
	m_position = e_pos1;
	m_charaCon.Init(150.0f, 500.0f, m_position);			//判定の大きさ。
	e_state = esIdle;									//最初に待機状態。
}
//敵の攻撃処理。
void Golem::Attack()
{
	m_player->Damage(prm.ATK);
}
//DAMAGE受ける処理
void Golem::Damage(int Damage)
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
	Enemys::ViewingAngle();
	//体力MAX時
	if (prm.HP == m_MaxHP) {
		//範囲外かつ視野角外なら
		if (m_diff.Length() >= m_enemytrack || fabsf(m_angle) > CMath::PI * 0.40f)
		{
			e_state = esIdle;
			isTracking = false;
		}
		//範囲内かつ視野角内なら
		else if (m_diff.Length() <= m_enemytrack && fabsf(m_angle) < CMath::PI * 0.40f)
		{
			Move = m_player->GetPosition() - m_position;
			isTracking = true;
			e_state = esTracking;
		}
	}
	//体力がMAXじゃないとき。ひたすら追いかける。
	else if (prm.HP < m_MaxHP)
	{
		Move = m_player->GetPosition() - m_position;
		isTracking = true;
		e_state = esTracking;
	}
	//攻撃の範囲計算。
	AttackRange();
}
//敵の更新内容。
void Golem::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	anim.Update(0.05f);
	m_charaCon.SetPosition(m_position);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//倒されたときに呼ぶ処理。
void Golem::Death()
{
	anim.Play(esDeath);
	if (anim.IsPlaying() == false)
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
	if (e_state == esIdle || e_state == esAttack) 
	{
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
	}
	else if (e_state == esTracking) {
		m_moveSpeed = Move * prm.SPD;
	}
}
//エネミーのアニメーション状態で変えてるよ
void Golem::EnemyState()
{
	switch (e_state)
	{
	case Enemys::esIdle:
		Search();
		anim.Play(esIdle);
		break;
	case Enemys::esTracking:
		Search();
		EMove();
		anim.Play(esTracking);
		break;
	case Enemys::esAttack:
		Search();
		EMove();
		anim.Play(esAttack);
		break;
	case Enemys::esDeath:
		Death();
	}
}
//アニメーションイベント
void Golem::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//if (m_player->GetIsDead() == false) {
	//	if (e_state == esAttack && eventName)
	//	{
	//		Attack();
	//		m_se[0].Play(false);
	//	}
	//}
	if (m_player->GetIsDead() == false) {
		CVector3 diff = m_position - m_player->GetPosition();
		if (diff.Length() <= m_Kyori && eventName)
		{
			Attack();
		}
	}
}
//攻撃できるか
void Golem::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking)
	{
		//距離内に近づいたら攻撃。
		e_state = esAttack;
	}
}