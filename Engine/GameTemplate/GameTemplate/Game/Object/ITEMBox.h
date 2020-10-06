#pragma once
#include"Game/GameObjectManager.h"
#include"physics/PhysicsStaticObject.h"
#include"../GameSystem/GameConst.h"
#include "character/CharacterController.h"
#include"ITEM/RecoveryITEM.h"
#include"Chara/Player.h"

class RecoveryITEM;
class Player;
class ITEMBox : public GameObject
{
public:
	ITEMBox();
	~ITEMBox();
	enum boxstate
	{
		open,
		close,
		death
	};
	bool Start() override;					//
	void Update() override;					//�A�b�v�f�[�g�����B
	void Draw();							//�h���[�֐��B
	void State();							//�X�e�[�g
	void Delete();							//�����B
	void Open();							//�J���Ă鎞�̏���
	void Close();							//�߂�B
public:
	//�J���邩�ǂ���
	void SetIsOpen(bool is)
	{
		isOpen = is;
	}
	//�J���Ă܂����H
	bool GetIsOpen()
	{
		return isOpen;
	}
	//�L�����R���̎擾�B
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
public:
	//�|�W�V������n���p�B
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//�傫����n���p
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	//�v���C���[�̏���n���B
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
private:
	RecoveryITEM* RItem;
	Player* m_player;
	SkinModel	Box;							//�}�b�v�̃X�L�����f���B
	Animation box_anim;							//�A�j���[�V�����B
	AnimationClip box_animClip[m_AnimClipNum];	//�v���C���[�̃A�j���[�V�����N���b�v
	boxstate state;								//��ԁB
	//PhysicsStaticObject m_physicsStaticObj;		//�ÓI�I�u�W�F�N�g
	CharacterController m_charaCon;				//�L�����N�^�[�R���g���[���[

	CVector3 m_position		= CVector3::Zero();			//�|�W�V����
	CVector3 m_scale =		CVector3::Zero();			//�X�P�[���B
	CQuaternion m_rotation = CQuaternion::Identity();	//���[�e�[�V�����B
	bool isOpen = false;						//���̃t���O
	bool itemDrop_flag = false;					//�A�C�e�����o�����B
	float m_timer = 0.0f;						//����
	float m_deathTime = 1.0f;					//�����J������Ă��������܂ł̎��ԁB

		//�G�t�F�N�g
	Effekseer::Effect* m_effect[3] = { nullptr };
	Effekseer::Handle m_playEffectHandle = 10;
};

