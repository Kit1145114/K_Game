#pragma once
#include"graphics/SpriteRender.h"
#include"GameConst.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"
#include"../GameSystem/Fade.h"

class Game;
class Fade;
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
	//フェード
	Fade* m_Fade = nullptr;
	//スプライト
	SpriteRender* m_sprite;
	//回転
	CQuaternion Rot = { 0.0f,1.0f,0.0f,0.0f };
	//音楽
	CSoundEngine m_soundEngine;
	CSoundSource m_bgm[2];
	//ステージ番号
	int m_stageNum = 0;
};

