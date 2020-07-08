/*!
 * @brief	���f���V�F�[�_�[�B
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
/// 
static const int NUM_CASCADES = 4;
//�A���x�h�e�N�X�`���B
Texture2D<float4> g_albedoTexture : register(t0);	
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);

Texture2D<float4> shadowMap : register(t2);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap1 : register(t3);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap2 : register(t4);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap3 : register(t5);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap4 : register(t6);		//todo �V���h�E�}�b�v�B

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler g_sampler : register(s0);

/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//todo ���C�g�r���[�s���ǉ��B
	float4x4 mLightView;	//���C�g�r���[�s��B
	float4x4 mLightProj;	//���C�g�v���W�F�N�V�����s��B
	float4x4 mLightViewProj[NUM_CASCADES];
	float4 mFar;
	int isShadowReciever;	//�V���h�E���V�[�o�[�t���O�B
	int shadowMapNumber = 0;
};
/// <summary>
/// ���C�g�p�̒萔�o�b�t�@
/// </summary>
cbuffer LightCb : register(b1) {
	float3 dligDirection[4];
	float4 dligColor[4];
	float3 eyePos;
	float specPow;
	float3 ambientLight;
	int hasSpec;
};
/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float3 Tangent  : TANGENT;				//�ڃx�N�g���B
    float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float2 TexCoord	: TEXCOORD0;			//UV���W�B
    float3 Tangent	: TANGENT;				//�ڃx�N�g���B
    uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
    float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;
	float4 posInLVP[NUM_CASCADES]		: TEXCOORD2;	//���C�g�r���[�v���W�F�N�V������Ԃł̍��W
};

/// <summary>
/// �V���h�E�}�b�v�p�̃s�N�Z���V�F�[�_�ւ̓��͍\���́B
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//���W�B
};

/*!
 *@brief	�X�L���s����v�Z�B
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
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	psInput.worldPos = pos;
	if (isShadowReciever == 1) {
		//���C�g�r���[�v���W�F�N�V�������W�ɕϊ�
		for (int i = 0; i < NUM_CASCADES; i++) {
			psInput.posInLVP[i] = mul(mLightViewProj[i], pos);
		}
	}
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));

	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
	    pos = mul(skinning, In.Position);
		psInput.worldPos = pos;
	}
	if (isShadowReciever == 1) {
		//���C�g�r���[�v���W�F�N�V�������W�ɕϊ�
		for (int i = 0; i < NUM_CASCADES; i++) {
			psInput.posInLVP[i] = mul(mLightViewProj[i], pos);
		}
	}
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	//albedo�e�N�X�`������J���[���t�F�b�`����B
	float4 albedoColor = g_albedoTexture.Sample(g_sampler, In.TexCoord);
	//�f�B���N�V�������C�g�̊g�U���ˌ����v�Z����B
	float3 lig = 0.0f;
	lig += float3(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 4; i++) {
		//lig += max(0.0f, dot(In.Normal * -1.0f, dligDirection[i])) * dligColor[i];
		//���ʔ��˂̌��̗ʂ��v�Z����B
		if (hasSpec == 1) {
			float3 rv = reflect(dligDirection[i], In.Normal);
			float3 toEye = normalize(eyePos - In.worldPos);
			float t = max(0.0f, dot(toEye, rv));
			t = pow(t, 4.0f);
			//lig += t * dligColor[i] * specPow;
		}
	}
	int count = 0;

	if (isShadowReciever == 1) {
		/*float dist = In.Position.w;
		if (dist < mFar.x) {
			float2 shadowMapUV = In.posInLVP[0].xy / In.posInLVP[0].w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//spsOut.shadow = shadowMapUV.x;
			//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
				///LVP��Ԃł̐[�x�l���v�Z�B
				float zInLVP = In.posInLVP[0].z / In.posInLVP[0].w;
				float zInShadowMap;
				//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
				zInShadowMap = g_cascadeShadowMap1.Sample(g_sampler, shadowMapUV);
				if (zInLVP > zInShadowMap + 0.0002f) {
					//�e�������Ă���̂ŁA�����キ����
					lig *= 0.5f;
				}
			}
		}
		else  if (dist < mFar.x) {
			float2 shadowMapUV = In.posInLVP[1].xy / In.posInLVP[1].w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//spsOut.shadow = shadowMapUV.x;
			//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
				///LVP��Ԃł̐[�x�l���v�Z�B
				float zInLVP = In.posInLVP[1].z / In.posInLVP[1].w;
				float zInShadowMap;
				//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
				zInShadowMap = g_cascadeShadowMap2.Sample(g_sampler, shadowMapUV);
				if (zInLVP > zInShadowMap + 0.0002f) {
					//�e�������Ă���̂ŁA�����キ����
					lig *= 0.5f;
				}
			}
		}
		else 	if (dist < mFar.x) {
			float2 shadowMapUV = In.posInLVP[2].xy / In.posInLVP[2].w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//spsOut.shadow = shadowMapUV.x;
			//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
				///LVP��Ԃł̐[�x�l���v�Z�B
				float zInLVP = In.posInLVP[2].z / In.posInLVP[2].w;
				float zInShadowMap;
				//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
				zInShadowMap = g_cascadeShadowMap3.Sample(g_sampler, shadowMapUV);
				if (zInLVP > zInShadowMap + 0.0002f) {
					//�e�������Ă���̂ŁA�����キ����
					lig *= 0.5f;
				}
			}
		}
		else 	if (dist < mFar.x) {
			float2 shadowMapUV = In.posInLVP[3].xy / In.posInLVP[3].w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//spsOut.shadow = shadowMapUV.x;
			//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
				///LVP��Ԃł̐[�x�l���v�Z�B
				float zInLVP = In.posInLVP[3].z / In.posInLVP[3].w;
				float zInShadowMap;
				//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
				zInShadowMap = g_cascadeShadowMap4.Sample(g_sampler, shadowMapUV);
				if (zInLVP > zInShadowMap + 0.0002f) {
					//�e�������Ă���̂ŁA�����キ����
					lig *= 0.5f;
				}
			}
		}*/
		for (int i = 0; i < NUM_CASCADES; i++) {
		
			//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
			//�v���W�F�N�V�����s����V���h�E�}�b�v��UV���W�ɕϊ����Ă���
			float2 shadowMapUV = In.posInLVP[i].xy / In.posInLVP[i].w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;
			//spsOut.shadow = shadowMapUV.x;
			//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
			if (shadowMapUV.x < 1.0f
				&& shadowMapUV.x > 0.0f
				&& shadowMapUV.y < 1.0f
				&& shadowMapUV.y > 0.0f
				) {
				///LVP��Ԃł̐[�x�l���v�Z�B
				float zInLVP = In.posInLVP[i].z / In.posInLVP[i].w;
				float zInShadowMap;
				//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
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
				if (zInLVP > zInShadowMap + 0.0002f) {
					//�e�������Ă���̂ŁA�����キ����
					lig *= 0.5f;
					//	spsOut.shadow = zInShadowMap;
						//spsOut.shadow = zInLVP;
						//spsOut.shadow = psIn.pos.z / psIn.pos.w;
					break;
				}
				else if (count == 0) {
					count++;
				}
				else {
					break;
				}
			}
		}
	}
	//lig *= 0.1f;
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//ambientLight = float3(1.0f,1.0f,1.0f);
	//�����Œl�𒼐ړ���Ă܂��B
	
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
//--------------------------------------------------------------------------------------
// �V���G�b�g�`��p�̃s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain_Silhouette(PSInput In) : SV_Target0
{
	return float4(0.5f, 0.5f, 0.5f, 1.0f);
}

// <summary>
/// �X�L�������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//���C�g�v���W�F�N�V�����s��ɕϊ����Ă���
	float4 pos = mul(mWorld, In.Position);
	//pos = mul(mView, pos);
	//pos = mul(mProj, pos);
	pos = mul(mLightView, pos);
	pos = mul(mLightProj, pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// �X�L������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
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
/// �X�L�������J�X�P�[�h�V���h�E�}�b�v��0���p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput_ShadowMap VSMain_CascadeShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//���C�g�v���W�F�N�V�����s��ɕϊ����Ă���
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mLightViewProj[shadowMapNumber], pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// �X�L������J�X�P�[�h�V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
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
/// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//�ˉe��Ԃł�Z�l��Ԃ��B
	return In.Position.z / In.Position.w;
}
