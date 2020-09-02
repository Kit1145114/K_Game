/*!
 * @brief	モデルシェーダー。
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
/// 
static const int NUM_CASCADES = 3;
//アルベドテクスチャ。
Texture2D<float4> g_albedoTexture : register(t0);	
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

Texture2D<float4> shadowMap : register(t2);		//todo シャドウマップ。
Texture2D<float4> g_cascadeShadowMap1 : register(t3);		//todo シャドウマップ。
Texture2D<float4> g_cascadeShadowMap2 : register(t4);		//todo シャドウマップ。
Texture2D<float4> g_cascadeShadowMap3 : register(t5);		//todo シャドウマップ。
Texture2D<float4> g_cascadeShadowMap4 : register(t6);		//todo シャドウマップ。

Texture2D<float4> g_normalMap : register(t7);
Texture2D<float4> g_specMap : register(t8);
Texture2D<float4> g_emissionMap : register(t9);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler g_sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//todo ライトビュー行列を追加。
	float4x4 mLightView;	//ライトビュー行列。
	float4x4 mLightProj;	//ライトプロジェクション行列。
	float4x4 mLightViewProj[NUM_CASCADES];
	float4x4 mLightViewInv[NUM_CASCADES];		//ライトビューの逆行列
	float4 mFar[NUM_CASCADES];
	int isShadowReciever;	//シャドウレシーバーフラグ。
	int shadowMapNumber = 0;
};
static const int NUM_LIGHTS = 4;
/// <summary>
/// ライト用の定数バッファ
/// </summary>
cbuffer LightCb : register(b1) {
	float4		dligDirection[NUM_LIGHTS];	//ライトの方向。
	float4		dligColor[NUM_LIGHTS];		//ライトのカラー。
	float3	    eyePos;			//視点の座標。
	float		specPow;		//鏡面反射の絞り
	float3		ambientLight;	//アンビエントライト。
	int			isSpec;		//スペキュラライトを当て
	int			isNormal;
	int			isEmission;
};
/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;
	float4 posInLVP[NUM_CASCADES]	: TEXCOORD2;	//ライトビュープロジェクション空間での座標
	float4 posInCamera : TEXCOORD6;					//カメラ空間での座標。
	float4 posInLVP2   : TEXCOORD7;
};

/// <summary>
/// シャドウマップ用のピクセルシェーダへの入力構造体。
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};

/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	psInput.worldPos = pos;
	if (isShadowReciever == 1) {
		float4 p = pos;
		p = mul(mLightView, p);
		psInput.posInLVP2 = mul(mLightProj, p);
		//ライトビュープロジェクション座標に変換
		for (int i = 0; i < NUM_CASCADES; i++) {
			psInput.posInLVP[i] = mul(mLightViewProj[i], pos);
		}
	}
	pos = mul(mView, pos);
	psInput.posInCamera = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));

	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
		psInput.worldPos = pos;
	}
	if (isShadowReciever == 1) {
		float4 p = pos;
		p = mul(mLightView, p);
		psInput.posInLVP2 = mul(mLightProj, p);
		//ライトビュープロジェクション座標に変換
		for (int i = 0; i < NUM_CASCADES; i++) {
			psInput.posInLVP[i] = mul(mLightViewProj[i], pos);
		}
	}
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	pos = mul(mView, pos);
	psInput.posInCamera = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	//albedoテクスチャからカラーをフェッチする。
	float4 albedoColor = g_albedoTexture.Sample(g_sampler, In.TexCoord);
	//ディレクションライトの拡散反射光を計算する。
	float3 lig = 0.0f;
	lig += float3(0.7f, 0.7f, 0.7f);

	


	float3 normal = 0;
	if (isNormal == 1) {
		//法線マップがある。
		//法線と接ベクトルの外積を計算して、従ベクトルを計算する。
		float3 biNormal = cross(In.Normal, In.Tangent);
		normal = g_normalMap.Sample(g_sampler, In.TexCoord);
		//0.0～1.0の範囲になっているタンジェントスペース法線を
		//-1.0～1.0の範囲に変換する。
		normal = (normal * 2.0f) - 1.0f;
		//法線をタンジェントスペースから、ワールドスペースに変換する。
		normal = In.Tangent * normal.x + biNormal * normal.y + In.Normal * normal.z;
	}
	else {
		normal = In.Normal;
	}

	for (int i = 0; i < NUM_LIGHTS; i++) {
		//実習　鏡面反射を計算しなさい。
				//①　反射ベクトルRを求める。
		float3 R = dligDirection[i].xyz
			+ 2 * dot(In.Normal, -dligDirection[i].xyz)
			* In.Normal;
		//②　視点からライトを当てる物体に伸びるベクトルEを求める。
		float3 E = normalize(In.worldPos - eyePos);
		//①と②で求まったベクトルの内積を計算する。
		//スペキュラ反射の強さを求める。
		float specPower = max(0, dot(R, -E));
		float spec;
		if (isSpec == 1) {
			//スペキュラマップがある。
			spec = g_specMap.Sample(g_sampler, In.TexCoord).r;

			float3 specLig = pow(specPower, 2.0f) * dligColor[i].xyz * spec * 10.0f;
			//⑤ スペキュラ反射が求まったら、ligに加算する。
			//鏡面反射を反射光に加算する。
			lig += specLig;
		}
		else {
			specPower = pow(specPower, specPow);
			//lig += dligColor[i].xyz * specPower;
		}

		//拡散反射
		lig += max(0.0f, dot(In.Normal * -1.0f, dligDirection[i].xyz)) * dligColor[i].xyz * 1.2f;
	}
	//エミッション
	if (isEmission == 1) {
		float emission = g_emissionMap.Sample(g_sampler, In.TexCoord);
		//lig += float3(2.0f, 2.0f, 2.0f) * emission;
	}
	
	/*for (int i = 0; i < 4; i++) {
		//lig += max(0.0f, dot(In.Normal * -1.0f, dligDirection[i])) * dligColor[i];
		//鏡面反射の光の量を計算する。
		if (hasSpec == 1) {
			float3 rv = reflect(dligDirection[i], In.Normal);
			float3 toEye = normalize(eyePos - In.worldPos);
			float t = max(0.0f, dot(toEye, rv));
			t = pow(t, 4.0f);
			//lig += t * dligColor[i] * specPow;
		}
	}*/

	if (isShadowReciever == 1) {
		/*//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
			//プロジェクション行列をシャドウマップのUV座標に変換している
		float2 shadowMapUV = In.posInLVP2.xy / In.posInLVP2.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//spsOut.shadow = shadowMapUV.x;
		//シャドウマップのUV座標範囲内かどうかを判定する。
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {
			///LVP空間での深度値を計算。
			float zInLVP = In.posInLVP2.z / In.posInLVP2.w;
			float zInShadowMap = 1.0f;
			//シャドウマップに書き込まれている深度値を取得。

			zInShadowMap = shadowMap.Sample(g_sampler, shadowMapUV);
			if (zInLVP > zInShadowMap + 0.0001f) {
				//影が落ちているので、光を弱くする
				lig *= 0.5f;
			}
		}*/
		for (int i = 0; i < NUM_CASCADES; i++) {
			if (mFar[i].x  > In.posInCamera.z) {
				//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
				//プロジェクション行列をシャドウマップのUV座標に変換している
				float2 shadowMapUV = In.posInLVP[i].xy / In.posInLVP[i].w;
				shadowMapUV *= float2(0.5f, -0.5f);
				shadowMapUV += 0.5f;
				//spsOut.shadow = shadowMapUV.x;
				//シャドウマップのUV座標範囲内かどうかを判定する。
				if (shadowMapUV.x <= 1.0f
					&& shadowMapUV.x >= 0.0f
					&& shadowMapUV.y <= 1.0f
					&& shadowMapUV.y >= 0.0f
					) {
					///LVP空間での深度値を計算。
					float zInLVP = In.posInLVP[i].z / In.posInLVP[i].w;
					float zInShadowMap = 1.0f;
					//シャドウマップに書き込まれている深度値を取得。
					if (i == 0) {
						zInShadowMap = g_cascadeShadowMap1.Sample(g_sampler, shadowMapUV);
					}
					else if (i == 1) {
						zInShadowMap = g_cascadeShadowMap2.Sample(g_sampler, shadowMapUV);
					}
					else if (i == 2) {
						zInShadowMap = g_cascadeShadowMap3.Sample(g_sampler, shadowMapUV);
					}
					else if (i == 3) {
						zInShadowMap = g_cascadeShadowMap4.Sample(g_sampler, shadowMapUV);
					}
					if (zInLVP > zInShadowMap + 0.0001f * (4 - i)) {
						//影が落ちているので、光を弱くする
						lig *= 0.5f;
					}
					break;
					
					
				}
			}
		}
	}
	//エミッション
	if (isEmission == 1) {
		float emission = g_emissionMap.Sample(g_sampler, In.TexCoord);
		lig += float3(2.5f, 2.5f, 2.5f) * emission;
	}
	//lig *= 0.1f;
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//ambientLight = float3(1.0f,1.0f,1.0f);
	//ここで値を直接入れてます。
	
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
//--------------------------------------------------------------------------------------
// シルエット描画用のピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain_Silhouette(PSInput In) : SV_Target0
{
	return float4(0.5f, 0.5f, 0.5f, 1.0f);
}

// <summary>
/// スキン無しシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(mWorld, In.Position);
	//pos = mul(mView, pos);
	//pos = mul(mProj, pos);
	pos = mul(mLightView, pos);
	pos = mul(mLightProj, pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// スキンありシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMainSkin_ShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	/*pos = mul(mView, pos);
	pos = mul(mProj, pos);*/
	pos = mul(mLightView, pos);
	pos = mul(mLightProj, pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// スキン無しカスケードシャドウマップ生0成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMain_CascadeShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mLightViewProj[shadowMapNumber], pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// スキンありカスケードシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMainSkin_CascadeShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mLightViewProj[shadowMapNumber], pos);
	psInput.Position = pos;
	return psInput;
}

/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z / In.Position.w;
}
