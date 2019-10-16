#pragma once
#include"GameObjectManajer.h"
#include"graphics/Sprite.h"

class Game;
class Title : public GameObject
{
public:
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
	void Draw();
	/// <summary>
	/// 終焉
	/// </summary>
	 void Death();
private:
	/// <summary>
	/// スプライト
	/// </summary>
	Sprite m_title;
	/// <summary>
	/// ゲームのインスタンス
	/// </summary>
	Game* game;
};

