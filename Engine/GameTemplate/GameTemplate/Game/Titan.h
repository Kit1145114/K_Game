#pragma once
#include"Enemys.h"

class Titan : public Enemys
{
public:
	Titan();
	~Titan() {};
	void Attack();					//�U��(���͖�����)
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
	SkinModel tModel;							//model�����s�v�b�I�I�I�I�I
	Animation t_anim;							//�A�j��
	AnimationClip t_animClip[m_AnimClipNum];	//�A�j���N���b�v
	EnemyInitParam prm;							//�G�l�p���B
	CVector3 e3_pos = { -400.0f,0.0f,0.0f };	//�G�l�~�[�̏����|�W
	CVector3 m_scale = { 30.0f,30.0f,30.0f };	//�G�l�~�[�̑傫���p�̃����o�ϐ��B
};

