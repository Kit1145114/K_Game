#include "stdafx.h"
#include "GameObjectManager.h"

//GameObjectManajerクラスのインスタンス。
GameObjectManager g_goMgr;

bool GameObjectManager::Start()
{
	for (auto go : m_goList)
	{
		if (!go->GetIsStart())
		{
			go->SetIsStart(true);
			go->Start();
		}
	}
	return true;
}

void GameObjectManager::Update()
{
	Start();
	for (auto go : m_goList) {
		go->GoUpdate();
	}
	
}

void GameObjectManager::Delete()
{
	for (auto it = m_goList.begin(); it != m_goList.end();)
	{
		if ((*it)->IsRecestDelete())
		{
			delete* it;
			it = m_goList.erase(it);
		}
		else {
			//リクエストを受けてないので
			it++;
		}
	}
}

void GameObjectManager::Draw()
{
	for (auto go : m_goList) {
		go->GoDraw();
	}
}

//GameObjectManajer* GameObjectManajer::GetInstance()
//{
//	//どこからでもゲームが呼び出せるように
//	//インスタンスをreturnで返す。
//	return m_instance;
//}

void GameObjectManager::BackUp()
{
	//フレームバッファののレンダリングターゲットをバックアップしておく。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);
}

void GameObjectManager::PreRender()
{

}

void GameObjectManager::ForwordRender()
{
	////レンダリングターゲットをメインに変更する。
	//auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//ChangeRenderTarget(d3dDeviceContext, &m_mainRenderTarget, &m_frameBufferViewports);
	////メインレンダリングターゲットをクリアする。
	//float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//m_mainRenderTarget.ClearRenderTarget(clearColor);
}

void GameObjectManager::PostRender()
{
	for (auto go : m_goList) {
		go->PostRenderWarapper();
	}
}

void GameObjectManager::FontRender()
{
	for (auto go : m_goList) {
		go->FontRenderWarapper();
	}
}

void GameObjectManager::ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, RenderTarget * renderTarget, D3D11_VIEWPORT * viewport)
{
	ChangeRenderTarget(
		d3dDeviceContext,
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}

void GameObjectManager::ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, ID3D11RenderTargetView * renderTarget, ID3D11DepthStencilView * depthStensil, D3D11_VIEWPORT * viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//レンダリングターゲットの切り替え。
	d3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//ビューポートが指定されていたら、ビューポートも変更する。
		d3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GameObjectManager::InitCamera()
{
}

void GameObjectManager::Render()
{
	//プリレンダリング
	PreRender();

	//フォワードレンダリング。
	ForwordRender();

	//ポストレンダリング
	PostRender();
}