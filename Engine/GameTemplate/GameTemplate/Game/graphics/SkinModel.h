#pragma once

#include "Skeleton.h"
#include "system/system.h"
#include "graphics/Shader.h"
#include"animation/Animation.h"
#include"animation/AnimationClip.h"
#include"Game/GameObjectManager.h"
#include"c3dmodel/C3DModelEffect.h"
#include "shadow\CascadeShadowMap.h"


/*!
*@brief	FBXの上方向。
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};

const int NUM_DIRECTION_LIG = 4;	//ディレクションライト(値変更不可)

struct SLight
{
	CVector4	direction[4];	//ライトの方向。
	CVector4	color[4];		//ライトのカラー。
	CVector3	eyePos;			//視点の座標。
	float		specPow;		//鏡面反射の絞り
	CVector3	ambientLight;	//アンビエントライト。
	int			isSpec = 0;		//スペキュラライトを当て
	int			isNormal = 0;
	int			isEmission = 0;
};

struct TextureData {
	const wchar_t* normalFilePath = nullptr;
	const wchar_t* specFilePath = nullptr;
	const wchar_t* emissionFilePath = nullptr;
};

/*!
*@brief	スキンモデルクラス。
*/
class SkinModel
{ 
public:
	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();
	
	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);

	/// <summary>
	/// 色んなテクスチャーの初期化
	/// </summary>
	/// <param name="textureData"></param>
	void InitTexture(TextureData* textureData);
	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/// <summary>
	/// ３Dモデルのワールド空間での回転処理
	/// </summary>
	void SetRotation(const CQuaternion & rotation)
	{
		m_rotation = rotation;
	}

	/*!
	*@brief	モデルを描画。
	*@param[in]	viewMatrix		カメラ行列。
	*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
	*@param[in]	projMatrix		プロジェクション行列。
	*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
	*/
	void Draw( CMatrix viewMatrix, CMatrix projMatrix,int Spec, EnRenderMode enRenderMode = enRenderMode_Normal);
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	void QueryMaterials(std::function<void(C3DModelEffect*)> func)
	{
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			func(reinterpret_cast<C3DModelEffect*>(material));
		});
	}
	/*!
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
		enSkinModelSRVReg_NormalTexture = 7,
		enSkinModelSRVReg_SpecTexture = 8,
		enSkinModelSRVReg_EmissionTexture = 9,
	};
	//アニメーションイベント
	void AddAnimationEventListener(std::function<void(const wchar_t* clipName, const wchar_t* eventName)> eventListener)
	{
		m_animation.AddAnimationEventListener(eventListener);
	}
	/// <summary>
	/// シャドウレシーバーを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetShadowReciver(bool flag)
	{
		m_isShadowReciver = flag;
	}
	/// <summary>
	/// シャドウキャスターを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetShadowCaster(bool flag)
	{
		m_isShadowCaster = flag;
	}
private:
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);
	//ディレクションライトの初期化。
	void InitDirectionLight();
	void SetTexture();
private:
	//定数バッファ。
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//todo ライトビュー行列。
		CMatrix mLightProj;		//todo ライトプロジェクション行列。
		CMatrix mLightViewProj[CascadeShadowMap::SHADOWMAP_NUM];	//ライトビュープロジェクション行列
		CMatrix mLightViewInv[CascadeShadowMap::SHADOWMAP_NUM];		//ライトビューの逆行列
	    CVector4 mFar[CascadeShadowMap::SHADOWMAP_NUM];
		int isShadowReciever;	//todo シャドウレシーバーのフラグ。
		int shadowMapNumber = 0;	//何番目のシャドウマップにレンダリングするか
	};
	Animation			m_animation;					//アニメーション。
	AnimationClip		m_animclip;					//アニメーションクリップ。
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBXの上方向。
	ID3D11Buffer*		m_cb = nullptr;					//!<定数バッファ。
	Skeleton			m_skeleton;						//!<スケルトン。
	CMatrix				m_worldMatrix;					//!<ワールド行列。
	DirectX::Model*		m_modelDx;						//!<DirectXTKが提供するモデルクラス。
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11ShaderResourceView* m_albedoTextureSRV = nullptr;	//!<アルベドテクスチャのSRV//!<サンプラステート。
	ID3D11Buffer*		m_lightCb = nullptr;				//!<ライト用の定数バッファ。
	//DirectionLight		m_dirLight;							//!<ディレクションライト。
	SLight				m_sLight;							//!<ライトの構造体。
	CQuaternion			m_rotation = CQuaternion::Identity();//回転
	bool m_isShadowReciver = true;		//シャドウレシーバー
	bool m_isShadowCaster = true;		//シャドウキャスター
	ID3D11ShaderResourceView* m_normalMap = nullptr;
	ID3D11ShaderResourceView* m_specMap = nullptr;
	ID3D11ShaderResourceView* m_emissionMap = nullptr;
};

