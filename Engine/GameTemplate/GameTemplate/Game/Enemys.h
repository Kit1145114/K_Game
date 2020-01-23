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
	virtual void Damage(int Damage) = 0;		//DAMAGE
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
	/// <summary>
	/// �{�X�p�̏�ԁB
	/// </summary>
	enum BossAnimState
	{
		bsIdle,			//�ҋ@�B
		bsTracking,		//�ǂ�������B
		bsDeath,		//You Are Dead�B
		bsSmallAttack,	//��U���B
		bsBigAttack		//���U���B
	};
	/// <summary>
	/// �G�l�~�[�ɏꏊ��n���B
	/// </summary>
	/// <param name="position">�|�W�V����</param>
	void SetPosition(CVector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]���[�邭��
	/// </summary>
	void SetRotation(CQuaternion rotation)
	{
		m_rotation = rotation;
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
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
	/// <summary>
	/// �G�l�~�[��n��Ƃ��Ƀp�����[�^�[����͂�����B
	/// </summary>
	/// <param name="HP">�q�b�g�|�C���g</param>
	/// <param name="Attack">�U����</param>
	/// <param name="Defense">�h���</param>
	/// <param name="Speed">��b���x</param>
	/// <param name="model">�G�l�~�[�̃��f��</param>
	void Init(float HP,float Attack, float Defense,float Speed);
protected:
	Player* m_player;									//�v���C���[�N���X�B
	SkinModel Model;									//�G�l�~�[�̃��f���B
	int m_HP = 0;										//�G�l�~�[��HP
	int m_MaxHP = 0;									//�G�l�~�[�̍ő�HP
	int m_ATK = 0;										//�G�l�~�[�̍U����
	int m_DEF = 0;										//�G�l�~�[�̖h���
	int m_SPD = 0;										//�G�l�~�[�̃X�s�[�h
	bool isDeath = false;								//�G�l�~�[�����񂾂��ǂ����B
	CVector3 m_position = CVector3::Zero();				//�G�l�~�[�̃|�W�V�����p�̃����o�ϐ�
	CVector3 m_moveSpeed = CVector3::Zero();			//�G�l�~�[�̈ړ��p�̃����o�ϐ�
	CVector3 m_scale = CVector3::Zero();				//�G�l�~�[�̑傫���p�̃����o�ϐ��B
	CVector3 m_toPlayer = CVector3::Zero();				//�v���C���[�Ɍ������x�N�g���p�B
	CVector3 m_forward = CVector3::AxisZ();				//<�G�l�~�[�̑O�������B
	CVector3 Move = CVector3::Zero();					//�G��������B�i�G�@�̎q�j
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�p�̃����o�ϐ��B
	CharacterController m_charaCon;						//�L�����R���B
	EnemyAnimState	e_state;							//�G�l�~�[�̏��			
	EnemyInitParam prm;									//�G�l�~�[�̃p�����[�^�[�������Ă��B
	Animation anim;										//�A�j��
	AnimationClip animClip[m_AnimClipNum];				//�A�j���N���b�v
};

