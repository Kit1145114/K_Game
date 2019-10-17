#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"

class PhysicsStaticObject;
class MAP	:	public GameObject
{
public:
	/// <summary>
	/// �C���X�g���N�^
	/// </summary>
	MAP();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MAP();
	void Update();
	void Draw();
private:
	SkinModel Map;	//�}�b�v�̃X�L�����f���B
	PhysicsStaticObject m_PSO;				//�ÓI�I�u�W�F�N�g
	CVector3 position = CVector3::Zero();
};

