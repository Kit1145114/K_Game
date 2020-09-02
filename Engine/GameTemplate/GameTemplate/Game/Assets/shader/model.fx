/*!
 * @brief	���f���V�F�[�_�[�B
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
/// 
static const int NUM_CASCADES = 3;
//�A���x�h�e�N�X�`���B
Texture2D<float4> g_albedoTexture : register(t0);	
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);

Texture2D<float4> shadowMap : register(t2);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap1 : register(t3);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap2 : register(t4);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap3 : register(t5);		//todo �V���h�E�}�b�v�B
Texture2D<float4> g_cascadeShadowMap4 : register(t6);		//todo �V���h�E�}�b�v�B

Texture2D<float4> g_normalMap : register(t7);
Texture2D<float4> g_specMap : register(t8);
Texture2D<float4> g_emissionMap : register(t9);

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
	float4x4 mLightViewInv[NUM_CASCADES];		//���C�g�r���[�̋t�s��
	float4 mFar[NUM_CASCADES];
	int isShadowReciever;	//�V���h�E���V�[�o�[�t���O�B
	int shadowMapNumber = 0;
};
static const int NUM_LIGHTS = 4;
/// <summary>
/// ���C�g�p�̒萔�o�b�t�@
/// </summary>
cbuffer LightCb : register(b1) {
	float4		dligDirection[NUM_LIGHTS];	//���C�g�̕����B
	float4		dligColor[NUM_LIGHTS];		//���C�g�̃J���[�B
	float3	    eyePos;			//���_�̍��W�B
	float		specPow;		//���ʔ��˂̍i��
	float3		ambientLight;	//�A���r�G���g���C�g�B
	int			isSpec;		//�X�y�L�������C�g�𓖂�
	int			isNormal;
	int			isEmission;
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
	float4 posInLVP[NUM_CASCADES]	: TEXCOORD2;	//���C�g�r���[�v���W�F�N�V������Ԃł̍��W
	float4 posInCamera : TEXCOORD6;					//�J������Ԃł̍��W�B
	float4 posInLVP2   : TEXCOORD7;
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
		float4 p = pos;
		p = mul(mLightView, p);
		psInput.posInLVP2 = mul(mLightProj, p);
		//���C�g�r���[�v���W�F�N�V�������W�ɕϊ�
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
		float4 p = pos;
		p = mul(mLightView, p);
		psInput.posInLVP2 = mul(mLightProj, p);
		//���C�g�r���[�v���W�F�N�V�������W�ɕϊ�
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
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	//albedo�e�N�X�`������J���[���t�F�b�`����B
	float4 albedoColor = g_albedoTexture.Sample(g_sampler, In.TexCoord);
	//�f�B���N�V�������C�g�̊g�U���ˌ����v�Z����B
	float3 lig = 0.0f;
	lig += float3(0.7f, 0.7f, 0.7f);

	


	float3 normal = 0;
	if (isNormal == 1) {
		//�@���}�b�v������B
		//�@���Ɛڃx�N�g���̊O�ς��v�Z���āA�]�x�N�g�����v�Z����B
		float3 biNormal = cross(In.Normal, In.Tangent);
		normal = g_normalMap.Sample(g_sampler, In.TexCoord);
		//0.0�`1.0�͈̔͂ɂȂ��Ă���^���W�F���g�X�y�[�X�@����
		//-1.0�`1.0�͈̔͂ɕϊ�����B
		normal = (normal * 2.0f) - 1.0f;
		//�@�����^���W�F���g�X�y�[�X����A���[���h�X�y�[�X�ɕϊ�����B
		normal = In.Tangent * normal.x + biNormal * normal.y + In.Normal * normal.z;
	}
	else {
		normal = In.Normal;
	}

	for (int i = 0; i < NUM_LIGHTS; i++) {
		//���K�@���ʔ��˂��v�Z���Ȃ����B
				//�@�@���˃x�N�g��R�����߂�B
		float3 R = dligDirection[i].xyz
			+ 2 * dot(In.Normal, -dligDirection[i].xyz)
			* In.Normal;
		//�A�@���_���烉�C�g�𓖂Ă镨�̂ɐL�т�x�N�g��E�����߂�B
		float3 E = normalize(In.worldPos - eyePos);
		//�@�ƇA�ŋ��܂����x�N�g���̓��ς��v�Z����B
		//�X�y�L�������˂̋��������߂�B
		float specPower = max(0, dot(R, -E));
		float spec;
		if (isSpec == 1) {
			//�X�y�L�����}�b�v������B
			spec = g_specMap.Sample(g_sampler, In.TexCoord).r;

			float3 specLig = pow(specPower, 2.0f) * dligColor[i].xyz * spec * 10.0f;
			//�D �X�y�L�������˂����܂�����Alig�ɉ��Z����B
			//���ʔ��˂𔽎ˌ��ɉ��Z����B
			lig += specLig;
		}
		else {
			specPower = pow(specPower, specPow);
			//lig += dligColor[i].xyz * specPower;
		}

		//�g�U����
		lig += max(0.0f, dot(In.Normal * -1.0f, dligDirection[i].xyz)) * dligColor[i].xyz * 1.2f;
	}
	//�G�~�b�V����
	if (isEmission == 1) {
		float emission = g_emissionMap.Sample(g_sampler, In.TexCoord);
		//lig += float3(2.0f, 2.0f, 2.0f) * emission;
	}
	
	/*for (int i = 0; i < 4; i++) {
		//lig += max(0.0f, dot(In.Normal * -1.0f, dligDirection[i])) * dligColor[i];
		//���ʔ��˂̌��̗ʂ��v�Z����B
		if (hasSpec == 1) {
			float3 rv = reflect(dligDirection[i], In.Normal);
			float3 toEye = normalize(eyePos - In.worldPos);
			float t = max(0.0f, dot(toEye, rv));
			t = pow(t, 4.0f);
			//lig += t * dligColor[i] * specPow;
		}
	}*/

	if (isShadowReciever == 1) {
		/*//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
			//�v���W�F�N�V�����s����V���h�E�}�b�v��UV���W�ɕϊ����Ă���
		float2 shadowMapUV = In.posInLVP2.xy / In.posInLVP2.w;
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
			float zInLVP = In.posInLVP2.z / In.posInLVP2.w;
			float zInShadowMap = 1.0f;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B

			zInShadowMap = shadowMap.Sample(g_sampler, shadowMapUV);
			if (zInLVP > zInShadowMap + 0.0001f) {
				//�e�������Ă���̂ŁA�����キ����
				lig *= 0.5f;
			}
		}*/
		for (int i = 0; i < NUM_CASCADES; i++) {
			if (mFar[i].x  > In.posInCamera.z) {
				//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
				//�v���W�F�N�V�����s����V���h�E�}�b�v��UV���W�ɕϊ����Ă���
				float2 shadowMapUV = In.posInLVP[i].xy / In.posInLVP[i].w;
				shadowMapUV *= float2(0.5f, -0.5f);
				shadowMapUV += 0.5f;
				//spsOut.shadow = shadowMapUV.x;
				//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
				if (shadowMapUV.x <= 1.0f
					&& shadowMapUV.x >= 0.0f
					&& shadowMapUV.y <= 1.0f
					&& shadowMapUV.y >= 0.0f
					) {
					///LVP��Ԃł̐[�x�l���v�Z�B
					float zInLVP = In.posInLVP[i].z / In.posInLVP[i].w;
					float zInShadowMap = 1.0f;
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
					if (zInLVP > zInShadowMap + 0.0001f * (4 - i)) {
						//�e�������Ă���̂ŁA�����キ����
						lig *= 0.5f;
					}
					break;
					
					
				}
			}
		}
	}
	//�G�~�b�V����
	if (isEmission == 1) {
		float emission = g_emissionMap.Sample(g_sampler, In.TexCoord);
		lig += float3(2.5f, 2.5f, 2.5f) * emission;
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
