#pragma once
#include "character/CharacterController.h"
#include"GameObjectManajer.h"
#include"physics/PhysicsGhostObject.h"
#include"Enemys.h"

class Enemys;
class Player : public GameObject
{					
public:
	/// <summary>
	/// �v���C���[�̃A�j���[�V�����p�̃X�e�[�g
	/// </summary>
	enum PlayerAnimClip 
	{
		plAnimClip_idle,		//�ҋ@
		plAnimClip_Walk,		//����
		plAnimClip_Atk			//�U��
	};

	/// <summary>
	/// �C���X�g���N�^
	/// </summary>
	Player();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();
	void Update();			//�v���C���[�̍X�V�֐�
	void Draw();			//�v���C���[�̕`�揈���B
	void Move();			//�v���C���[�̈ړ������B
	void MoveOperation();		//�v���C���[�̑���B
	void Rotation();		//�v���C���[�̉�]�����B
	void PlayerState();		//�v���C���[�̏��
	void PlayerAttack();	//�v���C���[�̍U����
	void Track();			//�G�l�~�[�T���B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//�A�j���[�V�����C�x���g�B
	/// <summary>
	/// �v���C���[�ɗ^����_���[�W�̐ݒ�B
	/// </summary>
	/// <param name="Damage">�_���[�W�i�G�̍U���́j</param>
	void SetDamage(float Damage)
	{
		HP -= Damage;
	}	
	/// <summary>
	/// �v���C���[�̃|�W�V������n���B
	/// </summary>
	/// <param name="m_position">�v���C���[�̃|�W�V����</param>
	CVector3 GetPosition()	const
	{
		return m_position;
	}
	/// <summary>
	/// �G�l�~�[��T���B(��)
	/// </summary>
	//void SetEnemys(Enemys * enemy)
	//{
	//	enemys = enemy;
	//}
	//
	void SetEnemysList(std::vector<Enemys*> GetEnemy)
	{
		m_goList = GetEnemy;
	}
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//�v���C���[�̑O�x�N�g�����擾�B
	void Forward();
private:
	SkinModel Gmodel;									//�X�L�����f���B
	Animation g_anim;
	Enemys* enemys;
	std::vector<Enemys*> m_goList;	//�Q�[���I�u�W�F�N�g�̃��X�g
	//�v���C���[�̃A�j���[�V����
	AnimationClip g_animClip[3];						//�v���C���[�̃A�j���[�V�����N���b�v
	CVector3 m_position = CVector3::Zero();				//�v���C���[�̃|�W�V�����B	
	CVector3 m_moveSpeed = CVector3::Zero();			//�v���C���[�̈ړ��p�̕ϐ�
	CVector3 m_scale = {3.0f,3.0f,3.0f};				//�v���C���[�̑傫���p�̕ϐ�
	CQuaternion m_rotation = CQuaternion::Identity();	//�v���C���[�̎���]�p�̕ϐ�
	PlayerAnimClip plClip;								//�v���C���[�̃A�j���[�V�����X�e�[�g
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[
	PhysicsGhostObject m_PhyGhostObj;					//�S�[�X�g
	CVector3 m_forward = CVector3::AxisZ();				//�v���C���[�̑O�������B
	CMatrix Rot = CMatrix::Identity();					//�v���C���[��
	CVector3 toPlayer = CVector3::Zero();
	float HP = 0;									// �v���C���[��HP
	float ATK = 50.0f;									// �v���C���[�̍U����
	float DEF = 0;									/// �v���C���[�̖h���
	float None = 0.0f;								//0�����������Ƃ��Ɏg���܂��B
	float NSpeed = 1.0f;							//�ʏ�̃X�s�[�h�B
	float SPeed2 = 2.0f;							//�Q�{�̃X�s�[�h�B
	float JumpPower = 400.0f;						//�W�����v�����Ƃ��̒l
	//float Gravity = 100.0f;							//�v���C���[�̂�����d�́B
};
//	�N���X�A�֐��̓R�[�f�B���O���[��	�A�b�p�[�L������MoveCount
//	�ϐ��́@�A���_�[�L������			m_moveCount		
//�@�����o�ϐ��ɂ�m_��t����B
//	���ꂷ�邱�Ƃ��厖�B
//�����A�ϐ��A�߂�l�A�����o�ϐ��ɃR�����g
/// <summary>
/// �v���C���[�A�^�O�B
/// </summary>
/// <remarks>�ڍׂȏ����������߂̃^�O</remarks>
/// <param name="num">���������B</param>
/// <param name="value">����̗�</param>
/// <returns>���ۂɓ���������</returns>
/// <example>�R�[�h�̐���</example>
/// <include file='' path='[@name=""]'/>