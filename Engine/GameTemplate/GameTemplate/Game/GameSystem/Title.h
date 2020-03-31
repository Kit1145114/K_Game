#pragma once
#include"graphics/SpriteRender.h"
#include"GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"

class Game;
class Title : public GameObject
{
public:
	//ステージ。
	/// <summary>
	/// インストラクタ。
	/// </summary>
	Title();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Title();
	//スタート関数。
	bool Start();
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 終焉
	/// </summary>
	 void Death();

private:
	//ステージ
	StageNum Stage;
	//スプライト
	SpriteRender* m_sprite;
	//ひろち
	CQuaternion Rot = { 0.0f,1.0f,0.0f,0.0f };
	//音楽
	CSoundEngine m_soundEngine;
	CSoundSource m_bgm[2];
};

