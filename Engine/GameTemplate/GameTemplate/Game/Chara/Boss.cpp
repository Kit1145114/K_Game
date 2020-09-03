#include "stdafx.h"
#include "Boss.h"

//敵が作られて最初に呼ぶ処理。
Boss::Boss()
{
	m_se[0].Init(L"Assets/sound/BossAttack1.wav");
	m_se[1].Init(L"Assets/sound/BossAttack2.wav");
	m_se[2].Init(L"Assets/sound/BossWalk.wav");
	Model.Init(L"Assets/modelData/RobbotBoss.cmo");		//モデルの呼び出し。
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Attack.efk");
	//モデルのアニメーションのロード。
	animClip[bsIdle].Load(L"Assets/animData/RB_idle.tka");	//待機をロード。
	animClip[bsIdle].SetLoopFlag(true);
	animClip[bsWalkTracking].Load(L"Assets/animData/RB_walk.tka");	//歩きをロード。
	animClip[bsWalkTracking].SetLoopFlag(true);
	animClip[bsFlyTracking].Load(L"Assets/animData/RB_FlyMove.tka");	//歩きをロード。
	animClip[bsFlyTracking].SetLoopFlag(true);
	animClip[bsDeath].Load(L"Assets/animData/RB_death.tka");	//死亡をロード。
	animClip[bsDeath].SetLoopFlag(false);
	animClip[bsSmallAttack].Load(L"Assets/animData/RB_shortATK.tka");	//攻撃をロード。
	animClip[bsSmallAttack].SetLoopFlag(true);
	animClip[bsBigAttack].Load(L"Assets/animData/RB_BigATK.tka");	//攻撃をロード。
	animClip[bsBigAttack].SetLoopFlag(true);
	animClip[bsHitMe].Load(L"Assets/animData/RB_HitDamage.tka");	//攻撃をロード。
	animClip[bsHitMe].SetLoopFlag(false);
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
	prm.HP = 150;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 60;										//攻撃力
	prm.DEF = 80;										//防御力
	prm.SPD = 700;										//速さ。
	m_scale = { 0.7f,0.7f,0.7f };						//エネミーの大きさ
	m_charaCon.Init(50.0f, 250.0f,m_position, enCollisionAttr_Enemy);			//判定の大きさ
	boss_State = bsIdle;								//最初なので待機。
	Mode = SmallATK;									//何攻撃をするか。
	bossFear = NO;
	m_maxHitAttack = 0;									//ひるむ回数。
}
//敵の更新内容。
void Boss::Update()
{
	Enemys::VectorAcquisition();
	HitMe();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}
