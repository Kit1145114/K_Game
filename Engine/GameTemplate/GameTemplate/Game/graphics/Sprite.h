#pragma once
#include "graphics/shader.h"
#include"GraphicsEngine.h"

class Sprite
{
public:
	static const CVector2	DEFAULT_PIVOT;
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
	//void UpdateWorldMatrix(CVector3 pos, CQuaternion rot, CVector3 scale);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="mView">�J�����s��</param>
	/// <param name="mProj">�v���W�F�N�V�����s��</param>
	void Draw(CMatrix mView, CMatrix mProj);
	void SetActiveFlag(bool flag)
	{
		Active = flag;
	}

	//���u�����h�X�e�[�g�B
	struct AlphaBlendState {
		ID3D11BlendState*	disable;	//!<�A���t�@�u�����f�B���O�������B
		ID3D11BlendState*	trans;		//!<�����������B
		ID3D11BlendState*	add;		//!<���Z�����B
		void Init(GraphicsEngine& ge);
	};

	//�[�x�X�e���V���X�e�[�g�B
	struct DepthStencilState
	{
		ID3D11DepthStencilState* disable;			//!<���ׂĖ����B
		ID3D11DepthStencilState* SceneRender;		//!<3D���f����`�悷�鎞�̊�{�I�Ȑ[�x�X�e���V���X�e�[�g�B
															//!<�[�x�e�X�g�A�[�x�������݂Ƃ��ɗL���ȃX�e�[�g�ł��B
		ID3D11DepthStencilState* spriteRender;		//!<2D�`�悷�鎞�̊�{�I�Ȑ[�x�X�e���V���X�e�[�g�B
															//!<�[�x�e�X�g�A�[�x�������݂Ƃ��ɖ����ȃX�e�[�g�ł��B
		ID3D11DepthStencilState* defferedRender;		//!<�f�B�t�@�[�h�����_�����O���s���Ƃ��̐[�x�X�e���V���X�e�[�g�B
		void Init(GraphicsEngine& ge);												//!<�[�x�e�X�g�����A�[�x�������ݗL���ȃX�e�[�g�ł��B
	};
	//���X�^���C�U�X�e�[�g�B
	struct RasterizerState
	{
		ID3D11RasterizerState*	sceneRender;		//!<3D���f����`�悷�鎞�̊�{�I�ȃ��X�^���C�U�X�e�[�g�B
		ID3D11RasterizerState*	spriteRender;		//!<2D�`�悷�鎞�̊�{�I�ȃ��X�^���C�U�X�e�[�g�B
		void Init(GraphicsEngine& ge);
	};
	void InitDepthStencil();
public:
	void Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot, float a = 0.5f);
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
	bool Active = true;
	Shader m_vs;
	Shader m_ps;
	ID3D11Buffer*	m_vertexBuffer = nullptr;
	ID3D11Buffer*	m_indexBuffer = nullptr;
	ID3D11Buffer*	m_cbGPU = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;
	CMatrix m_world = CMatrix::Identity();
	CVector2				m_size = CVector2::Zero();		//!<�T�C�Y�B
	float m_alpha = 1.0f;									//���l
	ID3D11RasterizerState* m_rasterizer = nullptr;
	ID3D11BlendState* m_alphablend = nullptr;
	ID3D11DepthStencilState* m_depthstencil = nullptr;

	ID3D11Device* ps2d = nullptr;
	


};

