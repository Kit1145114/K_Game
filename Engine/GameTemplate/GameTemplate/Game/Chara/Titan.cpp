#include "stdafx.h"
#include "Titan.h"

//敵が作られて最初に呼ぶ処理。
Titan::Titan()
{
	m_se[0].Init(L"Assets/sound/RE_Attack.wav");			//攻撃。
	m_se[1].Init(L"Assets/sound/RE_walk.wav");			//歩く。
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
	animClip[esAttackGap].Load(L"Assets/animData/RE1_accumulate.tka");	//隙の部分をロード。
	animClip[esAttackGap].SetLoopFlag(true);
	animClip[esStandbyAttack].Load(L"Assets/animData/RE1_beforeATK.tka");	//攻撃前のモーションをロード。
	animClip[esStandbyAttack].SetLoopFlag(false);
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
	prm.HP = 120;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 55;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 300;										//速さ。
	m_charaCon.Init(50.0f, 100.0f, m_position, enCollisionAttr_Enemy);			//判定の大きさ
	e_state = Enemys::EnemyAnimState::esIdle;									//最初なので待機。
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/RobbotEnemyAttack.efk");
}
//敵を消すときに消す。
Titan::~Titan()
{
	g_effektEngine->Stop(m_playEffectHandle);
}
//攻撃するときに呼ぶ関数
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
	if (e_state != esAttack) {
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
				isAttack_flag = true;
				e_state = esTracking;
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
}
//敵の更新内容。
void Titan::Update()
{
	//Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
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
		m_attackEffect->Release();
		isDeath_flag = true;
		m_se[1].Stop();
	}
}
//エネミーの状態
void Titan::EnemyState()
{
	switch (e_state)
	{
		//待機中。
	case Enemys::esIdle:
		Search();
		anim.Update(GameTime().GetFrameDeltaTime());
		anim.Play(esIdle);
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
		//追いかけてる。
	case Enemys::esTracking:
		Search();
		EMove();
		anim.Update(GameTime().GetFrameDeltaTime()*1.5f);
		//Rotation();
		anim.Play(esTracking);
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
		//攻撃。
	case Enemys::esAttack:
		Enemys::Rotation();
		anim.Update(GameTime().GetFrameDeltaTime());
		anim.Play(esAttack);
		break;
		//死亡したとき。
	case Enemys::esDeath:
		Death();
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
	case Enemys::esAttackGap:
		AttackCoolTime();
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
	case Enemys::esStandbyAttack:
		Search();
		StandbyAttack();
		anim.Update(GameTime().GetFrameDeltaTime());
		break;
	}
}
//エネミーが進む処理。
void Titan::EMove()
{
	m_move.Normalize();
	switch (e_state)
	{
	case Enemys::esIdle:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		if (m_se[1].IsPlaying())
		{
			m_se[1].Stop();
		}
		break;
	case Enemys::esAttack:
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
		if (m_se[1].IsPlaying())
		{
			m_se[1].Stop();
		}
		break;
	case Enemys::esTracking:
		m_moveSpeed = m_move * prm.SPD;
		m_se[1].Play(true);
		break;
	}
}
//アニメーションイベント
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)

{
	if (m_player->GetIsDead() == false) {
		if (e_state == esAttack && eventName && m_diff.Length() <= m_TattackDistance)
		{
			Attack();
			m_se[1].Stop();
			m_se[0].Play(false);
			Enemys::EnemyEffect();
			e_state = esIdle;
		}
		else if (m_diff.Length() >= m_TattackDistance)
		{
			e_state = esIdle;
		}
	}
}
//攻撃できるか
void Titan::AttackRange()
{
	if (m_diff.Length() <= m_TattackDistance && isAttack_flag
		&& fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//距離内に近づいたら攻撃。
		e_state = esStandbyAttack;
	}
}
//攻撃待機
void Titan::AttackCoolTime()
{
	Enemys::ViewingAngle();
	//アニメーションの再生。
	anim.Play(esAttackGap);
	//攻撃前、少しの間動かない系男子。
	float m_Limit = 0.75f;
	m_timer += GameTime().GetFrameDeltaTime();
	//時間になったら。
	if (anim.IsPlaying() && m_diff.Length() >= m_TattackDistance)
	{
		e_state = esIdle;
		m_timer = ZERO;
	}

	if (m_timer >= m_Limit)
	{
		//攻撃
		e_state = esAttack;
		m_timer = ZERO;
	}
}
//攻撃が発生する前の処理
void Titan::StandbyAttack()
{
	anim.Play(esStandbyAttack);
	//アニメーションの再生が終わったら切り替え。
	if (!anim.IsPlaying())
	{
		e_state = esAttackGap;
	}
	if (m_diff.Length() >= m_Distance)
	{
	e_state = esIdle;
	m_timer = ZERO;
	}
}
