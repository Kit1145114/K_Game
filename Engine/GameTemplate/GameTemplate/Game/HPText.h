#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
#include"SpriteRender.h"
#include"Player.h"

class Player;
class HPText : public GameObject
{
public:
	HPText();
	~HPText();
	void Update();		//アップデート。
	void HPUpdate();	//HP減ったり増えたり。	
	void SetPlayerHP(float HP)
	{
		m_HP = HP;
	}
private:
	int time = 300;
	float wide = 50.0f;
	float m_HP = 0.0f;
	SpriteRender* m_sprite;						//スプライトレンダー。
	CVector3 m_position = {0.0f,-200.0f,0.0f};	//HPを表示させる場所の指定。
	CVector3 m_scale = {0.0f,0.0f,0.0f };		//HP減ったり増えたりする用
	CVector2 pivot = { 0.5f,0.5f };
};

