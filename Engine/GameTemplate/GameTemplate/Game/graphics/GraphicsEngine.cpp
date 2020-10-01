#include "stdafx.h"
#include "GraphicsEngine.h"
#include "RenderTarget.h"
#include "shadow/ShadowMap.h"
#include "shadow\CascadeShadowMap.h"
#include "Sprite.h"
#include "graphics/postEffect/PostEffect.h"

GraphicsEngine::GraphicsEngine()
{
}


GraphicsEngine::~GraphicsEngine()
{
	Release();
}

void GraphicsEngine::BegineRender()
{
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
													  //描き込み先をバックバッファにする。
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	//バックバッファを灰色で塗りつぶす。
	m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void GraphicsEngine::EndRender()
{
	//バックバッファとフロントバッファを入れ替える。
	m_pSwapChain->Present(2, 0);

	//m_backBuffer->Release();
	//m_depthStencilView->Release();
}
void GraphicsEngine::Release()
{
	if (m_rasterizerState != NULL) {
		m_rasterizerState->Release();
		m_rasterizerState = NULL;
	}
	if (m_depthStencil != NULL) {
		m_depthStencil->Release();
		m_depthStencil = NULL;
	}
	if (m_depthStencilView != NULL) {
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}
	if (m_backBuffer != NULL) {
		m_backBuffer->Release();
		m_backBuffer = NULL;
	}
	if (m_pSwapChain != NULL) {
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	if (m_pd3dDeviceContext != NULL) {
		m_pd3dDeviceContext->Release();
		m_pd3dDeviceContext = NULL;
	}
	if (m_pd3dDevice != NULL) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
}
void GraphicsEngine::Init(HWND hWnd)
{
	//スワップチェインを作成するための情報を設定する。
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;									//スワップチェインのバッファ数。通常は１。
	sd.BufferDesc.Width = (UINT)FRAME_BUFFER_W;			//フレームバッファの幅。
	sd.BufferDesc.Height = (UINT)FRAME_BUFFER_H;		//フレームバッファの高さ。
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//フレームバッファのフォーマット。R8G8B8A8の32bit。
	sd.BufferDesc.RefreshRate.Numerator = 60;			//モニタのリフレッシュレート。(バックバッファとフロントバッファを入れ替えるタイミングとなる。)
	sd.BufferDesc.RefreshRate.Denominator = 1;			//２にしたら30fpsになる。1でいい。
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//サーフェスまたはリソースを出力レンダー ターゲットとして使用します。
	sd.OutputWindow = hWnd;								//出力先のウィンドウハンドル。
	sd.SampleDesc.Count = 1;							//1でいい。
	sd.SampleDesc.Quality = 0;							//MSAAなし。0でいい。
	sd.Windowed = TRUE;									//ウィンドウモード。TRUEでよい。

														//利用するDirectXの機能セット。DirectX10以上に対応しているGPUを利用可能とする。
														//この配列はD3D11CreateDeviceAndSwapChainの引数として使う。
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	//D3Dデバイスとスワップチェインを作成する。
	D3D11CreateDeviceAndSwapChain(
		NULL,											//NULLでいい。
		D3D_DRIVER_TYPE_HARDWARE,						//D3Dデバイスがアクセスするドライバーの種類。
														//基本的にD3D_DRIVER_TYPE_HARDWAREを指定すればよい。
		NULL,											//NULLでいい。
		0,												//０でいい。
		featureLevels,									//D3Dデバイスのターゲットとなる機能セットを指定する。
														//今回のサンプルはDirectX10以上をサポートするので、
														//それらを含むD3D_FEATURE_LEVELの配列を渡す。
		sizeof(featureLevels) / sizeof(featureLevels[0]),	//機能セットの数。
		D3D11_SDK_VERSION,								//使用するDirectXのバージョン。
														//D3D11_SDK_VERSIONを指定すればよい。
		&sd,											//スワップチェインを作成するための情報。
		&m_pSwapChain,									//作成したスワップチェインのアドレスの格納先。
		&m_pd3dDevice,									//作成したD3Dデバイスのアドレスの格納先。
		&m_featureLevel,								//使用される機能セットの格納先。
		&m_pd3dDeviceContext							//作成したD3Dデバイスコンテキストのアドレスの格納先。
	);

	//書き込み先になるレンダリングターゲットを作成。
	ID3D11Texture2D* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();
	//深度ステンシルビューの作成。
	{
		//深度テクスチャの作成。
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = (UINT)FRAME_BUFFER_W;
		texDesc.Height = (UINT)FRAME_BUFFER_H;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &m_depthStencil);
		//深度ステンシルビューを作成。
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = texDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		m_pd3dDevice->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
	}
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//ラスタライザとビューポートを初期化。
	m_pd3dDevice->CreateRasterizerState(&desc, &m_rasterizerState);

	m_viewPort.Width = FRAME_BUFFER_W;
	m_viewPort.Height = FRAME_BUFFER_H;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_pd3dDeviceContext->RSSetViewports(1, &m_viewPort);
	m_pd3dDeviceContext->RSSetState(m_rasterizerState);
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pd3dDeviceContext);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_pd3dDevice, L"Assets/font/myfile.spritefont");

	m_shadowMap = new ShadowMap();
	m_cascadeShadowMap = new CascadeShadowMap();

	m_mainRenderTarget = new RenderTarget();
	//メインとなるレンダリングターゲットを作成する。
	m_mainRenderTarget->Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	m_copyMainRtToFrameBufferSprite = new Sprite();
	m_copyMainRtToFrameBufferSprite->Init(m_mainRenderTarget->GetRenderTargetSRV());

	m_postEffect = new PostEffect();
	m_postEffect->InitFullScreenQuadPrimitive();

}
void GraphicsEngine::ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport)
{
	ChangeRenderTarget(
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}
void GraphicsEngine::ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//レンダリングターゲットの切り替え。
	m_pd3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//ビューポートが指定されていたら、ビューポートも変更する。
		m_pd3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GraphicsEngine::ChangeMainRenderTarget()
{
	//ChangeRenderTarget(&m_mainRenderTarget, &m_frameBufferViewports);

	ID3D11RenderTargetView* rt[] = {
		m_mainRenderTarget->GetRenderTargetView()
	};
	m_pd3dDeviceContext->OMSetRenderTargets(1, rt, m_mainRenderTarget->GetDepthStensilView());
	//m_pd3dDeviceContext->RSSetViewports(1, m_mainRenderTarget.GetViewport());
	m_pd3dDeviceContext->RSSetViewports(1, &m_viewPort);
	//メインレンダリングターゲットをクリアする。
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_mainRenderTarget->ClearRenderTarget(clearColor);
}

void GraphicsEngine::RenderToShadowMap()
{
	//m_shadowMap->RenderToShadowMap();
	m_cascadeShadowMap->Update();
	m_cascadeShadowMap->RenderToShadowMap();


	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
												  //描き込み先をバックバッファにする。
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	m_pd3dDeviceContext->RSSetViewports(1, &m_viewPort);
	//バックバッファを灰色で塗りつぶす。
	m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GraphicsEngine::PostRender()
{
	//ポストエフェクトの描画。
	m_postEffect->Draw();

	//レンダリングターゲットをフレームバッファに戻す。
	ChangeRenderTarget(
		m_backBuffer,
		m_depthStencilView,
		&m_viewPort
	);

	//m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	CMatrix mView;
	CMatrix mProj;
	mView.MakeLookAt(
		{ 0, 0, 1 },
		{ 0, 0, 0 },
		{ 0,1,0 }
	);
	mProj.MakeOrthoProjectionMatrix(1280.0f, 720.0f, 0.10f, 100.0f);
	m_copyMainRtToFrameBufferSprite->Update(CVector3(0.0f, 0.0f, 0.0f), { 0.0f,1.0f,0.0f,0.0f }, CVector3(1.0f,1.0f,1.0f), CVector2(0.5f,0.5f));
	m_copyMainRtToFrameBufferSprite->Draw(mView, mProj, m_alpha);
}
