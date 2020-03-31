#pragma once
#include"physics/PhysicsStaticObject.h"
#include"RenderTarget.h"
#include"graphics/c3dmodel/C3DModelDraw.h"
#include"level/Level.h"

class Enemys;
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
	void SetPosition(CVector3 pos)
	{
		position = pos;
	}
private:
	SkinModel	Map;						//�}�b�v�̃X�L�����f���B
	Level		mapLevel;					//�}�b�v�̃��x���B�B�B�B�B�B
	std::vector<Enemys*> m_enemysList;		//�G�l�~�[�I�u�W�F�N�g�̃��X�g
	C3DModelDraw m_mapDraw;					//C3D���f���h���[�B
	PhysicsStaticObject m_physicsStaticObj;	//�ÓI�I�u�W�F�N�g
	CVector3 position = CVector3::Zero();	//�|�W�V����
};

