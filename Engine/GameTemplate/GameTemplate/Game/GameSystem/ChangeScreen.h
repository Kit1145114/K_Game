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
	int m_stageNum = 0;
	//スプライト
	SpriteRender* m_sprite;
	//ゲームだよ！！
	Game* game = nullptr;
	//ひろち
	CQuaternion Rot = { 0.0f,1.0f,0.0f,0.0f };
	//プレイヤーの体力卍
	int m_playerHp = 0;
	//時間で遷移するので時間用の変数
	float m_time = 0.0f;
	//遷移時間の変数
	float m_changeTime = 1.5f;
};

