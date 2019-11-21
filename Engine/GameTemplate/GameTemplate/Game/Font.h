#pragma once
#include "GameObjectManajer.h"
class Font : public GameObject
{
public:
	Font();
	~Font();
private:
	DirectX::SpriteBatch*	m_spriteBatch = nullptr;	//!<スプライトバッチ。
};

