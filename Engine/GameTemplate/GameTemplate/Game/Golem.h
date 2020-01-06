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
	SkinModel gModel;						//model�����s�v�b�I�I�I�I�I
	Animation g_anim;						//�A�j��
	AnimationClip g_animClip[3];			//�A�j���N���b�v
	EnemyInitParam prm;						//�G�l�p���B
	CVector3 e2_pos = { 400.0f,0.0f,0.0f };
	CVector3 scale = {5.0f,5.0f,5.0f};
};

