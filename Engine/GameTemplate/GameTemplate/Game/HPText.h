#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
#include"SpriteRender.h"

class Player;
class HPText : public GameObject
{
public:
	HPText();
	~HPText();
	void Update();		//�A�b�v�f�[�g�B
	void HPUpdate();	//HP�������葝������B	
	void SetPlayerHP(float HP)
	{
		m_HP = HP;
	}
private:
	float m_HP = 0.0f;
	SpriteRender* m_sprite;						//�X�v���C�g�����_�[�B
	CVector3 m_position = {0.0f,-200.0f,0.0f};	//HP��\��������ꏊ�̎w��B
	CVector3 m_scale = {0.0f,0.0f,0.0f };		//HP�������葝�����肷��p
	CVector2 pivot = { 0.5f,0.5f };
};

