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
	e_state = esIdle;									//最初なので待機。
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/RobbotEnemyAttack.efk");

}
//敵を消すときに消す。
Titan::~Titan()
{
	g_effektEngine->Stop(m_playEffectHandle);
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
		isDeath = true;
		m_se[1].Stop();
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
		anim.Update(GameTime().GetFrameDeltaTime());
		//Rotation();
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
		break;
		//攻撃。
	case Enemys::esAttack:
		Search();
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
		if (m_se[1].IsPlaying())
		{
			m_se[1].Stop();
		}
	}
	else if (e_state == esTracking) {
		m_moveSpeed = Move * prm.SPD;
		m_se[1].Play(true);
	}
}
//アニメーションイベント
void Titan::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	CVector3 dis;
	CVector3 diff;
	if (m_player->GetIsDead() == false) {
		if (e_state == esAttack && eventName && m_diff.Length() <= m_TattackDistance)
		{
			Attack();
			m_se[1].Stop();
			m_se[0].Play(false);
			dis = (m_player->GetPosition() + m_position) * 0.5f;
			diff = m_player->GetPosition() - m_position;
			dis.y += 30.0f;
			//{ m_forward.x, 50.0f,m_forward.z };
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			g_effektEngine->SetPosition(m_playEffectHandle,/*m_player->GetPosition()*/dis);
			g_effektEngine->SetRotation(m_playEffectHandle, 0.0f, atan2( diff.x, diff.z), 0.0f);
			e_state = esAttackGap;
		}
		else if (e_state == esAttack && eventName && m_diff.Length() >= m_TattackDistance)
		{
			e_state = esAttackGap;
		}
	}
}
//攻撃できるか
void Titan::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//距離内に近づいたら攻撃。
		e_state = esAttack;
	}
}
//攻撃待機
void Titan::AttackCoolTime()
{
	//攻撃後、少しの間動かない系男子。
	float Limit = 0.75f;
	if (e_state == esAttackGap)
	{
		anim.Play(esIdle);
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//時間で戻る。
	if (m_timer > Limit)
	{
		e_state = esIdle;
		m_timer = ZERO;
	}
}
