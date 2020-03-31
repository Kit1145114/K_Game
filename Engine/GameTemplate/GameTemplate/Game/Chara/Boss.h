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
	void Attack();							//�U��
	void AttackRange();						//�U���ł��邩�̏����B
	void Damage(int Damage);				//DAMAGE
	void Search();							//�v���C���[��������֐�
	void Update()		override;						//�G�̍X�V���e���L��
	void Death();							//�G�l�~�[�����ʏ�������B
	void EnemyState();						//�G�l�~�[���X�e�C�g�ōs�����鏈������B
	void EMove();							//�G�l�~�[��������������B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//�A�j���[�V�����C�x���g�B
	void HitMe();							//�_���[�W���󂯂邚....
private:
	BossAnimState boss_State;				//�{�X�̃X�e�[�g�B
	BossMode Mode;							//�{�X�̍U���p�̃X�e�[�g�B
	float track = 1500.0f;					//�ǂ������邽�߂̕ϐ�
};

