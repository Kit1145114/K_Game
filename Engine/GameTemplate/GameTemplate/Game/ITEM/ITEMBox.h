#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"
#include"../GameSystem/GameConst.h"
#include "character/CharacterController.h"

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
	bool GetIsOpen()
	{
		return isOpen;
	}
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}
private:
	SkinModel	Box;							//�}�b�v�̃X�L�����f���B
	Animation box_anim;							//�A�j���[�V�����B
	AnimationClip box_animClip[m_AnimClipNum];	//�v���C���[�̃A�j���[�V�����N���b�v
	boxstate state;								//��ԁB
	PhysicsStaticObject m_physicsStaticObj;		//�ÓI�I�u�W�F�N�g
	CharacterController m_charaCon;				//�L�����N�^�[�R���g���[���[

	CVector3 m_position		= CVector3::Zero();			//�|�W�V����
	CVector3 m_scale =		CVector3::Zero();			//�X�P�[���B
	CQuaternion m_rotation = CQuaternion::Identity();	//���[�e�[�V�����B
	bool isOpen = false;						//���̃t���O
};

