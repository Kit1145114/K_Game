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
	//プレイヤーがマップ移動した後の体力を引き継ぎます。
	void SetPlayerHP(int hp)
	{
		m_playerHp = hp;
	}
private:
	//ステージ
	int m_stageNum;
	//スプライト
	SpriteRender* m_sprite;
	//ゲームだよ！！
	Game* game;
	//ひろち
	CQuaternion Rot = { 0.0f,1.0f,0.0f,0.0f };
	//プレイヤーの体力卍
	int m_playerHp = 0;
};

