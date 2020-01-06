#pragma once
#include"Enemys.h"

class Golem : public Enemys
{
public:
	Golem();
	~Golem() {};
	void Attack();
	void Damage(float Damage) override;
	void Search();
	void Update();
	void Draw();
	void Death();
	void EnemyState();
	void EMove();
private:
	SkinModel gModel;						//model説明不要ッ！！！！！
	Animation g_anim;						//アニメ
	AnimationClip g_animClip[3];			//アニメクリップ
	EnemyInitParam prm;						//エネパラ。
	CVector3 e2_pos = { 400.0f,0.0f,0.0f };
	CVector3 scale = {5.0f,5.0f,5.0f};
};

