#include "stdafx.h"
#include "C3DModelEffect.h"


C3DModelEffect::C3DModelEffect()
{
	//���_�V�F�[�_�[�����[�h�B
	m_vsShader.Load("Assets/shader/model.fx", "VSMain", Shader::EnType::VS);
	m_psShader.Load("Assets/shader/model.fx", "PSMain", Shader::EnType::PS);
	m_psSilhouette.Load("Assets/shader/model.fx", "PSMain_Silhouette", Shader::EnType::PS);

	//�f�v�X�X�e���V���̏������B
	InitSilhouettoDepthStepsilState();
}

C3DModelEffect::~C3DModelEffect()
{
	if (m_albedoTexture != nullptr) {
		m_albedoTexture->Release();
	}
	if (m_silhouettoDepthStepsilState != nullptr) {
		m_silhouettoDepthStepsilState->Release();
	}
}

void C3DModelEffect::InitSilhouettoDepthStepsilState()
{
	//D3D�f�o�C�X���擾�B
	auto pd3d = g_graphicsEngine->GetD3DDevice();
	//�쐬����[�x�X�e���V���X�e�[�g�̒�`��ݒ肵�Ă����B
	D3D11_DEPTH_STENCIL_DESC desc = { 0 };
	desc.DepthEnable = true;						   //Z�e�X�g���L���B
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; //Z�o�b�t�@��Z�l��`�����܂Ȃ��B
	desc.DepthFunc = D3D11_COMPARISON_GREATER;		   //Z�l���傫����΃t���[���o�b�t�@�ɕ`�����ށB

	pd3d->CreateDepthStencilState(&desc, &m_silhouettoDepthStepsilState);
}

void __cdecl C3DModelEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	//�V�F�[�_�[��K�p����B
	deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
	switch (m_renderMode) {
	case 0:
		//�ʏ�`��B
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShader.GetBody(), NULL, 0);
		deviceContext->PSSetShaderResources(0, 1, &m_albedoTexture);
		break;
	case 1:
		//�V���G�b�g�`��B
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psSilhouette.GetBody(), NULL, 0);
		//�f�v�X�X�e���V���X�e�[�g��؂�ւ���B
		deviceContext->OMSetDepthStencilState(m_silhouettoDepthStepsilState, 0);
		break;
	}
}

void C3DModelEffect::PopAlbedoTexture()
{
	m_albedoTextureStackPos--;
	SetAlbedoTexture(m_albedoTextureStack[m_albedoTextureStackPos]);
}

void C3DModelEffect::PushAlbedoTexture()
{
	m_albedoTextureStack[m_albedoTextureStackPos] = m_albedoTexture;
	m_albedoTexture = nullptr;
	m_albedoTextureStackPos++;
}

void C3DModelEffect::SetAlbedoTexture(ID3D11ShaderResourceView* texSRV)
{
	if (m_albedoTexture != nullptr) {
		//�Q�ƃJ�E���^��������B
		m_albedoTexture->Release();
	}
	m_albedoTexture = texSRV;
	//�Q�ƃJ�E���^���グ��B
	m_albedoTexture->AddRef();
}