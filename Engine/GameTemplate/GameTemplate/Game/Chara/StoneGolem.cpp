#include "stdafx.h"
#include "StoneGolem.h"


StoneGolem::StoneGolem()
{
	Model.Init(L"Assets/modelData/Enemy1.cmo");		//モデルの呼び出し。
	//パラメーター
	prm.HP = 100;										//HP
	m_MaxHP = prm.HP;									//MAXHP;
	prm.ATK = 20;										//攻撃力
	prm.DEF = 30;										//防御力
	prm.SPD = 300;										//速さ。
	m_charaCon.Init(50.0f, 150.0f, m_position);		//判定の大きさ
	e_state = esIdle;
}

void StoneGolem::Update()
{
	Enemys::Draw();
	Enemys::VectorAcquisition();
	Enemys::Rotation();
	EnemyState();
	m_moveSpeed.y -= gravity;
	anim.Update(0.03f);
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	Model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	m_charaCon.SetPosition(m_position);
}

void StoneGolem::Attack()
{
	m_player->Damage(prm.ATK);
}

void StoneGolem::AttackRange()
{
	if (m_diff.Length() <= attackDistance && isTracking)
	{
		//距離内に近づいたら攻撃。
		e_state = esAttack;
	}
}

void StoneGolem::Search()
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

void StoneGolem::Death()
{
	this->SetActive(false);
	m_charaCon.RemoveRigidBoby();
	isDeath = true;
}

void StoneGolem::EnemyState()
{
	switch (e_state)
	{
		//待機中。
	case Enemys::esIdle:
		Search();
		Rotation();
		//anim.Play(esIdle);
		break;
		//追いかけてる。
	case Enemys::esTracking:
		Search();
		EMove();
		Rotation();
		//anim.Play(esTracking);
		break;
		//攻撃。
	case Enemys::esAttack:
		Search();
		Rotation();
		//anim.Play(esAttack);
		break;
		//死亡したとき。
	case Enemys::esDeath:
		Death();
	}
}

void StoneGolem::EMove()
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

void StoneGolem::HitMe()
{

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