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
	int time = 300;
	float wide = 50.0f;
	SpriteRender* m_sprite;						//�X�v���C�g�����_�[�B
	CVector3 m_position = {0.0f,-200.0f,0.0f};	//HP��\��������ꏊ�̎w��B
	CVector3 m_scale = {0.0f,0.0f,0.0f };		//HP�������葝�����肷��p
};

