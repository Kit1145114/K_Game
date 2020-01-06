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
	SkinModel seModel;						//model�����s�v�b�I�I�I�I�I
	Animation s_anim;						//�A�j��
	AnimationClip s_animClip[3];			//�A�j���N���b�v
	EnemyInitParam prm;						//�G�l�p���B
	CVector3 Scale = { 3.0f,3.0f,3.0f };	//�傫��������悤
};

