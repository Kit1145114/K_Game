#pragma once
#include"Enemys.h"
class Boss : public Enemys
{
public:
	Boss();
	~Boss() {};
	enum BossMode
	{
		SmallATK,							//��U���B
		BigATK								//���U���B
	};
	void Attack();							//�U��(���͖�����)
	void Damage(int Damage);				//DAMAGE
	void Search();							//�v���C���[��������֐�
	void Update();							//�G�̍X�V���e���L��
	void Draw();							//�G�l�~�[���h���[��������B
	void Death();							//�G�l�~�[�����ʏ�������B
	void EnemyState();						//�G�l�~�[���X�e�C�g�ōs�����鏈������B
	void EMove();							//�G�l�~�[��������������B
	void Rotation();						//�G�l�~�[�������ɍ��킹�ĉ�]���鏈������B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//�A�j���[�V�����C�x���g�B
	void ViewingAngle();					//�G�l�~�[�̎���p�B
	void VectorAcquisition();				//�G�l�~�[�̃x�N�g�����擾���邽�߂̊֐��B
private:
	BossAnimState boss_State;				//�{�X�̃X�e�[�g�B
	BossMode Mode;
	CVector3 diff = CVector3::Zero();		//�v���C���[�Ƌ����𑪂�p
	float attackDistance = 300.0f;			//�͈͓��ōU�����邽�߂̕ϐ�
	float track = 1500.0f;					//�ǂ������邽�߂̕ϐ�
	float angle = 0.0f;
};

