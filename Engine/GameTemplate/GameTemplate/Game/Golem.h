#pragma once
#include"Enemys.h"

class Golem : public Enemys
{
public:
	Golem();
	~Golem() {};
	void Attack() override;
	void Damage(float Damage) override;
	void Search();
	void Update();
	void Draw();
private:
	SkinModel gModel;						//model説明不要ッ！！！！！
	Animation g_anim;						//アニメ
	AnimationClip g_animClip[3];			//アニメクリップ
	CVector3 m_position = CVector3::Zero();	//ポジション
	EnemyInitParam prm;						//エネパラ。
};

