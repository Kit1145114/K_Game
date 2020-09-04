#pragma once
class GameObject
{
public:
	//基底クラスには仮想デストラクタが必要
	virtual ~GameObject(){};
	/// 更新関数。
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// レンダ関数
	/// </summary>
	virtual void Render();

	virtual void Draw() {}

	virtual void FontRender() {}

	virtual bool Start();

	void GoUpdate()
	{
		if (isUpdate&&isActive&&isStart) {
			Update();
		}
	}

	void GoDraw()
	{
		if (isUpdate && isActive && isStart) {
			Draw();
		}
	}
	//ポストレンダー
	//virtual void PostRender();

	void PostRenderWarapper()
	{
		if (isUpdate&isActive&&isStart) {
			Render();
		}
	}
	void FontRenderWarapper()
	{
		if (isUpdate&isActive&&isStart) {
			FontRender();
		}
	}
	void RecestDelete()
	{
		isRecestDelete = true;
	}
	void SetIsNewGO()
	{
		isNewGO = true;
	}
	bool IsRecestDelete()
	{
		return isRecestDelete;
	}
	bool GetIsGO()
	{
		return isNewGO;
	}
	//アップデートするかどうか
	bool GetisUpdate() const
	{
		return isUpdate;
	}
	//アップデートフラグを設定する
	void SetUpdateFlag(bool flag)
	{
		isUpdate = flag;
	}	
	void SetActive(bool flag)
	{
		isActive = flag;
	}
	//スタート関数のフラグを変える。
	void SetIsStart(bool flag)
	{
		isStart = flag;
	}
	bool GetIsStart()
	{
		return isStart;
	}
private:
	bool isRecestDelete = false;	//フラグ
	bool isNewGO = false;
	bool isUpdate = true;
	bool isActive = true;
	bool isStart = false;
};

