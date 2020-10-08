#pragma once
#include"Enemys.h"
#include"physics/PhysicsGhostObject.h"

class StoneGolem : public Enemys
{
public:
	StoneGolem();
	~StoneGolem();
	void Attack();							//�U��
	void AttackRange();						//�U���ł��邩�̏����B
	void Damage(int Damage)override;		//�_���[�W	
	void Search();							//�v���C���[��������֐�
	void Update()		override;			//�G�̍X�V���e���L��
	void Death();							//�G�l�~�[�����ʏ�������B
	void EnemyState();						//�G�l�~�[���X�e�C�g�ōs�����鏈������B
	void EMove();							//�G�l�~�[��������������B
	void AttackAfter();						//�U����B
	void HitPlayerObj();
private:
	float m_attackTime = 0.0f;
	float m_objPosAdd = 2.5f;
	float m_trackToPlayer = 1000.0f;					//�G�l�~�[�ɂ���Ēǂ�������͈͂��ς�邽�߁B
	Effekseer::Handle m_playEffectHandle = 2;
	CVector3 m_efePos = CVector3::Zero();
	CVector3 diff = CVector3::Zero();
	bool SoundMakeFlag = true;
	PhysicsGhostObject m_PhyGhostObj;					//�S�[�X�g
	CVector3 box_scale = { 10.0f,10.0f,650.0f };		//�{�b�N�X�傫��
	const float m_r = 5.0f;								//���񂿂��̔��a
	const float m_length = 1200.0f;						//������������Ƌ�����ς�����B
};

