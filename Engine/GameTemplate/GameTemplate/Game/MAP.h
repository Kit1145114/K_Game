#pragma once
#include"GameObjectManajer.h"
#include"physics/PhysicsStaticObject.h"
#include"RenderTarget.h"
#include"graphics/c3dmodel/C3DModelDraw.h"


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
	SkinModel Map;							//�}�b�v�̃X�L�����f���B
	C3DModelDraw m_mapDraw;					//C3D���f���h���[�B
	PhysicsStaticObject m_physicsStaticObj;	//�ÓI�I�u�W�F�N�g
	CVector3 position = CVector3::Zero();	//�|�W�V����
};

