#pragma once
#include "graphics/shader.h"
#include"GraphicsEngine.h"

class Sprite
{
public:
	static const CVector2	DEFAULT_PIVOT;
	/// <summary>
	/// インストラクタ
	/// </summary>
	Sprite();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="textureFilePath">テクスチャのファイルパス</param>
	void Init(const wchar_t* textureFilePath, float w, float h);
	/// <summary>
	/// ワールド行列の更新
	/// </summary>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	//void UpdateWorldMatrix(CVector3 pos, CQuaternion rot, CVector3 scale);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="mView">カメラ行列</param>
	/// <param name="mProj">プロジェクション行列</param>
	void Draw(CMatrix mView, CMatrix mProj);
	void SetActiveFlag(bool flag)
	{
		Active = flag;
	}

	//αブレンドステート。
	struct AlphaBlendState {
		ID3D11BlendState*	disable;	//!<アルファブレンディングが無効。
		ID3D11BlendState*	trans;		//!<半透明合成。
		ID3D11BlendState*	add;		//!<加算合成。
		void Init(GraphicsEngine& ge);
	};

	//深度ステンシルステート。
	struct DepthStencilState
	{
		ID3D11DepthStencilState* disable;			//!<すべて無効。
		ID3D11DepthStencilState* SceneRender;		//!<3Dモデルを描画する時の基本的な深度ステンシルステート。
															//!<深度テスト、深度書き込みともに有効なステートです。
		ID3D11DepthStencilState* spriteRender;		//!<2D描画する時の基本的な深度ステンシルステート。
															//!<深度テスト、深度書き込みともに無効なステートです。
		ID3D11DepthStencilState* defferedRender;		//!<ディファードレンダリングを行うときの深度ステンシルステート。
		void Init(GraphicsEngine& ge);												//!<深度テスト無効、深度書き込み有効なステートです。
	};
	//ラスタライザステート。
	struct RasterizerState
	{
		ID3D11RasterizerState*	sceneRender;		//!<3Dモデルを描画する時の基本的なラスタライザステート。
		ID3D11RasterizerState*	spriteRender;		//!<2D描画する時の基本的なラスタライザステート。
		void Init(GraphicsEngine& ge);
	};
	void InitDepthStencil();
public:
	void Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot, float a = 0.5f);
private:
	/// <summary>
	/// シェーダーをロード。
	/// </summary>
	void LoadShader();
	/// <summary>
	/// 頂点バッファを作成。
	/// </summary>
	void CreateVertexBuffer(float w, float h);
	/// <summary>
	/// インデックスバッファを作成。
	/// </summary>
	void CreateIndexBuffer();
	/// <summary>
	/// サンプラステートを作成。
	/// </summary>
	void CreateSamplerState();
	/// <summary>
	/// 定数バッファの作成。
	/// </summary>
	void CreateConstantBuffer();
	/// <summary>
	/// <param name="textureFilePath">ロードするテクスチャのファイルパス</param>
	void LoadTexture(const wchar_t* textureFilePath);


private:
	bool Active = true;
	Shader m_vs;
	Shader m_ps;
	ID3D11Buffer*	m_vertexBuffer = nullptr;
	ID3D11Buffer*	m_indexBuffer = nullptr;
	ID3D11Buffer*	m_cbGPU = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;
	CMatrix m_world = CMatrix::Identity();
	CVector2				m_size = CVector2::Zero();		//!<サイズ。
	float m_alpha = 1.0f;									//α値
	ID3D11RasterizerState* m_rasterizer = nullptr;
	ID3D11BlendState* m_alphablend = nullptr;
	ID3D11DepthStencilState* m_depthstencil = nullptr;

	ID3D11Device* ps2d = nullptr;
	


};

