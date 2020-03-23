#include "stdafx.h"
#include "GameObjectManajer.h"

//GameObjectManajerクラスのインスタンス。
GameObjectManajer g_goMgr;

bool GameObjectManajer::Start()
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

void GameObjectManajer::Update()
{
	Start();
	for (auto go : m_goList) {
		go->AgoUpdate();
	}
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

//GameObjectManajer* GameObjectManajer::GetInstance()
//{
//	//どこからでもゲームが呼び出せるように
//	//インスタンスをreturnで返す。
//	return m_instance;
//}

void GameObjectManajer::BackUp()
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

void GameObjectManajer::PreRender()
{

}

void GameObjectManajer::ForwordRender()
{
	////レンダリングターゲットをメインに変更する。
	//auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//ChangeRenderTarget(d3dDeviceContext, &m_mainRenderTarget, &m_frameBufferViewports);
	////メインレンダリングターゲットをクリアする。
	//float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//m_mainRenderTarget.ClearRenderTarget(clearColor);
}

void GameObjectManajer::PostRender()
{
	for (auto go : m_goList) {
		go->PostRenderWarapper();
	}
}

void GameObjectManajer::FontRender()
{
	for (auto go : m_goList) {
		go->FontRenderWarapper();
	}
}

void GameObjectManajer::ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, RenderTarget * renderTarget, D3D11_VIEWPORT * viewport)
{
	ChangeRenderTarget(
		d3dDeviceContext,
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}

void GameObjectManajer::ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, ID3D11RenderTargetView * renderTarget, ID3D11DepthStencilView * depthStensil, D3D11_VIEWPORT * viewport)
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

void GameObjectManajer::InitCamera()
{
	//g_camera3D.SetPosition({ 0.0f, 1000.0f, 2200.0f });
	//g_camera3D.SetTarget({ 0.0f, 200.0f, 0.0f });
	//g_camera3D.Update();
	//g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	//g_camera2D.SetWidth(FRAME_BUFFER_W);
	//g_camera2D.SetHeight(FRAME_BUFFER_H);
	//g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	//g_camera2D.SetTarget(CVector3::Zero());
	//g_camera2D.Update();
}

void GameObjectManajer::Render()
{
	//プリレンダリング
	PreRender();

	//フォワードレンダリング。
	ForwordRender();

	//ポストレンダリング
	PostRender();
}