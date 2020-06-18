#include "stdafx.h"
#include "Golem.h"

//敵が作られて最初に呼ぶ処理。
Golem::Golem()
{
	m_se[0].Init(L"Assets/sound/BossAttack1.wav");						//音の初期化。
	Model.Init(L"Assets/modelData/Enemy2.cmo");							//モデルの初期化。
	//モデルのアニメーションのロード。
	animClip[esIdle].Load(L"Assets/animData/E2_idle.tka");				//待機アニメーションをロード。
	animClip[esIdle].SetLoopFlag(true);									//ループするのでtrue
	animClip[esTracking].Load(L"Assets/animData/E2_Walk.tka");			//待機アニメーションをロード。
	animClip[esTracking].SetLoopFlag(true);								//ループするのでtrue
	animClip[esAttack].Load(L"Assets/animData/E2_ATK.tka");				//攻撃アニメーションをロード。
	animClip[esAttack].SetLoopFlag(true);								//ループするのでtrue
	animClip[esDeath].Load(L"Assets/animData/E2_Death.tka");			//死ぬアニメーションをロード。
	animClip[esAttackGap].Load(L"Assets/animData/E2_Fall.tka");			//攻撃できる隙のアニメーションをロード
	animClip[esAttackGap].SetLoopFlag(false);							//ループしないのでfalse
	animClip[esStandbyAttack].Load(L"Assets/animData/E2_Standby.tka");	//攻撃のための部分をロード。
	animClip[esStandbyAttack].SetLoopFlag(false);						//ループしないのでfalse
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
	m_scale = { 3.0f,3.0f,3.0f };						//大きさ
	m_charaCon.Init(50.0f, 200.0f, m_position, enCollisionAttr_Enemy);			//判定の大きさ。
	e_state = esIdle;									//最初に待機状態。
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Wind.efk");
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
	if (prm.HP == m_MaxHP && !isTrackflag) {
		//範囲外かつ視野角外ならかつ、一度でも見つけてないとき。
		if (m_diff.Length() >= m_enemytrack || fabsf(m_angle) > CMath::PI * 0.40f)
		{
			e_state = esIdle;
		}
		//範囲内かつ視野角内なら
		else if (m_diff.Length() <= m_enemytrack && fabsf(m_angle) < CMath::PI * 0.40f)
		{
			Move = m_player->GetPosition() - m_position;
			isTracking = true;
			e_state = esTracking;
			isTrackflag = true;
		}
	}
	//体力がMAXじゃないとき。ひたすら追いかける。
	else if (prm.HP < m_MaxHP || isTrackflag)
	{
		Move = m_player->GetPosition() - m_position;
		isTracking = true;
		isTrackflag = true;
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
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	anim.Update(GameTime().GetFrameDeltaTime());
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
		AttackStanby();
		break;
	case Enemys::esDeath:
		Death();
		break;
	case Enemys::esAttackGap:
		anim.Play(esAttackGap);
		AttackGap();
		break;
	case Enemys::esStandbyAttack:
		AttackStanby();
		break;
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
	attackDistance = 400.0f;
	if (m_diff.Length() <= attackDistance && isTracking && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//距離内に近づいたら攻撃。
		m_moveSpeed = CVector3::Zero();
		e_state = esStandbyAttack;
	}
}
//攻撃までの
void Golem::AttackStanby()
{
	float m_goAttack = 1.0f;
	float LimitTime = 3.0f;
	CVector3 pos = m_position;
	//一度だけプレイヤーまでの距離を把握したいため、フラグを立てます。
	if (isDestinationflag) {
		Destination.x = m_player->GetPosition().x - m_position.x;
		Destination.z = m_player->GetPosition().z - m_position.z;
		isDestinationflag = false;
	}
	//時間になったら攻撃に代わる。
	if (m_timer >= m_goAttack)
	{
		if (loop) {
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			loop = false;
		}
		m_moveSpeed = Destination * 2.0f;
		m_AttackTime += GameTime().GetFrameDeltaTime();
		e_state = esAttack;
		anim.Play(esAttack);
		g_effektEngine->SetPosition(m_playEffectHandle, pos);
		m_Attackdis = m_position - m_player->GetPosition();
		if (m_Attackdis.Length() <= m_attack)
		{
			m_player->Damage(51);
		}
		if (m_AttackTime >= LimitTime)
		{
			e_state = esAttackGap;
			m_timer = ZERO;
			m_AttackTime = ZERO;
			loop = true;
		}
	}
	else {
		//タイマーを回します。子のタイマーは敵が攻撃するまでの時間
		m_timer += GameTime().GetFrameDeltaTime();
		anim.Play(esStandbyAttack);
	}
}
//攻撃できる隙
void Golem::AttackGap()
{
	//時間まで倒れてます
	g_effektEngine->Stop(m_playEffectHandle);
	float StopLimit = 2.0f;
	m_Falltimer += GameTime().GetFrameDeltaTime();
	m_moveSpeed = CVector3::Zero();
	if (m_Falltimer >= StopLimit)
	{
		e_state = esIdle;
		//ここでフラグも戻します。
		isDestinationflag = true;
		m_Falltimer = ZERO;
	}
}