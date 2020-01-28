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
	void Damage(int Damage);				//DAMAGE
	void Search();							//プレイヤーを見つける関数
	void Update();							//敵の更新内容を記入
	void Draw();							//エネミーをドロー処理だよ。
	void Death();							//エネミーが死ぬ処理だよ。
	void EnemyState();						//エネミーをステイトで行動する処理だよ。
	void EMove();							//エネミーが動く処理だよ。
	void Rotation();						//エネミーが動きに合わせて回転する処理だよ。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//アニメーションイベント。
	void ViewingAngle();					//エネミーの視野角。
	void VectorAcquisition();				//エネミーのベクトルを取得するための関数。
private:
	BossAnimState boss_State;				//ボスのステート。
	BossMode Mode;
	CVector3 diff = CVector3::Zero();		//プレイヤーと距離を測る用
	float attackDistance = 300.0f;			//範囲内で攻撃するための変数
	float track = 1500.0f;					//追いかけるための変数
	float angle = 0.0f;
};

