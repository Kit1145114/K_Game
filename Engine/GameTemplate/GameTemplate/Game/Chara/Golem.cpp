#include "stdafx.h"
#include "Golem.h"

//敵が作られて最初に呼ぶ処理。
Golem::Golem()
{					//音の初期化。
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
	prm.HP = 150;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 200;										//速さ。
	m_scale = { 3.0f,3.0f,3.0f };						//大きさ
	m_charaCon.Init(50.0f, 200.0f, m_position, enCollisionAttr_Enemy);			//判定の大きさ。
	e_state = esIdle;									//最初に待機状態。
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Wind.efk");
	m_se[0].Init(L"Assets/sound/wind.wav");				//攻撃
	m_se[1].Init(L"Assets/sound/Enemy2_walk.wav");			//歩く。
}
Golem::~Golem()
{
	g_effektEngine->Stop(m_playEffectHandle);
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
	if (prm.HP == m_MaxHP && !isTrack_flag) {
		//範囲外かつ視野角外ならかつ、一度でも見つけてないとき。
		if (m_diff.Length() >= m_enemyTrack || fabsf(m_angle) > CMath::PI * m_fowndAngle)
		{
			e_state = esIdle;
		}
		//範囲内かつ視野角内なら
		else if (m_diff.Length() <= m_enemyTrack && fabsf(m_angle) < CMath::PI * m_fowndAngle)
		{
			m_move = m_player->GetPosition() - m_position;
			e_state = esTracking;
			isAttack_flag = true;
			isTrack_flag = true;
		}
	}
	//体力がMAXじゃないとき。ひたすら追いかける。
	else if (prm.HP < m_MaxHP || isTrack_flag)
	{
		m_move = m_player->GetPosition() - m_position;
		isAttack_flag = true;
		isTrack_flag = true;
		e_state = esTracking;
	}
	//攻撃の範囲計算。
	AttackRange();
}
//敵の更新内容。
void Golem::Update()
{
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
	if (m_se[0].IsPlaying())
	{
		m_se[0].Stop();
		g_effektEngine->Stop(m_playEffectHandle);
	}
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath_flag = true;
	}
}
//エネミーが進む処理。
void Golem::EMove()
{
	m_move.Normalize();
	//状態で移動するかを決める。
	switch (e_state)
	{
	case Enemys::esIdle:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		break;
	case Enemys::esAttack:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		break;
	case Enemys::esTracking:
		m_moveSpeed = m_move * prm.SPD;
		m_se[1].Play(true);
		break;
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
	//一度攻撃した後、常に広範囲で特定する。
	m_attackDistance = 6000.0f;
	if (m_diff.Length() <= m_attackDistance && isAttack_flag && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//距離内に近づいたら攻撃。
		m_moveSpeed = CVector3::Zero();
		e_state = esStandbyAttack;
	}
}
//攻撃までの
void Golem::AttackStanby()
{
	CVector3 pos = m_position;
	//一度だけプレイヤーまでの距離を把握したいため、フラグを立てます。
	if (isDestination_flag) {
		m_destination.x = m_player->GetPosition().x - m_position.x;
		m_destination.z = m_player->GetPosition().z - m_position.z;
		m_se[1].Stop();
		isDestination_flag = false;
	}
	//時間になったらエフェクトの生成。
	if (m_timer >= m_effectSpwanTime)
	{
		//エフェクトを大量生成しないために一度だけ呼び出す。
		if (isEffectLoop_flag) {
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			m_se[0].Play(false);
			isEffectLoop_flag = false;
		}
		//移動中は常に攻撃状態のため、エフェクト等を追従させている。
		m_moveSpeed = m_destination * 2.0f;
		m_attackTime += GameTime().GetFrameDeltaTime();
		e_state = esAttack;
		anim.Play(esAttack);
		g_effektEngine->SetPosition(m_playEffectHandle, pos);
		m_attackDis = m_position - m_player->GetPosition();
		//範囲内にいる間常にプレイヤーへダメージ。
		if (m_attackDis.Length() <= m_attackDeistance)
		{
			m_player->Damage(m_playerToDamage);
		}
		//攻撃するに伴って
		if (m_attackTime >= m_goAttackTime)
		{
			e_state = esAttackGap;
			m_timer = ZERO;
			m_attackTime = ZERO;
			isEffectLoop_flag = true;
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
	m_falltimer += GameTime().GetFrameDeltaTime();
	m_moveSpeed = CVector3::Zero();
	//時間になったらすべてをリセットして倒れる。
	if (m_falltimer >= m_fallLimitTime)
	{
		e_state = esIdle;
		//ここでフラグも戻します。
		isDestination_flag = true;
		m_falltimer = ZERO;
	}
}