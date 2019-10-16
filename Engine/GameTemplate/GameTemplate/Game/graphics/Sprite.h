#pragma once
#include "graphics/shader.h"

class Sprite
{
public:
	/// <summary>
	/// �C���X�g���N�^
	/// </summary>
	Sprite();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Sprite();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="textureFilePath">�e�N�X�`���̃t�@�C���p�X</param>
	void Init(const wchar_t* textureFilePath, float w, float h);
	/// <summary>
	/// ���[���h�s��̍X�V
	/// </summary>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	void UpdateWorldMatrix(CVector3 pos, CQuaternion rot, CVector3 scale);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="mView">�J�����s��</param>
	/// <param name="mProj">�v���W�F�N�V�����s��</param>
	void Draw(CMatrix mView, CMatrix mProj);
private:
	/// <summary>
	/// �V�F�[�_�[�����[�h�B
	/// </summary>
	void LoadShader();
	/// <summary>
	/// ���_�o�b�t�@���쐬�B
	/// </summary>
	void CreateVertexBuffer(float w, float h);
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���쐬�B
	/// </summary>
	void CreateIndexBuffer();
	/// <summary>
	/// �T���v���X�e�[�g���쐬�B
	/// </summary>
	void CreateSamplerState();
	/// <summary>
	/// �萔�o�b�t�@�̍쐬�B
	/// </summary>
	void CreateConstantBuffer();
	/// <summary>
	/// <param name="textureFilePath">���[�h����e�N�X�`���̃t�@�C���p�X</param>
	void LoadTexture(const wchar_t* textureFilePath);
private:

	Shader m_vs;
	Shader m_ps;
	ID3D11Buffer*	m_vertexBuffer = nullptr;
	ID3D11Buffer*	m_indexBuffer = nullptr;
	ID3D11Buffer*	m_cbGPU = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;
	CMatrix m_world = CMatrix::Identity();
};

