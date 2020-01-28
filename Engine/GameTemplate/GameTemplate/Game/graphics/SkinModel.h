#pragma once

#include "Skeleton.h"
#include "system/system.h"
#include "graphics/Shader.h"
#include"animation/Animation.h"
#include"animation/AnimationClip.h"
#include"GameObjectManajer.h"
#include"c3dmodel/C3DModelEffect.h"

/*!
*@brief	FBX�̏�����B
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};

const int NUM_DIRECTION_LIG = 4;	//�f�B���N�V�������C�g(�l�ύX�s��)

/// <summary>
/// �f�B���N�V�������C�g�B
/// </summary>
struct DirectionLight
{
	CVector4 direction[NUM_DIRECTION_LIG];	//���C�g�̕����B
	CVector4 color[NUM_DIRECTION_LIG];		//���C�g�̃J���[�B
};
struct SLight
{
	DirectionLight m_dirLight;
	CVector3	eyePos;			//���_�̍��W�B
	float		specPow;		//���ʔ��˂̍i��
	CVector3	ambientLight;	//�A���r�G���g���C�g�B
	int			hasSpec;		//�X�y�L�������C�g�𓖂�
};

/*!
*@brief	�X�L�����f���N���X�B
*/
class SkinModel
{ 
public:
	//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~SkinModel();
	
	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	�{�[���������B
	*@param[in]		boneName	�{�[���̖��O�B
	*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/// <summary>
	/// �RD���f���̃��[���h��Ԃł̉�]����
	/// </summary>
	void SetRotation(const CQuaternion & rotation)
	{
		m_rotation = rotation;
	}

	/*!
	*@brief	���f����`��B
	*@param[in]	viewMatrix		�J�����s��B
	*  ���[���h���W�n��3D���f�����J�������W�n�ɕϊ�����s��ł��B
	*@param[in]	projMatrix		�v���W�F�N�V�����s��B
	*  �J�������W�n��3D���f�����X�N���[�����W�n�ɕϊ�����s��ł��B
	*/
	void Draw( CMatrix viewMatrix, CMatrix projMatrix,int Spec);
	/*!
	*@brief	�X�P���g���̎擾�B
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	���b�V������������B
	*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
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
	*@brief	SRV�̃��W�X�^�ԍ��B
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
	};
	//�A�j���[�V�����C�x���g
	void AddAnimationEventListener(std::function<void(const wchar_t* clipName, const wchar_t* eventName)> eventListener)
	{
		m_animation.AddAnimationEventListener(eventListener);
	}
private:
	/*!
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState();
	/*!
	*@brief	�萔�o�b�t�@�̍쐬�B
	*/
	void InitConstantBuffer();
	/*!
	*@brief	�X�P���g���̏������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*/
	void InitSkeleton(const wchar_t* filePath);
	//�f�B���N�V�������C�g�̏������B
	void InitDirectionLight();
private:
	//�萔�o�b�t�@�B
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
	};
	Animation			m_animation;					//�A�j���[�V�����B
	AnimationClip		m_animclip;					//�A�j���[�V�����N���b�v�B
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBX�̏�����B
	ID3D11Buffer*		m_cb = nullptr;					//!<�萔�o�b�t�@�B
	Skeleton			m_skeleton;						//!<�X�P���g���B
	CMatrix				m_worldMatrix;					//!<���[���h�s��B
	DirectX::Model*		m_modelDx;						//!<DirectXTK���񋟂��郂�f���N���X�B
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11ShaderResourceView* m_albedoTextureSRV = nullptr;	//!<�A���x�h�e�N�X�`����SRV//!<�T���v���X�e�[�g�B
	ID3D11Buffer*		m_lightCb = nullptr;				//!<���C�g�p�̒萔�o�b�t�@�B
	//DirectionLight		m_dirLight;							//!<�f�B���N�V�������C�g�B
	SLight				m_sLight;							//!<���C�g�̍\���́B
	CQuaternion			m_rotation = CQuaternion::Identity();//��]
};

