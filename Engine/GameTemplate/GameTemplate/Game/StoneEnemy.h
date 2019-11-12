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
	Enemys* enemys;
	SkinModel seModel;						//model�����s�v�b�I�I�I�I�I
	Animation s_anim;						//�A�j��
	AnimationClip s_animClip[3];			//�A�j���N���b�v
	CVector3 m_position = CVector3::Zero();	//�|�W�V����
	EnemyInitParam prm;						//�G�l�p���B
	CVector3 Scale = { 3.0f,3.0f,3.0f };
};

