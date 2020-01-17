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
	void Attack();							//攻撃(今は未実装)
	void Damage(float Damage);				//DAMAGE
	void Search();							//プレイヤーを見つける関数
	void Update();							//敵の更新内容を記入
	void Draw();							//エネミーをドロー処理だよ。
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void Rotation();						//エネミーが動きに合わせて回転する処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
private:
	BossAnimState boss_State;				//ボスのステート。
	BossMode Mode;
	CVector3 diff = CVector3::Zero();		//プレイヤーと距離を測る用
	float Kyori = 500.0f;
};

