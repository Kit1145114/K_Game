#pragma once
#include"Enemys.h"

class Golem : public Enemys
{
public:
	Golem();
	~Golem();
	void Attack();					//�U��(���͖�����)
	void Damage(int Damage) override;		//DAMAGE�i���͖������j
	void Search();							//�v���C���[��������֐�
	void Update()override;					//�G�̍X�V���e���L��
	void Death();							//�G�l�~�[�����ʏ�������B
	void EnemyState();						//�G�l�~�[���X�e�C�g�ōs�����鏈������B
	void EMove();							//�G�l�~�[��������������B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//�A�j���[�V�����C�x���g�B
	void AttackRange();						//�U���ł��邩�̏����B
	void AttackStanby();					//�U���ҋ@����U���܂ŁB
	void AttackGap();						//�U����̌��̕����B
private:
	int m_playerToDamage = 51;				//�v���C���[�ɗ^����_���[�W�̒l�B
	float m_effectSpwanTime = 1.0f;			//�U���Ɉڂ�܂ł̎��ԗp�̕ϐ�
	float m_attackTime = 0.0f;				//�U���܂ł̎��Ԃ�}�邽�߂̕ϐ��B
	float m_goAttackTime = 3.0f;			//���̎��ԂɂȂ�����U�������B
	float m_falltimer = 0.0f;				//�|��Ă��鎞�Ԃ�}�邽�߂̕ϐ��B
	float m_fallLimitTime = 2.0f;			//���܂œ|��Ă��邩�����߂�ϐ��B
	float m_attackDeistance = 125.0f;		//�U����ԂɂȂ鋗���B
	CVector3 m_attackDis = CVector3::Zero();	//�U�����v���C���[�ɗ^����͈͂��v�Z���邽�߂̂���
	Effekseer::Handle m_playEffectHandle = 1;
};

