#pragma once
#include"graphics/Sprite.h"
#include"graphics/SpriteRender.h"

class Player;
class EnergyText : public GameObject
{
public:
	EnergyText();
	~EnergyText();
	void Update();		//アップデート。
	void EnergyUpdate();	//エナジー減ったり増えたり。	
	void SetPlayerEnergy(float Energy)
	{
		m_Energy = Energy;
	}
private:
	float m_Energy = 0.0f;							//エナジー用のメンバ変数。
	SpriteRender* m_sprite;							//スプライトレンダー。
	CVector3 m_position = {500.0f,-250.0f,0.0f };	//HPを表示させる場所の指定。
	CVector3 m_scale =	{ 0.0f,0.0f,0.0f };			//HP減ったり増えたりする用
};

