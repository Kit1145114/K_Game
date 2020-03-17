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
	//スタート関数。
	virtual bool Start() = 0;

	void AgoUpdate()
	{
		if (isUpdate&isActive) {
			Update();
		}
	}
	//ポストレンダー
	//virtual void PostRender();

	void PostRenderWarapper()
	{
		if (isUpdate&isActive) {
			Render();
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

