#pragma once
#include"Enemys.h"

class Titan : public Enemys
{
public:
	Titan();
	~Titan() {};
	void Attack() override;
	void Damage(float Damage) override;
	void Search();
	void Update();
	void Draw();
private:
	SkinModel tModel;						//model説明不要ッ！！！！！
	Animation t_anim;						//アニメ
	AnimationClip t_animClip[3];			//アニメクリップ
	CVector3 m_position = CVector3::Zero();	//ポジション
	EnemyInitParam prm;						//エネパラ。
	CVector3 e3_pos = { -400.0f,0.0f,0.0f };
};

