#pragma once
#include"Enemys.h"
class Boss : public Enemys
{
public:
	Boss();
	~Boss() {};
	enum BossMode
	{
		SmallATK,							//弱攻撃。
		BigATK								//強攻撃。
	};
	void Attack();							//攻撃
	void AttackRange();						//攻撃できるかの処理。
	void Damage(int Damage);				//DAMAGE
	void Search();							//プレイヤーを見つける関数
	void Update()		override;						//敵の更新内容を記入
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	void HitMe();							//ダメージを受けるｚ....
private:
	BossAnimState boss_State;				//ボスのステート。
	BossMode Mode;							//ボスの攻撃用のステート。
	float track = 1500.0f;					//追いかけるための変数
};

