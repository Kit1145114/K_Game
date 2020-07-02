#pragma once
#include"Enemys.h"

class Titan : public Enemys
{
public:
	Titan();
	~Titan() {};
	void Attack();							//攻撃(今は未実装)
	void Damage(int Damage) override;		//DAMAGE（今は未実装）
	void Search();							//プレイヤーを見つける関数
	void Update()override;					//敵の更新内容を記入
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	void AttackRange();						//攻撃できるかの処理。
	void AttackCoolTime();					//攻撃した後の隙の時間
private:
	Effekseer::Handle m_playEffectHandle = 1;
};

