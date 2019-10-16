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
	SkinModel gModel;						//model�����s�v�b�I�I�I�I�I
	Animation g_anim;						//�A�j��
	AnimationClip g_animClip[3];			//�A�j���N���b�v
	CVector3 m_position = CVector3::Zero();	//�|�W�V����
	EnemyInitParam prm;						//�G�l�p���B
};

