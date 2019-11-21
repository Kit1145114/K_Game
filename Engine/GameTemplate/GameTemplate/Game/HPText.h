#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
#include"SpriteRender.h"

class HPText : public GameObject
{
public:
	HPText();
	~HPText();
	void Update();		//�A�b�v�f�[�g�B
private:
	SpriteRender* m_sprite;						//�X�v���C�g�����_�[�B
	CVector3 m_position = {0.0f,-200.0f,0.0f};	//HP��\��������ꏊ�̎w��B
	int wide = 500;
};

