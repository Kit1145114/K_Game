#pragma once
#include"graphics/SpriteRender.h"

class Game;
class ChangeScreen : public GameObject
{
public:
	ChangeScreen();
	~ChangeScreen();
	void Update() override;
	void Death();
private:
	//ステージ
	int m_stageNum;
	//スプライト
	SpriteRender* m_sprite;
	//ゲームだよ！！
	Game* game;
	//ひろち
	CQuaternion Rot = { 0.0f,1.0f,0.0f,0.0f };
};

