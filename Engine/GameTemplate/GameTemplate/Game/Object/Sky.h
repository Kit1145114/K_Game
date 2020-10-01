#pragma once
#include"graphics/RenderTarget.h"
#include"graphics/c3dmodel/C3DModelDraw.h"

class Sky : public GameObject
{
public:
	Sky();
	~Sky();
	bool Start()override;
	void Update()override;
	void Draw();
public:
	//�O������|�W�V������ݒ�B
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//�O������X�P�[����ݒ�B
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
private:
	SkinModel	SkyModel;						//�}�b�v�̃X�L�����f���B
	C3DModelDraw m_mapDraw;					//C3D���f���h���[�B
	CVector3 m_position = CVector3::Zero();	//�|�W�V����
	CVector3 m_scale = {1.4f,1.4f,1.4f};//CVector3::One();		//�傫��
};

