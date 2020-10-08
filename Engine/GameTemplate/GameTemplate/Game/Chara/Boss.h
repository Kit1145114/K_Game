#pragma once
#include"Enemys.h"

class Boss : public Enemys
{
public:
	Boss();
	~Boss() {};
	enum BossMode
	{
		SmallATK,								//弱攻撃。
		BigATK									//強攻撃。
	};
	enum BossFear
	{
		NO,										//よろける。
		YES										//よろけない。
	};
	void Attack();								//攻撃
	void AttackRange();							//攻撃できるかの処理。
	void Damage(int Damage);					//DAMAGE
	void Search();								//プレイヤーを見つける関数
	void Update()		override;				//敵の更新内容を記入
	void Death();								//エネミーが死ぬ処理だよ。
	void EnemyState();							//エネミーをステイトで行動する処理だよ。
	void EMove();								//エネミーが動く処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	void HitMe();								//ダメージを受けるｚ....
	void Fear();								//よろけるかよろけないか。
private:
	Enemys::BossAnimState boss_State;			//ボスのステート。
	BossMode Mode;								//ボスの攻撃用のステート。
	BossFear bossFear;							//ボスがひるむかのステート
	CVector3 m_efePos;							//エフェクトのポジション
	CVector3 m_efeRot;							//エフェクトの回転
	int m_hitAttack = 0;						//攻撃を受けた回数。
	int m_maxHitAttack = 0;						//怯む時に使用。
	float m_track = 4500.0f;					//追いかけるための変数
	float m_waitTime = 0.0f;					//攻撃後の待機時間
	float m_flySpeed = 2.0f;					//飛行のスピード。
	int m_upDamage = 1.2f;						//2発目のダメージを上げる
	bool fearAdd_flag = true;					//フラグ
	Effekseer::Handle m_playEffectHandle = 10;	//エフェクトを呼び出すときの番号入り。
};

