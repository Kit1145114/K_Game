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
	enum BossFear
	{
		NO,								//��낯��B
		YES								//��낯�Ȃ��B
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
	void Fear();							//��낯�邩��낯�Ȃ����B
	void EnemyEffect() override;			//�U���̃G�t�F�N�g�B
private:
	BossAnimState boss_State;				//�{�X�̃X�e�[�g�B
	BossMode Mode;							//�{�X�̍U���p�̃X�e�[�g�B
	BossFear bossFear;						//�{�X���Ђ�ނ��̃X�e�[�g
	int m_hitAttack = 0;					//�U�����󂯂��񐔁B
	int m_maxHitAttack = 0;					//���ގ��Ɏg�p�B
	float track = 4500.0f;					//�ǂ������邽�߂̕ϐ�
	float m_waitTime = 0.0f;				//�U����̑ҋ@����
	bool fearAdd_flag = true;				//�t���O

	Effekseer::Handle m_playEffectHandle = 10;
};

