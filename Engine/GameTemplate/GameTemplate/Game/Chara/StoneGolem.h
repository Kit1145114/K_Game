#pragma once
#include"Enemys.h"
#include"physics/PhysicsGhostObject.h"

class StoneGolem : public Enemys
{
public:
	StoneGolem();
	~StoneGolem() {};
	void Attack();							//�U��
	void AttackRange();						//�U���ł��邩�̏����B
	void Damage(int Damage)override;		//�_���[�W	
	void Search();							//�v���C���[��������֐�
	void Update()		override;			//�G�̍X�V���e���L��
	void Death();							//�G�l�~�[�����ʏ�������B
	void EnemyState();						//�G�l�~�[���X�e�C�g�ōs�����鏈������B
	void EMove();							//�G�l�~�[��������������B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//�A�j���[�V�����C�x���g�B
	void HitMe();							//�������ł��B
	void AttackAfter();						//�U����B
	void HitPlayerObj();
private:
	float m_attackTime = 0.0f;
	float m_objPosAdd = 2.5f;
	Effekseer::Handle m_playEffectHandle = 2;
	CVector3 m_efePos = CVector3::Zero();
	CVector3 diff = CVector3::Zero();
	bool loop = true;
	PhysicsGhostObject m_PhyGhostObj;					//�S�[�X�g
	CVector3 box_scale = { 10.0f,10.0f,700.0f };		//�{�b�N�X�傫��
};

