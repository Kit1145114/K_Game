#pragma once
#include"Enemys.h"
#include"physics/PhysicsGhostObject.h"

class StoneGolem : public Enemys
{
public:
	StoneGolem();
	~StoneGolem() {};
	void Attack();							//攻撃
	void AttackRange();						//攻撃できるかの処理。
	void Damage(int Damage)override;		//ダメージ	
	void Search();							//プレイヤーを見つける関数
	void Update()		override;			//敵の更新内容を記入
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	void HitMe();							//未実装です。
	void AttackAfter();						//攻撃後。
	void HitPlayerObj();
private:
	float m_attackTime = 0.0f;
	float m_objPosAdd = 2.5f;
	Effekseer::Handle m_playEffectHandle = 2;
	CVector3 m_efePos = CVector3::Zero();
	CVector3 diff = CVector3::Zero();
	bool loop = true;
	PhysicsGhostObject m_PhyGhostObj;					//ゴースト
	CVector3 box_scale = { 10.0f,10.0f,700.0f };		//ボックス大きさ
};

