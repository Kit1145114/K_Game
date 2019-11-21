#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
#include"SpriteRender.h"

class HPText : public GameObject
{
public:
	HPText();
	~HPText();
	void Update();		//アップデート。
private:
	SpriteRender* m_sprite;						//スプライトレンダー。
	CVector3 m_position = {0.0f,-200.0f,0.0f};	//HPを表示させる場所の指定。
	int wide = 500;
};

