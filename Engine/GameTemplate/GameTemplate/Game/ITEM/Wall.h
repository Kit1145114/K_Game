#pragma once
#include"character/CharacterController.h"
#include"physics/PhysicsStaticObject.h"

class PhysicsStaticObject;
class Wall : public GameObject
{
public:
	Wall();
	~Wall();
	bool Start() override;
	void Update() override;
	void Draw();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModel  m_wall;
	CharacterController m_charaCon;						//�L�����R���B
	CVector3 m_position = CVector3::Zero();				//�|�W�V����
	CVector3 m_scale = CVector3::One();				//�X�P�[���B
	CQuaternion m_rotation = CQuaternion::Identity();	//���[�e�[�V�����B
	PhysicsStaticObject m_physicsStaticObj;	//�ÓI�I�u�W�F�N�g
};

