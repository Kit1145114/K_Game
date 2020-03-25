#pragma once
#include"Enemys.h"

class Golem : public Enemys
{
public:
	Golem();
	~Golem() {};
	void Attack();					//攻撃(今は未実装)
	void Damage(int Damage) override;		//DAMAGE（今は未実装）
	void Search();							//プレイヤーを見つける関数
	void Update();							//敵の更新内容を記入
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void Rotation();						//エネミーが動きに合わせて回転する処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	void AttackRange();						//攻撃できるかの処理。
private:
};

