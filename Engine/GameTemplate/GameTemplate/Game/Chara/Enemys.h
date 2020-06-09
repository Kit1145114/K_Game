#pragma once
#include"character/CharacterController.h"
#include"Player.h"
#include"GameSystem/GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class Player;
class Enemys : public GameObject
{
public:
	 Enemys();
	virtual ~Enemys();
	virtual void Damage(int Damage) = 0;			//DAMAGE
	//�G�l�~�[�����S����ɂ���B
	virtual void Death()
	{
		isDeath = true;
	}
	//�X�e�[�g�̏������z�֐�
	virtual void EnemyState() = 0;
	//���������͓G�̋��ʂ��鏈�����L�ځB�Ăяo����Enemys::�Z�Z;
	void Draw();				//�G�̕`�揈���B
	void ViewingAngle();		//�G�l�~�[�̎���p�B
	void VectorAcquisition();	//�G�l�~�[�̃x�N�g�����擾���邽�߂̊֐��B
	void Rotation();			//�G�l�~�[�̉�]�����B

public:
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
		esDeath,	//You Are Dead�B
		esAttackMe	//�U���󂯂��B
	};
	/// <summary>
	/// �{�X�p�̏�ԁB
	/// </summary>
	enum BossAnimState
	{
		bsIdle,			//�ҋ@�B
		bsWalkTracking,	//�����Ēǂ�������B
		bsFlyTracking,	//��s�Œǂ�������B
		bsDeath,		//You Are Dead�B
		bsSmallAttack,	//��U���B
		bsBigAttack,	//���U���B
		bsHitMe		//�U�����󂯂�
	};
	/// <summary>
	/// �G�l�~�[�ɏꏊ��n���B
	/// </summary>
	/// <param name="position">�|�W�V����</param>
	void SetPosition(CVector3 position)
	{
		m_position = position;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// �L�����R���ɑ���B
	/// </summary>
	void SetChaPos(CVector3 pos)
	{
		m_charaCon.SetPosition(pos);
	}
	/// <summary>
	/// ��]���[�邭��
	/// </summary>
	void SetRotation(CQuaternion rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
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
	void SetHitMe(bool isHit)
	{
		isHitMe = isHit;
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
	float walkingDistance = 450.0f;						//���s�������B
	float flyDistance = 500.0f;							//��s������
	float m_angle = 0.0f;
	float m_enemytrack = 600.0f;						//�ǂ�������͈́B
	float attackDistance = 150.0f;						//�͈͓��ōU�����邽�߂̕ϐ�
	float m_Kyori = 500.0f;
	bool isDeath = false;								//�G�l�~�[�����񂾂��ǂ����B
	bool isHitMe = false;								//�U���󂯂��B
	bool isTracking = false;							//�G�l�~�[���ǂ��������B
	CVector3 m_position= CVector3::Zero();				//�G�l�~�[�̃|�W�V�����p�̃����o�ϐ�
	CVector3 m_moveSpeed = CVector3::Zero();			//�G�l�~�[�̈ړ��p�̃����o�ϐ�
	CVector3 m_scale = CVector3::One();				//�G�l�~�[�̑傫���p�̃����o�ϐ��B
	CVector3 m_toPlayer = CVector3::Zero();				//�v���C���[�Ɍ������x�N�g���p�B
	CVector3 m_forward = CVector3::AxisZ();				//<�G�l�~�[�̑O�������B
	CVector3 m_diff = CVector3::Zero();
	CVector3 Move = CVector3::Zero();					//�G��������B�i�G�@�̎q�j
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�p�̃����o�ϐ��B
	CharacterController m_charaCon;						//�L�����R���B
	EnemyAnimState	e_state;							//�G�l�~�[�̏��			
	EnemyInitParam prm;									//�G�l�~�[�̃p�����[�^�[�������Ă��B
	Animation anim;										//�A�j��
	AnimationClip animClip[m_AnimClipNum];				//�A�j���N���b�v
	//��
	CSoundSource m_se[3];
};

