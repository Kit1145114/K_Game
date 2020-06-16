#include "stdafx.h"
#include "Titan.h"

//敵が作られて最初に呼ぶ処理。
Titan::Titan()
{
	m_se[0].Init(L"Assets/sound/BossAttack1.wav");				//音の初期化。
	Model.Init(L"Assets/modelData/RobbotEnemy1.cmo");		//モデルの呼び出し。
	//モデルのアニメーションのロード。
	animClip[esIdle].Load(L"Assets/animData/RE1_idle.tka");	//待機をロード。
	animClip[esIdle].SetLoopFlag(true);
	animClip[esTracking].Load(L"Assets/animData/RE1_walk.tka");	//歩きをロード。
	animClip[esTracking].SetLoopFlag(true);
	animClip[esAttack].Load(L"Assets/animData/RE1_ATK.tka");	//攻撃をロード。
	animClip[esAttack].SetLoopFlag(true);
	animClip[esDeath].Load(L"Assets/animData/RE1_death.tka");	//死亡をロード。
	animClip[esDeath].SetLoopFlag(false);
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
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 300;										//速さ。
	m_charaCon.Init(50.0f, 100.0f, m_position);		//判定の大きさ
	e_state = esIdle;									//最初なので待機。
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/RobbotEnemyAttack.efk");
}
//敵の攻撃処理。
void Titan::Attack()
{
	m_player->Damage(prm.ATK);
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
void Titan::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(GameTime().GetFrameDeltaTime());
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, {10.0f,10.0f,10.0f});
	m_charaCon.SetPosition(m_position);
}
//倒されたときに呼ぶ処理。
void Titan::Death()
{
	anim.Play(esDeath);
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
		//待機中。
	case Enemys::esIdle:
		Search();
		Rotation();
		anim.Play(esIdle);
		break;
		//追いかけてる。
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		anim.Play(esTracking);
		break;
		//攻撃。
	case Enemys::esAttack:
		Search();
		Rotation();
		anim.Play(esAttack);
		break;
		//死亡したとき。
	case Enemys::esDeath:
		Death();
	}
}
//エネミーが進む処理。
void Titan::EMove()
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
//アニメーションイベント
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	CVector3 dis;
	CVector3 diff;
	if (m_player->GetIsDead() == false) {
		if (e_state == esAttack && eventName)
		{
			Attack();
			m_se[0].Play(false);
			dis = (m_player->GetPosition() + m_position) * 0.5f;
			diff = m_player->GetPosition() - m_position;
			dis.y += 30.0f;
			//{ m_forward.x, 50.0f,m_forward.z };
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			g_effektEngine->SetPosition(m_playEffectHandle,/*m_player->GetPosition()*/dis);
			g_effektEngine->SetRotation(m_playEffectHandle, 0.0f, atan2( diff.x, diff.z), 0.0f);
		}
	}
}
//攻撃できるか
void Titan::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking)
	{
		//距離内に近づいたら攻撃。
		e_state = esAttack;
	}
}
