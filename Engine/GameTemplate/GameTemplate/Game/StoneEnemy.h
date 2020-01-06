#pragma once
#include"Enemys.h"
class StoneEnemy :public Enemys
{
public:
	StoneEnemy();
	~StoneEnemy();
	void Attack();
	void Damage(float Damage) override;
	void Search();
	void EMove();
	void Update();
	void Draw();
	void Death();
	void EnemyState();
private:
	Enemys* enemys;
	SkinModel seModel;						//model説明不要ッ！！！！！
	Animation s_anim;						//アニメ
	AnimationClip s_animClip[3];			//アニメクリップ
	EnemyInitParam prm;						//エネパラ。
	CVector3 Scale = { 3.0f,3.0f,3.0f };	//大きさいじるよう
};

