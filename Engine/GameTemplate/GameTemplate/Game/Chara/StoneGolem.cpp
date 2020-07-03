#include "stdafx.h"
#include "StoneGolem.h"


StoneGolem::StoneGolem()
{
	Model.Init(L"Assets/modelData/RobbotEnemy2.cmo");					//モデルの呼び出し。
	//パラメーター
	animClip[esIdle].Load(L"Assets/animData/RE2_Idle.tka");				//待機アニメーションをロード。
	animClip[esIdle].SetLoopFlag(true);									//再生し続けるのでtrue
	animClip[esTracking].Load(L"Assets/animData/RE2_Move.tka");			//待機アニメーションをロード。
	animClip[esTracking].SetLoopFlag(true);								//再生し続けるのでtrue
	animClip[esAttack].Load(L"Assets/animData/RE2_Attack.tka");			//攻撃アニメーションをロード。
	animClip[esAttack].SetLoopFlag(false);								//再生し続けるのでtrue
	animClip[esDeath].Load(L"Assets/animData/RE2_Death.tka");			//死ぬアニメーションをロード。
	animClip[esDeath].SetLoopFlag(false);								//一回でいいのでfalse
	animClip[esAttackGap].Load(L"Assets/animData/RE2_AfterAttack.tka");	//攻撃終わった後アニメーション
	animClip[esAttackGap].SetLoopFlag(false);							//一回でいいのでfalse
	anim.Init(
		Model,
		animClip,
		m_AnimClipNum
	);
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 51;										//攻撃力
	prm.DEF = 70;										//防御力
	prm.SPD = 300;										//速さ。
	m_charaCon.Init(50.0f, 100.0f, m_position, enCollisionAttr_Enemy);		//判定の大きさ
	e_state = esIdle;
	m_attackEffect = g_effektEngine->CreateEffekseerEffect(L"Assets/effect/Laser2.efk");
	m_se[0].Init(L"Assets/sound/fire4.wav");			//攻撃
	m_se[1].Init(L"Assets/sound/laserDamage.wav");		//ダメージ音
	m_se[2].Init(L"Assets/sound/RE2_Walk.wav");			//歩く。
}

StoneGolem::~StoneGolem()
{
	g_effektEngine->Stop(m_playEffectHandle);
}

void StoneGolem::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(GameTime().GetFrameDeltaTime());
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_charaCon.SetPosition(m_position);
}

void StoneGolem::Attack()
{
	anim.Play(esAttack);
	m_timer += GameTime().GetFrameDeltaTime();
	m_se[2].Stop();
	if (m_timer >= 0.5f)
	{
		if (loop) {
			diff.x = m_forward.x;
			diff.z = m_forward.z;
			m_playEffectHandle = g_effektEngine->Play(m_attackEffect);
			g_effektEngine->SetRotation(m_playEffectHandle, ZERO, atan2(diff.x, diff.z), ZERO);
			loop = false;
		}
		m_se[0].Play(false);
		m_se[0].SetVolume(0.3f);
		m_efePos = m_position;
		m_efePos.y += 142.0f;
		//攻撃のエフェクト
		g_effektEngine->SetPosition(m_playEffectHandle, m_efePos);
		//攻撃したときのゴーストがプレイヤーと当たっているか。
		HitPlayerObj();
		//終わるまでの時間を回す。
		m_attackTime += GameTime().GetFrameDeltaTime();
		if (m_attackTime >= 3.5f)
		{
			g_effektEngine->Stop(m_playEffectHandle);
			e_state = esAttackGap;
			m_timer = ZERO;
			m_attackTime = ZERO;
			m_se[0].Stop();
			if (m_se[1].IsPlaying())
			{
				m_se[1].Stop();
			}
			loop = true;
		}
	}
	//m_player->Damage(prm.ATK);
}

void StoneGolem::AttackRange()
{
	attackDistance = 300.0f;
	if (m_diff.Length() <= attackDistance && isTracking && fabsf(m_angle) < CMath::PI * 0.20f)
	{
		//距離内に近づいたら攻撃。
		e_state = esAttack;
	}
}

void StoneGolem::Search()
{
	Enemys::ViewingAngle();
	//体力MAX時
	m_enemytrack = 1000.0f;
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

void StoneGolem::Death()
{
	anim.Play(esDeath);
	g_effektEngine->Stop(m_playEffectHandle);
	m_PhyGhostObj.Release();
	if (m_se[0].IsPlaying())
	{
		m_se[0].Stop();
		m_attackEffect->Release();
	}
	if (anim.IsPlaying() == false)
	{
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
		isDeath = true;
	}
}

void StoneGolem::EnemyState()
{
	switch (e_state)
	{
		//待機中。
	case Enemys::esIdle:
		Search();
		anim.Play(esIdle);
		break;
		//追いかけてる。
	case Enemys::esTracking:
		Search();
		EMove();
		anim.Play(esTracking);
		break;
		//攻撃。
	case Enemys::esAttack:
		Attack();
		break;
		//死亡したとき。
	case Enemys::esDeath:
		Death();
		break;
	case Enemys::esAttackGap:
		AttackAfter();
	}
}
//キャラの移動処理。
void StoneGolem::EMove()
{
	Move.Normalize();
	if (e_state == esIdle || e_state == esAttack)
	{
		m_moveSpeed.x = ZERO;
		m_moveSpeed.z = ZERO;
	}
	else if (e_state == esTracking) {
		m_moveSpeed = Move * prm.SPD;
		m_se[2].SetFrequencyRatio(8.0f);
		m_se[2].Play(true);
	}
}

void StoneGolem::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

void StoneGolem::Damage(int Damage)
{
	prm.HP -= (Damage - prm.DEF);
	//もし、HPが0以下なら死亡処理。
	if (prm.HP <= 0.0f)
	{
		e_state = esDeath;
	}
}

void StoneGolem::AttackAfter()
{
	anim.Play(esAttackGap);
	//終わったら最初の状態へ。
	if (!anim.IsPlaying())
	{
		e_state = esIdle;
	}
}

void StoneGolem::HitPlayerObj()
{
	//攻撃の判定とってます。
	//Aベクトル(単位ベクトル)、モルガンの正面のベクトル
	CVector3 front = m_forward;
	CVector3 pos = m_player->GetPosition() - m_position;
	//AベクトルとBベクトルの内積を求める
	//これにより「BベクトルからAベクトルに垂直ベクトル」と「Aベクトル」の交わる座標からAベクトルの始点までの長さが求まる
	float ip = front.Dot(pos);
	if (ip > 0 && ip < m_length) {
		//Aベクトルに求めた内積をかけて、「BベクトルからAベクトルに垂直ベクトル」と「Aベクトル」の交わる座標のCベクトルを求める
		CVector3 c = front * ip;
		//CベクトルとBベクトルの距離の距離の2乗が一定以下(敵のコリジョンの半径＋モルガンのえんちうの半径の2乗)ならプレイヤーにダメージを与える
		CVector3 pos2 = pos - c;
		if (pos2.LengthSq() <= std::pow((m_player->GetRadius() + m_r), 2.0f)) {
			m_player->Damage(prm.ATK);
			if (SoundMakeFlag)
			{
				m_se[1].Play(false);
				m_se[1].SetVolume(0.3f);
				SoundMakeFlag = false;
			}
		}
		else if(m_se[1].IsPlaying() &&  e_state != esAttack
			|| pos2.LengthSq() >= std::pow((m_player->GetRadius() + m_r), 2.0f))
		{
			m_se[1].Stop();
			SoundMakeFlag = true;
		};
	}
}