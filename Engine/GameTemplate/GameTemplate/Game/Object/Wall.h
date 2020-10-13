#pragma once
#include"character/CharacterController.h"
#include"physics/PhysicsStaticObject.h"
#include"../GameSystem/GameConst.h"

class PhysicsStaticObject;
class Wall : public GameObject
{
public:
	enum WallState
	{
		w_idle,
		w_move,
		w_death
	};
	Wall();
	~Wall();
	bool Start() override;
	void Update() override;
	void Move();
	void Draw();
	void State();
	void Death();
	void Idle();
	const void SetPosition(CVector3& pos)
	{
		m_position = pos;
	}
	const void SetRotation(CQuaternion& rot)
	{
		m_rotation = rot;
	}
	const void SetObjNum(int& num)
	{
		m_objNum = num;
	}
	const void SetMoveflag(bool flag) 
	{
		m_move_flag = flag;
	}
	int GetObjData() const
	{
		return m_objNum;
	}
private:
	SkinModel  m_wall;
	CharacterController m_charaCon;						//�L�����R���B
	CSoundSource m_se;									//SE
	CVector3 m_position = CVector3::Zero();				//�|�W�V����
	CVector3 m_scale = CVector3::One();					//�X�P�[���B
	CQuaternion m_rotation = CQuaternion::Identity();	//���[�e�[�V�����B
	PhysicsStaticObject m_physicsStaticObj;				//�ÓI�I�u�W�F�N�g
	Animation w_anim;									//�A�j���[�V�����B
	AnimationClip w_animClip[m_AnimClipNum];			//�v���C���[�̃A�j���[�V�����N���b�v
	WallState wallState;
	int m_objNum = 0;									
	bool m_move_flag = false;							//�����Ă�����
	bool m_death_flag = false;							//��
	float m_timer = 0.0f;								//��������
};

