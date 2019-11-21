#pragma once
#include"graphics/Shader.h"
#include<array>

class C3DModelEffect : public DirectX::IEffect
{
public:
	C3DModelEffect();
	~C3DModelEffect();
	//���̊֐���DirectX::Model::Draw�����̃h���[�R�[���̒��O�ɌĂ΂��B
//�Ȃ̂ŁA���̊֐��̂Ȃ��ŁA�V�F�[�_�[�̐ݒ��A�e�N�X�`���̐ݒ�Ȃǂ��s���Ƃ悢�B
	void __cdecl Apply(ID3D11DeviceContext* deviceContext) override;

	//���̊֐���DirectX::Model�̏�������������Ă΂��B
	//���_�V�F�[�_�[�̃o�C�g�R�[�h�ƃR�[�h�̒�����ݒ肷��K�v������B
	void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
	{
		*pShaderByteCode = m_vsShader.GetByteCode();
		*pByteCodeLength = m_vsShader.GetByteCodeSize();
	}

	void SetAlbedoTexture(ID3D11ShaderResourceView* texSRV);
	/// <summary>
	/// ���݂̃A���x�h�e�N�X�`�����X�^�b�N�ɑޔ�������B
	/// </summary>
	void PushAlbedoTexture();
	/// <summary>
	/// �X�^�b�N����A���x�h�e�N�X�`�����|�b�v����B
	/// </summary>
	void PopAlbedoTexture();

	void SetRenderMode(int renderMode)
	{
		m_renderMode = renderMode;
	}
private:
	Shader m_vsShader;
	Shader m_psShader;
	Shader m_psSilhouette;		//�V���G�b�g�`��p�̃s�N�Z���V�F�[�_�[�B
	ID3D11ShaderResourceView* m_albedoTexture = nullptr;
	//C++11�Œǉ����ꂽ�z��B�]����C�̔z��̃p�t�H�[�}���X���ێ�����
	//�z��̗v�f���̎擾��A���E�`�F�b�N�܂łł���D����́B
	//���ʂ̔z��Ȃ�Ďg��Ȃ��Ă悭�ˁH���ĂȂ�܂��B
	std::array<ID3D11ShaderResourceView*, 4> m_albedoTextureStack = { nullptr };
	int m_albedoTextureStackPos = 0;
	bool m_renderMode = 0;
	ID3D11DepthStencilState* m_silhouettoDepthStepsilState = nullptr;	//�V���G�b�g�`��p�̃f�v�X�X�e���V���X�e�[�g�B

	void InitSilhouettoDepthStepsilState();
};

/*!
*@brief
*  �G�t�F�N�g�t�@�N�g���B
*@detail
* �G�t�F�N�g�̃t�@�N�g���B�}�e���A���̏��ɉ����č쐬����G�t�F�N�g��ύX����N���X�B
*
*/
class C3DModelEffectFactory : public DirectX::EffectFactory {
public:
	C3DModelEffectFactory(ID3D11Device* device) :
		EffectFactory(device) {}
	//�G�t�F�N�g���쐬�B
	std::shared_ptr<DirectX::IEffect> __cdecl CreateEffect(const EffectInfo& info, ID3D11DeviceContext* deviceContext)override
	{
		std::shared_ptr<C3DModelEffect> effect = std::make_shared< C3DModelEffect>();


		if (info.diffuseTexture && *info.diffuseTexture)
		{
			//�A���x�h�e�N�X�`�����\���Ă���B
			ID3D11ShaderResourceView* texSRV;
			DirectX::EffectFactory::CreateTexture(info.diffuseTexture, deviceContext, &texSRV);
			effect->SetAlbedoTexture(texSRV);
			texSRV->Release();
		}

		return effect;
	}
};
