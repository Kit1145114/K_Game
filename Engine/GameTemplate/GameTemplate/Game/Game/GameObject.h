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
		if (isUpdate_flag&&isActive_flag&&isStart_flag) {
			Update();
		}
	}

	void GoDraw()
	{
		if (isUpdate_flag && isActive_flag && isStart_flag) {
			Draw();
		}
	}
	//ポストレンダー
	//virtual void PostRender();

	void PostRenderWarapper()
	{
		if (isUpdate_flag&isActive_flag&&isStart_flag) {
			Render();
		}
	}
	void FontRenderWarapper()
	{
		if (isUpdate_flag&isActive_flag&&isStart_flag) {
			FontRender();
		}
	}
	void RecestDelete()
	{
		isRecestDelete_flag = true;
	}
	void SetIsNewGO()
	{
		isNewGO_flag = true;
	}
	bool IsRecestDelete()
	{
		return isRecestDelete_flag;
	}
	bool GetIsGO()
	{
		return isNewGO_flag;
	}
	//アップデートするかどうか
	bool GetisUpdate() const
	{
		return isUpdate_flag;
	}
	//アップデートフラグを設定する
	void SetUpdateFlag(bool flag)
	{
		isUpdate_flag = flag;
	}	
	void SetActive(bool flag)
	{
		isActive_flag = flag;
	}
	//スタート関数のフラグを変える。
	void SetIsStart(bool flag)
	{
		isStart_flag = flag;
	}
	bool GetIsStart()
	{
		return isStart_flag;
	}
private:
	bool isRecestDelete_flag = false;	//フラグ
	bool isNewGO_flag = false;
	bool isUpdate_flag = true;
	bool isActive_flag = true;
	bool isStart_flag = false;
};

