#pragma once
#include"Enemys.h"
#include"ITEM/ITEMBox.h"
#include"character/CharacterController.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"
#include"graphics/2Dgraphics/Font.h"
#include"physics/PhysicsGhostObject.h"

class Enemys;
class ITEMBox;
class Player : public GameObject
{					
public:
	/// <summary>
	/// �v���C���[�̃X�e�[�g
	/// </summary>
	enum PlayerStateClip
	{
		pl_idle,		//�ҋ@
		pl_Walk,		//����
		pl_FlyMove,		//���V�ړ��B
		pl_Atk,			//�U��
		pl_Combo,		//�R���{���B
		pl_Death		//���S
	};
	//�v���C���[�̃G�i�W�[�B
	enum ENERGYState
	{
		ene_Charge,			//charge��
		ene_Full			//������
	};
	/// <summary>
	/// �C���X�g���N�^
	/// </summary>
	Player();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();
	void Update()override;			//�v���C���[�̍X�V�֐�
	void Draw();			//�v���C���[�̕`�揈���B
	void Render()override;	//�����_�[
	void FontRender() override;  //�����\��
	void Move();			//�v���C���[�̈ړ������B
	void MoveOperation();		//�v���C���[�̑���B
	void Rotation();		//�v���C���[�̉�]�����B
	void PlayerState();		//�v���C���[�̏��
	void PlayerAttack();	//�v���C���[�̍U����
	void Track();			//�G�l�~�[�T���B
	void Energy();			//�G�l���M�[�p�̊֐��B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);		//�A�j���[�V�����C�x���g�B
	/// <summary>
	/// �v���C���[�ɗ^����_���[�W�̐ݒ�B
	/// </summary>
	/// <param name="Damage">�_���[�W�i�G�̍U���́j</param>
	void Damage(int Damage);
	bool GetIsDead()
	{
		return m_isdeath;
	}
	/// <summary>
	/// �v���C���[�̃|�W�V������n���B
	/// </summary>
	/// <param name="m_position">�v���C���[�̃|�W�V����</param>
	const CVector3& GetPosition()	
	{
		return m_position;
	}
	const CVector3& GetRookEnemyPos()
	{
		return m_enemyPos;
	}
	void SetEnemysList(std::vector<Enemys*> GetEnemy)
	{
		m_enemysList = GetEnemy;
	}

	void SetBox(ITEMBox* box)
	{
		ItemBox = box;
	}
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	//HP�o�[�p�̎cHP��n���֐��B
	int GetPlayerHP()
	{
		return HP;
	}
	//�G�i�W�[�p�̎c�G�i�W�[��n���֐��B
	int GetPlayerEnergy()
	{
		return ENERGY;
	}
	bool GetIsRooking()
	{
		return m_isRookOn;
	}
	//�R���{�֘A�����Z�b�g
	void ComboReset()
	{
		playerState = pl_idle;
		m_isCombo = false;
		m_ComboNow = false;
	}
	//��
	void Sound(int SoundNum)
	{
		m_se[SoundNum].Play(false);
	}
	void StopSound(int SoundNum)
	{
		m_se[SoundNum].Stop();
	}
	//�v���C���[�̑O�x�N�g�����擾�B
	void Forward();
	//�G�����b�N�I������Ƃ��̏����B
	void RookOnEnemys();
	//�v���C���[���R���{�U�������鏈���B
	void ComboAttack();
private:
	SkinModel Gmodel;									//�X�L�����f���B
	Animation g_anim;									//�A�j���[�V�����B
	ITEMBox* ItemBox = nullptr;									//�A�C�e���p�̂���
	Enemys* enemys;										//�G�̏��𓾂邽�߂�class�I�Ȃ��́B
	std::vector<Enemys*> m_enemysList;					//�G�l�~�[��Vector�z��
	AnimationClip g_animClip[m_AnimClipNum];			//�v���C���[�̃A�j���[�V�����N���b�v
	//��
	CSoundSource m_se[2];								//SE
	CVector3 m_position = CVector3::Zero();				//�v���C���[�̃|�W�V�����B	
	CVector3 m_enemyPos = CVector3::Zero();				//�G�l�~�[�̃|�W�V�����擾�p�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�v���C���[�̈ړ��p�̕ϐ�
	CVector3 m_scale = CVector3::One();				//�v���C���[�̑傫���p�̕ϐ�
	CVector3 box_scale = { 50.0f,50.0f,50.0f };			//�{�b�N�X�傫��
	CVector3 diff = { 0.0f,0.0f,0.0f };					//�G�l�~�[�ƃv���C���[�̋���
	CQuaternion m_rotation = CQuaternion::Identity();	//�v���C���[�̎���]�p�̕ϐ�
	PlayerStateClip playerState;						//�v���C���[�̃X�e�[�g
	ENERGYState	playerENE;								//�v���C���[�̃G�i�W�[�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[
	PhysicsGhostObject m_PhyGhostObj;					//�S�[�X�g
	CVector3 m_forward = CVector3::AxisZ();				//�v���C���[�̑O�������B
	CMatrix Rot = CMatrix::Identity();					//�v���C���[��
	CVector3 toPlayer = CVector3::Zero();				//�O������}��悤�̂��́B
	Font m_font;									//���O�B
	int HP = 0;											//�v���C���[��HP
	int ATK = 0;										//�v���C���[�̍U����
	int DEF = 0;										//�v���C���[�̖h���
	int ENERGY = 0;										//�v���C���[�̃u�[�X�g�e�ʁB
	int ENERGYFLUCT = 5;								//�G�i�W�[�ϓ��l�B
	int MAXENERGY = 300;								//�ő�G�i�W�[�B
	int MINENERGY = 0;									//�ŏ��G�l���M�[�B
	float None = 0.0f;									//0�����������Ƃ��Ɏg���܂��B
	float NSpeed = 1.0f;								//�ʏ�̃X�s�[�h�B
	float SPeed2 = 3.5f;								//�Q�{�̃X�s�[�h�B
	float JumpPower = 40.0f;							//�W�����v�����Ƃ��̒l
	bool m_isdeath = false;								//���S����B
	bool m_isRookOn = false;							//�G�����b�N�I�����Ă��邩�ǂ����B
	bool m_isCombo = false;								//�R���{���邩�H
	bool m_ComboNow = false;
};
/// <summary>
/// �v���C���[�A�^�O�B
/// </summary>
/// <remarks>�ڍׂȏ����������߂̃^�O</remarks>
/// <param name="num">���������B</param>
/// <param name="value">����̗�</param>
/// <returns>���ۂɓ���������</returns>
/// <example>�R�[�h�̐���</example>
/// <include file='' path='[@name=""]'/>