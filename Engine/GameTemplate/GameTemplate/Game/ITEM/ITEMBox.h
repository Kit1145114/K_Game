#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"
#include"GameConst.h"
#include "character/CharacterController.h"

class ITEMBox : public GameObject
{
public:
	ITEMBox();
	~ITEMBox();
	enum boxstate
	{
		open
	};
	void Update();							//�A�b�v�f�[�g�����B
	void Draw();							//�h���[�֐��B
	void State();							//�X�e�[�g
public:
	//�J���邩�ǂ���
	void SetIsOpen(bool is)
	{
		isOpen = is;
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

