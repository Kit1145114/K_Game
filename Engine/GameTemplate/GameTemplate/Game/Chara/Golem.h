#pragma once
#include"Enemys.h"

class Golem : public Enemys
{
public:
	Golem();
	~Golem();
	void Attack();					//攻撃(今は未実装)
	void Damage(int Damage) override;		//DAMAGE（今は未実装）
	void Search();							//プレイヤーを見つける関数
	void Update()override;					//敵の更新内容を記入
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	void AttackRange();						//攻撃できるかの処理。
	void AttackStanby();					//攻撃待機から攻撃まで。
	void AttackGap();						//攻撃後の隙の部分。
private:
	int m_playerToDamage = 51;				//プレイヤーに与えるダメージの値。
	float m_effectSpwanTime = 1.0f;			//攻撃に移るまでの時間用の変数
	float m_attackTime = 0.0f;				//攻撃までの時間を図るための変数。
	float m_goAttackTime = 3.0f;			//この時間になったら攻撃するよ。
	float m_falltimer = 0.0f;				//倒れている時間を図るための変数。
	float m_fallLimitTime = 2.0f;			//いつまで倒れているかを決める変数。
	float m_attackDeistance = 125.0f;		//攻撃状態になる距離。
	CVector3 m_attackDis = CVector3::Zero();	//攻撃をプレイヤーに与える範囲を計算するためのもの
	Effekseer::Handle m_playEffectHandle = 1;
};