//敵の攻撃処理。
void Boss::Attack()
{
	m_player->Damage(prm.ATK);
}
//攻撃できる範囲か調べる処理
void Boss::AttackRange()
{
	if (m_diff.Length() <= attackDistance && Mode == SmallATK
		&& isTracking)
	{
		//距離内に近づいたら攻撃。
		boss_State = bsSmallAttack;
	}
	else if (m_diff.Length() <= attackDistance && Mode == BigATK
		&& isTracking)
	{
		boss_State = bsBigAttack;
	}

}
//エネミーが進む処理。
void Boss::EMove()
{
	Move.Normalize();
	if (boss_State == bsIdle)
	{
		m_moveSpeed = CVector3::Zero();
	}
	else if (boss_State == bsBigAttack || boss_State == bsSmallAttack)
	{
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
	}
	else if (boss_State == bsWalkTracking) {
		m_moveSpeed = Move * prm.SPD;
		m_se[2].Play(true);
	}
	else if (boss_State == bsFlyTracking)
	{
		m_moveSpeed = Move * prm.SPD * 2;
	}
}
//DAMAGE受ける処理
void Boss::Damage(int Dam)
{
	prm.HP -= (Dam - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0)
	{
		boss_State = bsDeath;
	}
}
//プレイヤーの見つける処理。
void Boss::Search()
{
	Enemys::ViewingAngle();
	//体力MAX時
	if (prm.HP == m_MaxHP) {
		//範囲外かつ視野角外なら
		if (m_diff.Length() >= track || fabsf(m_angle) > CMath::PI * 0.60f)
		{
			boss_State = bsIdle;
			isTracking = false;	
		}
		//範囲内かつ視野角内なら
		else if (m_diff.Length() <= track && fabsf(m_angle) < CMath::PI * 0.40f)
		{
			Move = m_player->GetPosition() - m_position;
			isTracking = true;
			//飛行距離内なら
			if (m_diff.Length() >= flyDistance)
			{
				Move = m_player->GetPosition() - m_position;
				boss_State = bsFlyTracking;
			}
			//歩行距離内なら
			else if (m_diff.Length() >= walkingDistance)
			{
				Move = m_player->GetPosition() - m_position;
				boss_State = bsWalkTracking;
			}
		}
	}
	//体力がMAXじゃないとき。ひたすら追いかける。
	else if (prm.HP < m_MaxHP)
	{
		Move = m_player->GetPosition() - m_position;
		isTracking = true;
		//飛行距離内なら
		if (m_diff.Length() >= flyDistance)
		{
			Move = m_player->GetPosition() - m_position;
			boss_State = bsFlyTracking;
		}
		//歩行距離内なら
		else if (m_diff.Length() >= walkingDistance)
		{
			Move = m_player->GetPosition() - m_position;
			boss_State = bsWalkTracking;
		}
	}
	//攻撃の範囲計算。
	AttackRange();
}
//倒されたときに呼ぶ処理。
void Boss::Death()
{
	anim.Play(bsDeath);
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
		EMove();
		Enemys::Rotation();
		anim.Play(bsIdle);
		m_se[2].Stop();
		break;
	case Enemys::bsWalkTracking:
		Search();
		EMove();
		Enemys::Rotation();
		anim.Play(bsWalkTracking);
		break;
	case Enemys::bsFlyTracking:
		Search();
		EMove();
		Enemys::Rotation();
		anim.Play(bsFlyTracking);
		m_se[2].Stop();
	case Enemys::bsDeath:
		Death();
		m_se[2].Stop();
		break;
	case Enemys::bsSmallAttack:
		Search();
		EMove();
		Enemys::Rotation();
		anim.Play(bsSmallAttack);
		m_se[2].Stop();
		break;
	case Enemys::bsBigAttack:
		Search();
		EMove();
		Enemys::Rotation();
		anim.Play(bsBigAttack);
		break;
	case Enemys::bsHitMe:
		Fear();
		m_se[2].Stop();
		break;
	}
}
//アニメーションイベント
void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (m_player->GetIsDead() == false) {
		if (boss_State == bsSmallAttack 
			&& eventName)
		{
			Attack();
			Mode = BigATK;
			m_se[0].Play(false);
			EnemyEffect();
		}
		else if (boss_State == bsBigAttack 
			&& eventName)
		{
			prm.ATK * 1.2f;
			Attack();
			Mode = SmallATK;
			m_se[1].Play(false);
			EnemyEffect();
		}
	}
}
//DAMAGE受けたときの...
void Boss::HitMe()
{
	//攻撃受けたかつひるまないとき。
	if (isHitMe && bossFear == NO)
	{
		//怯みの回数に達したとき。
		if (m_hitAttack == m_maxHitAttack)
		{
			bossFear = YES;
			boss_State = bsHitMe;
		}
		//達さなかった時、ひるんでるときに回数を増やしたくないのでフラグ縦。
		else if (m_hitAttack < m_maxHitAttack && fearAdd_flag)
		{
			m_hitAttack++;
			isHitMe = false;
		}
	}
}
//ひるんだ時の処理
void Boss::Fear()
{
	anim.Play(bsHitMe);
	//一度だけ処理する。
	if (anim.IsPlaying() && fearAdd_flag)
	{
		m_maxHitAttack++;
		fearAdd_flag = false;
	}
	else if (!anim.IsPlaying())
	{
		boss_State = bsIdle;
		bossFear = NO;
		fearAdd_flag = true;
		m_hitAttack = ZERO;
		isHitMe = false;
	}
}
//敵のエフェクト。
void Boss::EnemyEffect()
{
	CVector3 dis;
	CVector3 diff;
	//エフェクトの位置を調整。
	dis = (m_player->GetPosition() + m_position) * 0.5f;
	diff = m_player->GetPosition() - m_position;
	dis.y += 30.0f;
	m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
	g_effektEngine->SetPosition(m_playEffectHandle,/*m_player->GetPosition()*/dis);
	g_effektEngine->SetRotation(m_playEffectHandle, 0.0f, atan2(diff.x, diff.z), 0.0f);
}