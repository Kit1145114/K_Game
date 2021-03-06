#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"
#include "shadow\ShadowMap.h"
#include "shadow\CascadeShadowMap.h"
#include "graphics/SkinModelEffect.h"

SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//定数バッファを解放。
		m_cb->Release();
	}
	if (m_lightCb != nullptr)
	{
		//開放
		m_lightCb->Release();
	}
	if (m_samplerState != nullptr) {
		//サンプラステートを解放。
		m_samplerState->Release();
	}
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//スケルトンのデータを読み込む。
	InitSkeleton(filePath);

	//定数バッファの作成。
	InitConstantBuffer();

	//サンプラステートの初期化。
	InitSamplerState();

	//ディレクションライトの初期化。
	InitDirectionLight();

	//SkinModelDataManagerを使用してCMOファイルのロード。
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

	m_enFbxUpAxis = enFbxUpAxis;
}

void SkinModel::InitTexture(TextureData* textureData)
{
	//D3Dデバイスを取得。
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();

	if (textureData->normalFilePath != nullptr) {
		DirectX::CreateDDSTextureFromFileEx(
			d3dDevice,
			textureData->normalFilePath,
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			false,
			nullptr,
			&m_normalMap);
	}
	
	
	if (textureData->specFilePath != nullptr) {
		DirectX::CreateDDSTextureFromFileEx(
			d3dDevice,
			textureData->specFilePath,
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			false,
			nullptr,
			&m_specMap);
	}
	
	if (textureData->emissionFilePath != nullptr) {
		DirectX::CreateDDSTextureFromFileEx(
			d3dDevice,
			textureData->emissionFilePath,
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			false,
			nullptr,
			&m_emissionMap);
	}
}

void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if ( result == false ) {
		//スケルトンが読み込みに失敗した。
		//アニメーションしないモデルは、スケルトンが不要なので
		//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(SVSConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																//アライメントって→バッファのサイズが16の倍数ということです。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																//CPUアクセスが不要な場合は0。
	//作成。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	{
		//作成するバッファのサイズをsizeof演算子で求める。
		int bufferSize = sizeof(SLight);
		bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;		//SDirectionLightは16byteの倍数になっているので、切り上げはやらない。
		g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
	}
}
void SkinModel::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation( position );
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion( rotation );
	rotMatrix.Mul(mBias, rotMatrix);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//スケルトンの更新。
	m_skeleton.Update(m_worldMatrix);

	if (m_isShadowCaster) {
		g_graphicsEngine->GetShadowMap()->RegistShadowCaster(this);
		g_graphicsEngine->GetCascadeShadowMap()->RegistShadowCaster(this);
	}
	
}
void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix,int Spec, EnRenderMode enRenderMode)
{
	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
	
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	auto shadowMap = g_graphicsEngine->GetShadowMap();
	auto cascadeMap = g_graphicsEngine->GetCascadeShadowMap();
	//定数バッファの内容を更新。
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	
	if (enRenderMode == enRenderMode_CreateCascadeShadowMap) {
		vsCb.shadowMapNumber = cascadeMap->GetShadowMapNumber();
	}
	else if (enRenderMode == enRenderMode_CreateShadowMap) {
		vsCb.mView = shadowMap->GetLightViewMatrix();
		vsCb.mProj = shadowMap->GetLightProjMatrix();
	}
	else if (enRenderMode == enRenderMode_Normal) {
		vsCb.mView = viewMatrix;
		vsCb.mProj = projMatrix;
	}
	vsCb.mLightView = shadowMap->GetLightViewMatrix();
	vsCb.mLightProj = shadowMap->GetLightProjMatrix();

	if (m_isShadowReciver) {
		vsCb.isShadowReciever = 1;
	}
	else {
		vsCb.isShadowReciever = 0;
	}
	for (int i = 0; i < CascadeShadowMap::SHADOWMAP_NUM; i++) {
		ID3D11ShaderResourceView* srvArray[]{
			cascadeMap->GetRenderTarget(i)->GetRenderTargetSRV()
		};
		//引数がポインタのポインタ、t2なので引数を2、1にしてる
		d3dDeviceContext->PSSetShaderResources(3 + i, 1, srvArray);
		vsCb.mLightViewProj[i] = cascadeMap->GetLightViewProjMatrix(i);
		vsCb.mFar[i] = CVector4(cascadeMap->GetFar(i),0.0f,0.0f,0.0f);
		vsCb.mLightViewInv[i] = cascadeMap->GetLightViewInv(i);
	}
	
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//ライト用の定数バッファを更新。
	m_sLight.eyePos = g_camera3D.GetPosition();
	m_sLight.specPow = 3.0f;
	SetTexture();
	//m_sLight.hasSpec = Spec;
	d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_sLight, 0, 0);
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//ボーン行列をGPUに転送。
	m_skeleton.SendBoneMatrixArrayToGPU();

	ID3D11ShaderResourceView* srvArray[]{
	shadowMap->GetShadowMapSRV()
	};
	//引数がポインタのポインタ、t2なので引数を2、1にしてる
	d3dDeviceContext->PSSetShaderResources(2, 1, srvArray);

	

	m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
		auto modelMaterial = reinterpret_cast<ModelEffect*>(material);
		modelMaterial->SetRenderMode(enRenderMode);
	});

	//描画。
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}

void SkinModel::SetTexture()
{
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	if (m_normalMap != nullptr) {
		d3dDeviceContext->PSSetShaderResources(enSkinModelSRVReg_NormalTexture, 1, &m_normalMap);
		m_sLight.isNormal = 1;
	}
	else {
		m_sLight.isNormal = 0;
	}
	if (m_specMap != nullptr) {
		d3dDeviceContext->PSSetShaderResources(enSkinModelSRVReg_SpecTexture, 1, &m_specMap);
		m_sLight.isSpec = 1;
	}
	else {
		m_sLight.isSpec = 0;
	}
	if (m_emissionMap != nullptr) {
		d3dDeviceContext->PSSetShaderResources(enSkinModelSRVReg_EmissionTexture, 1, &m_emissionMap);
		m_sLight.isEmission = 1;
	}
	else {
		m_sLight.isEmission = 0;
	}
}

void SkinModel::InitDirectionLight()
{
	
	m_sLight.direction[0] = { 0.577f, -0.577f, 0.577f, 0.0f };
	m_sLight.color[0] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_sLight.direction[1] = { -0.707f, -0.707f, 0.0f, 0.0f };
	m_sLight.color[1] = { 0.3f, 0.3f, 0.3f, 1.0f };
	m_sLight.direction[2] = { 0.0f, -0.707f, -0.707f, 0.0f };
	m_sLight.color[2] = { 0.3f, 0.3f, 0.3f, 1.0f };
	m_sLight.direction[3] = { 1.0f, 0.0f, -1.0f, 0.0f };
	m_sLight.color[3] = { 0.3f, 0.3f, 0.3f, 1.0f };
	m_sLight.ambientLight = { 10.0f,10.0f,10.0f };
	auto cascade = g_graphicsEngine->GetCascadeShadowMap();
	/*m_sLight.direction = cascade->GetLightDir();
	m_sLight.color = { 2.7f,2.7f,2.7f,0.0f };*/
}