#pragma once
#include"Enemys.h"

class StoneEnemy :public Enemys
{
public:
	StoneEnemy();
	~StoneEnemy(){}
	void Attack() override;
	void Damage(float Damage) override;
	void Search();
	void Update();
	void Draw();
private:
	SkinModel seModel;						//model説明不要ッ！！！！！
	Animation s_anim;						//アニメ
	AnimationClip s_animClip[3];			//アニメクリップ
	CVector3 m_position = CVector3::Zero();	//ポジション
	EnemyInitParam prm;						//エネパラ。
};

