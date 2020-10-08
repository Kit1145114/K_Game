#pragma once
#include"../Game/graphics/SpriteRender.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class GameClear : public GameObject
{
public:
	GameClear();
	~GameClear();
	void Update() override;
	void Death();
private:
	//2D
	SpriteRender* m_sprite;
	//���y
	CSoundEngine m_soundEngine;
	CSoundSource m_bgm[2];
	//����
	float m_volume = 0.15f;
};

