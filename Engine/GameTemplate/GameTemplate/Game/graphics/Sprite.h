#pragma once
#include "graphics/shader.h"

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
	void UpdateWorldMatrix(CVector3 pos, CQuaternion rot, CVector3 scale);
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
public:
	void Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot);
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
};

