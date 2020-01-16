#pragma once
#include"Enemys.h"
class StoneEnemy :public Enemys
{
public:
	StoneEnemy();
	~StoneEnemy(){};
	void Attack();							//�U��(���͖�����)
	void Damage(float Damage) override;		//DAMAGE�i���͖������j
	void Search();							//�v���C���[��������֐�
	void Update();							//�G�̍X�V���e���L��
	void Draw();							//�G�l�~�[���h���[��������B
	void Death();							//�G�l�~�[�����ʏ�������B
	void EnemyState();						//�G�l�~�[���X�e�C�g�ōs�����鏈������B
	void EMove();							//�G�l�~�[��������������B
	void Rotation();						//�G�l�~�[�������ɍ��킹�ĉ�]���鏈������B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//�A�j���[�V�����C�x���g�B
private:
	Enemys* enemys;
	SkinModel seModel;						//model�����s�v�b�I�I�I�I�I
	Animation s_anim;						//�A�j��
	AnimationClip s_animClip[m_AnimClipNum];//�A�j���N���b�v
};

