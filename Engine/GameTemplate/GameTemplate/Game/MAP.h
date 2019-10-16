#pragma once
#include"GameObjectManajer.h"
class MAP	:	public GameObject
{
public:
	/// <summary>
	/// インストラクタ
	/// </summary>
	MAP();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MAP();
	void Update();
	void Draw();
private:
	SkinModel Map;	//マップのスキンモデル。
};

