#pragma once
#include"Enemys.h"

class Titan : public Enemys
{
public:
	Titan();
	~Titan() {};
	void Attack() override;					//�U��(���͖�����)
	void Damage(float Damage) override;		//DAMAGE�i���͖������j
	void Search();							//�v���C���[��������֐�
	void Update();
	void Draw();
private:
	SkinModel tModel;							//model�����s�v�b�I�I�I�I�I
	Animation t_anim;							//�A�j��
	AnimationClip t_animClip[3];				//�A�j���N���b�v
	EnemyInitParam prm;							//�G�l�p���B
	CVector3 e3_pos = { -400.0f,0.0f,0.0f };	//�G�l�~�[�̏����|�W
};

