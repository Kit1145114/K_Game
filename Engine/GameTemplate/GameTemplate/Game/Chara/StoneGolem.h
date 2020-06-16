#pragma once
#include"Enemys.h"
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
		
private:

};

