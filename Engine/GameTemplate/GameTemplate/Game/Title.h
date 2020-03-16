#pragma once
#include"GameObjectManajer.h"
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
};

