#pragma once
#include<vector>
#include"GameObject.h"

class GameObjectManajer 
{
public:
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// ゲームオブジクトを追加
	/// </summary>
	template<class T>
	T* NewAGO()
	{
		T* newObj = new T();
		m_goList.push_back(newObj);
		newObj->SetIsNewGO();
		return newObj;
	}
	/// <summary>
	/// ゲームオブジェクトからリストを削除する。
	/// </summary>
	/// <param name="go"></param>削除するゲームオブジェクト
	void QutavaleyaAGO(GameObject* go) {
			if (go->GetIsGO() == true) {
				//見つかった。
				//リストから削除する。
				//m_goList.erase();
				////インスタンス自体も削除。
				//delete go;/
				go->RecestDelete();
				//削除できたので終わり。
				return;
		}
	}
	void FindGO(GameObject*go) {
		if (go->GetIsGO() == true)
		{
			return ;
		}
	}
private:
	//可変長配列
	std::vector<GameObject*>m_goList;			//ゲームオブジェクトのリスト
	std::vector<GameObject*>m_delete_goList;	//デリートします。
};

extern GameObjectManajer g_goMgr;
