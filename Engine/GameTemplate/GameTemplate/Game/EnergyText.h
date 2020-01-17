#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
#include"SpriteRender.h"

class Player;
class EnergyText : public GameObject
{
public:
	EnergyText();
	~EnergyText();
	void Update();		//�A�b�v�f�[�g�B
	void EnergyUpdate();	//�G�i�W�[�������葝������B	
	void SetPlayerEnergy(float Energy)
	{
		m_Energy = Energy;
	}
private:
	float m_Energy = 0.0f;							//�G�i�W�[�p�̃����o�ϐ��B
	SpriteRender* m_sprite;							//�X�v���C�g�����_�[�B
	CVector3 m_position = { 0.0f,-250.0f,0.0f };	//HP��\��������ꏊ�̎w��B
	CVector3 m_scale =	{ 0.0f,0.0f,0.0f };			//HP�������葝�����肷��p
	CVector2 pivot = { 0.0f,0.5f };
};
