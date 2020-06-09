#pragma once
#include"../Game/graphics/SpriteRender.h"
#include"sound/SoundSource.h"
#include"graphics/2Dgraphics/Font.h"

class GameOver : public GameObject
{
public:
	GameOver();
	~GameOver();
	bool Start() override;
	void Update() override;
	void Flag();				//フラグ関係の処理
	void Death();				//死ぬか生きるか。
	void FontRender();			//２Ⅾのフォント出してます。
private:
	//2D
	SpriteRender* m_sprite;
	//CSoundSource m_bgm[2];
	Font m_font;									//フォント。
	bool m_continueFlag = false;					//続けるかのフラグ。
};

