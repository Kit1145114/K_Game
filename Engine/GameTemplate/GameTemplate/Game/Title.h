#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"
#include"../Game/graphics/SpriteRender.h"
#include"GameConst.h"

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
	/// <summary>
	/// 更新関数。
	/// </summary>
	void Update();
	/// <summary>
	///描画関数
	/// </summary>
	void Draw(){};
	/// <summary>
	/// 終焉
	/// </summary>
	 void Death();
	 const void SetStageNum(StageNum& num)
	 {
		 stagenum = num;
	 }
private:
	//ステージ
	int m_stageNum;
	StageNum stagenum;
	//スプライト
	SpriteRender* m_sprite;
	//ゲームだよ！！
	Game* game;
	//ひろち
	CQuaternion Rot = {0.0f,1.0f,0.0f,0.0f};
};

