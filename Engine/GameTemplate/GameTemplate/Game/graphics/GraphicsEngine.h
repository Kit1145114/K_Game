#pragma once
/*!
 *@brief	グラフィックスエンジン。
 */

 /// <summary>
 /// レンダリングモード。
 /// </summary>
enum EnRenderMode {
	enRenderMode_Invalid,			//不正なレンダリングモード。
	enRenderMode_CreateCascadeShadowMap,    //カスケードシャドウマップ生成
	enRenderMode_CreateShadowMap,	//シャドウマップ生成。
	enRenderMode_Normal,			//通常レンダリング。
	enRenderMode_Num,				//レンダリングモードの数。
};

class RenderTarget;
class ShadowMap;
class CascadeShadowMap;
class Sprite;
class PostEffect;
class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	初期化。
	 *@param[in]	hWnd		ウィンドウハンドル。
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	解放。
	 */
	void Release();
	/*!
	 *@brief	D3D11デバイスを取得。
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11デバイスコンテキストを取得。
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/*!
	 *@brief	描画開始。
	 */
	void BegineRender();
	/*!
	 *@brief	描画終了。
	 */
	void EndRender();
	DirectX::SpriteBatch* GetSpriteBatch()
	{
		return m_spriteBatch.get();
	}
	DirectX::SpriteFont* GetSpriteFont()
	{
		return m_spriteFont.get();
	}
	/// <summary>
	/// シャドウマップを取得
	/// </summary>
	/// <returns></returns>
	ShadowMap* GetShadowMap()
	{
		return m_shadowMap;
	}
	/// <summary>
	/// カスケードシャドウマップを取得
	/// </summary>
	/// <returns></returns>
	CascadeShadowMap* GetCascadeShadowMap()
	{
		return m_cascadeShadowMap;
	}
	void ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	/// <summary>
	/// シャドウマップを生成
	/// </summary>
	void RenderToShadowMap();
	/// <summary>
	/// レンダリングターゲットをメインに設定
	/// </summary>
	void ChangeMainRenderTarget();
	/// <summary>
	/// ポストエフェクト
	/// </summary>
	void PostRender();
	/// <summary>
	/// メインレンダリングターゲットを取得。
	/// </summary>
	/// <returns></returns>
	RenderTarget* GetMainRenderTarget()
	{
		return m_mainRenderTarget;
	}
private:
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11デバイス。
	IDXGISwapChain*			m_pSwapChain = NULL;		//スワップチェイン。
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//ラスタライザステート。
	ID3D11Texture2D*		m_depthStencil = NULL;		//デプスステンシル。
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。

	//定義。
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;     //これが肝
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;		//これも肝

	ShadowMap* m_shadowMap = nullptr;
	CascadeShadowMap* m_cascadeShadowMap = nullptr;

	CD3D11_VIEWPORT m_viewPort;
	RenderTarget*			m_mainRenderTarget;			//!<メインレンダリングターゲット。
	Sprite* m_copyMainRtToFrameBufferSprite;
	PostEffect* m_postEffect;

	float m_alpha = 1.0f;
};

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン