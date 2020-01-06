#pragma once
#include"GameObjectManajer.h"
#include"character/CharacterController.h"
#include"Player.h"
#include"GameConst.h"

class Enemy;
class Player;

class Enemys : public GameObject
{
public:
	 Enemys();
	virtual ~Enemys();

	//virtual void Attack() = 0;					//�U��
	virtual void Damage(float Damage) = 0;		//DAMAGE
	//�G�l�~�[�����S����ɂ���B
	virtual void Death()
	{
		isDeath = true;
	}

	virtual void EnemyState() = 0;

	//�G�l�~�[�����񂾂��ǂ�����Ԃ��B
	bool GetisDeath()
	{
		return isDeath;
	}

	struct EnemyInitParam
	{
		float HP;
		float ATK;
		float DEF;
		float SPD;
		//SkinModel model;
	};
	/// <summary>
	/// �G�̏�ԁB
	/// </summary>
	enum EnemyAnimState
	{
		esIdle,		//�ҋ@�B
		esTracking,	//�ǂ�������B
		esAttack,	//�U���B
		esDeath		//You Are Dead�B
	};
	void SetEnemyID(int ID)
	{
		m_ID = ID;
	}
	/// <summary>
	/// �G�l�~�[�ɏꏊ��n���B
	/// </summary>
	/// <param name="position">�|�W�V����</param>
	void SetPosition(CVector3 position)
	{
		m_position = position;
	}
	//�|�W�V�������炦�܂��B
	CVector3 GetPosition()	const
	{
		return m_position;
	}
	//�v���C���[...���������Ă܂��B
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	//���S���ǂ����B
	bool GetIsDead()
	{
		return isDeath;
	}
	/// <summary>
	/// �G�l�~�[��n��Ƃ��Ƀp�����[�^�[����͂�����B
	/// </summary>
	/// <param name="HP">�q�b�g�|�C���g</param>
	/// <param name="Attack">�U����</param>
	/// <param name="Defense">�h���</param>
	/// <param name="Speed">��b���x</param>
	/// <param name="model">�G�l�~�[�̃��f��</param>
	void Init(float HP,float Attack, float Defense,float Speed /*,SkinModel model*/);
	//bool Start();
	//void Update();
	//void Draw();
protected:
	/// <summary>
	/// �v���C���[�N���X�B
	/// </summary>
	Player* m_player;
	/// <summary>
	/// �G�l�~�[�̃��f���B
	/// </summary>
	SkinModel m_sm;
	/// <summary>
	/// �G�l�~�[��HP
	/// </summary>
	float m_HP = 0;
	/// <summary>
	/// �G�l�~�[�̍ő�HP
	/// </summary>
	float m_MaxHP = 0;
	/// <summary>
	/// �G�l�~�[�̍U���́B
	/// </summary>
	float m_ATK = 0;
	/// <summary>
	/// �G�l�~�[�̖h��́B
	/// </summary>
	float m_DEF = 0;
	/// <summary>
	/// �G�l�~�[�̊�b���x�B
	float m_SPD = 0;
	/// </summary>

	int m_ID = 0;
	CVector3 m_position = CVector3::Zero();					/// �G�l�~�[�̃|�W�V�����p�̃����o�ϐ�
	CVector3 m_moveSpeed = CVector3::Zero();				/// �G�l�~�[�̈ړ��p�̃����o�ϐ�
	CQuaternion m_rotation = CQuaternion::Identity();		//��]�p�̃����o�ϐ��B
	/// <summary>
	/// �G�l�~�[�����񂾂��ǂ����̕ϐ�
	/// </summary>
	bool isDeath = false;
	CharacterController m_charaCon;							// �L�����R���B
	EnemyAnimState	e_state;								//�G�l�~�[�̏��			
	CVector3 Move = CVector3::Zero();						//�G��������B�i�G�@�̎q�j
};

