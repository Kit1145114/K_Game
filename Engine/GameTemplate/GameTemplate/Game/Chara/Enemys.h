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
		isDeath_flag = true;
	}
	//�X�e�[�g�̏������z�֐�
	virtual void EnemyState() = 0;
	virtual void Draw() override final; 				//�G�̕`�揈���B
	//���������͓G�̋��ʂ��鏈�����L�ځB�Ăяo����Enemys::�Z�Z;

	void ViewingAngle();		//�G�l�~�[�̎���p�B
	void VectorAcquisition();	//�G�l�~�[�̃x�N�g�����擾���邽�߂̊֐��B
	void Rotation();			//�G�l�~�[�̉�]�����B
	void EnemyEffect();			//�G�l�~�[�̃G�t�F�N�g�B
public:
	//�G�l�~�[�����񂾂��ǂ�����Ԃ��B
	bool GetisDeath() const
	{
		return isDeath_flag;
	}
	struct EnemyInitParam
	{
		int HP;
		int ATK;
		int DEF;
		float SPD;
		//SkinModel model;
	};
	/// <summary>
	/// �G�̏�ԁB
	/// </summary>
	enum EnemyAnimState
	{
		esIdle,			//�ҋ@�B
		esTracking,		//�ǂ�������B
		esAttack,		//�U���B
		esDeath,		//You Are Dead�B
		esAttackMe,		//�U���󂯂��B
		esAttackGap,	//�U���̌�
		esStandbyAttack	//�U���ɓ���܂�
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
		bsHitMe,		//�U�����󂯂�
	};
	/// <summary>
	/// �G�l�~�[�ɏꏊ��n���B
	/// </summary>
	/// <param name="position">�|�W�V����</param>
	void SetPosition(const CVector3& position)
	{
		m_position = position;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// �L�����R���ɑ���B
	/// </summary>
	void SetChaPos(const CVector3& pos)
	{
		m_charaCon.SetPosition(pos);
	}
	/// <summary>
	/// ��]���[�邭��
	/// </summary>
	void SetRotation(const CQuaternion& rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(const CVector3& scale) 
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
	bool GetIsDead() const
	{
		return isDeath_flag;
	}
	CharacterController& GetCharaCon() 
	{
		return m_charaCon;
	}
	void SetHitMe(bool isHit)
	{
		isHitMe_flag = isHit;
	}
	void SetObjNum(int num)
	{
		m_objNum = num;
	}
	int GetObjData() const
	{
		return m_objNum;
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
	Player* m_player = nullptr;									//�v���C���[�N���X�B
	SkinModel Model;									//�G�l�~�[�̃��f���B
	float m_HP = 0.0f;										//�G�l�~�[��HP
	float m_MaxHP = 0.0f;									//�G�l�~�[�̍ő�HP
	int m_ATK = 0;										//�G�l�~�[�̍U����
	int m_DEF = 0;										//�G�l�~�[�̖h���
	int m_SPD = 0;										//�G�l�~�[�̏����X�s�[�h�B
	int m_objNum = 0;
	//�G�l�~�[�̃X�s�[�h
	float m_walkingDistance = 300.0f;						//���s�������B
	float m_flyDistance = 500.0f;							//��s������
	float m_angle = 0.0f;
	float m_enemyTrack = 1250.0f;						//�ǂ�������͈́B
	float m_attackDistance = 125.0f;					//�͈͓��ōU�����邽�߂̕ϐ�
	float m_Kyori = 500.0f;								
	float m_timer = 0.0f;								//�^�C�}�[�B
	float m_fowndAngle = 0.4f;							//������p�x
	bool isDeath_flag = false;							//�G�l�~�[�����񂾂��ǂ����B
	bool isHitMe_flag = false;							//�U���󂯂��B
	bool isAttack_flag = false;							//�G�l�~�[���ǂ��������B
	bool isTrack_flag = false;							//������͕ʂ̒ǂ�������t���O�B
	bool isDestination_flag = true;						//��񂾂��ړI�n�����߂����̂ŁB
	bool isEffectLoop_flag = true;			//�G�t�F�N�g���ʐ������Ȃ��悤�Ɏg���t���O�B
	CVector3 m_position= CVector3::Zero();				//�G�l�~�[�̃|�W�V�����p�̃����o�ϐ�
	CVector3 m_moveSpeed = CVector3::Zero();			//�G�l�~�[�̈ړ��̃X�s�[�h����B
	CVector3 m_scale = CVector3::One();					//�G�l�~�[�̑傫���p�̃����o�ϐ��B
	CVector3 m_toPlayer = CVector3::Zero();				//�v���C���[�Ɍ������x�N�g���p�B
	CVector3 m_forward = CVector3::AxisZ();				//<�G�l�~�[�̑O�������B
	CVector3 m_diff = CVector3::Zero();
	CVector3 m_move = CVector3::Zero();					//�G��������B�i�G�@�̎q�j
	CVector3 m_destination = CVector3::Zero();			//�����ꏊ�܂ŁB
	CVector3 m_efePos;									//�G�t�F�N�g�̃|�W�V����
	CVector3 m_efeRot;									//�G�t�F�N�g�̉�]
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�p�̃����o�ϐ��B
	CharacterController m_charaCon;						//�L�����R���B
	EnemyAnimState	e_state;							//�G�l�~�[�̏�ԁB			
	EnemyInitParam prm;									//�G�l�~�[�̃p�����[�^�[�������Ă��B
	Animation anim;										//�A�j���B
	AnimationClip animClip[m_AnimClipNum];				//�A�j���N���b�v�B
	//��
	CSoundSource m_se[10];								//���B
	//�G�t�F�N�g
	Effekseer::Effect* m_attackEffect = nullptr;		//��{�U�����̃G�t�F�N�g�B
	Effekseer::Handle m_playEffectHandle = 1;
};

