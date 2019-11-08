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

	void AgoUpdate()
	{
		if (isUpdate) {
			Update();
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
private:
	bool isRecestDelete = false;	//フラグ
	bool isNewGO = false;
	bool isUpdate = true;
};

