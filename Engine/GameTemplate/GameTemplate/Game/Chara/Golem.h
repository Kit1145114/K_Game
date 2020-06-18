#pragma once
#include"Enemys.h"

class Golem : public Enemys
{
public:
	Golem();
	~Golem() {};
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
	float m_AttackTime = 0.0f;
	float m_Falltimer = 0.0f;
	float m_attack = 125.0f;
	CVector3 m_Attackdis = CVector3::Zero();
	Effekseer::Handle m_playEffectHandle = 1;
	bool loop = true;
};

