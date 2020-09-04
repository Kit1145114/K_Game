#pragma once
#include<vector>
#include "GameObject.h"
#include "graphics/RenderTarget.h"
#include "graphics/Sprite.h"


class GameObjectManajer 
{
public:
	bool Start();
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// ゲームオブジクトを追加
	/// </summary>
	template<class T>
	T* NewGO()
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
	void DeleteGO(GameObject* go) {
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
	/// <summary>
	/// 3Dモデルを描画
	/// </summary>
	void Draw();
	/// <summary>
	/// ゲームオブジェクトをデリート
	/// </summary>
	void Delete();
	//レンダー類
	/// </summary>
	void Render();
	/// <summary>
//	static GameObjectManajer* GetInstance();		//シングルトン。

	void BackUp();
	/// <summary>
	/// プリレンダリング。
	/// </summary>
	void PreRender();
	/// <summary>
	/// フォワードレンダリング(通常の描画だと考えてOK)
	/// </summary>
	void ForwordRender();
	/// <summary>
	/// ポストレンダリング
	/// </summary>
	void PostRender();
	/// <summary>
	/// フォントレンダリング
	/// </summary>
	void FontRender();
	/// <summary>
	/// レンダリングターゲットの切り替え。
	/// </summary>
	/// <param name="d3dDeviceContext">D3Dデバイスコンテキスト</param>
	/// <param name="renderTarget">レンダリングターゲット</param>
	/// <param name="viewport">ビューポート</param>
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	/// <summary>
	/// カメラを初期化。
	/// </summary>
	void InitCamera();
private:
	//レンダー
	RenderTarget m_renderTarget;
	static GameObjectManajer* m_instance;		//ゲームのインスタンス。//レンダリングターゲット。
	//可変長配列
	std::list<GameObject*>m_goList;			//ゲームオブジェクトのリスト
	std::list<GameObject*>m_delete_goList;	//デリートします。
	//レンダーターゲット
	Sprite m_copyMainRtToFrameBufferSprite;			//メインレンダリングターゲットに描かれた絵をフレームバッファにコピーするためのスプライト。
	D3D11_VIEWPORT m_frameBufferViewports;			//フレームバッファのビューポート。
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー。
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー。
};

extern GameObjectManajer g_goMgr;
