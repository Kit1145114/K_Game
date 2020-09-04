#pragma once
#include<vector>
#include "GameObject.h"
#include "graphics/RenderTarget.h"
#include "graphics/Sprite.h"


class GameObjectManajer 
{
public:
	bool Start();
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
	/// <summary>
	/// �Q�[���I�u�W�N�g��ǉ�
	/// </summary>
	template<class T>
	T* NewGO()
	{
		T* newObj = new T();
		m_goList.push_back(newObj);
		newObj->SetIsNewGO();
		return newObj;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���烊�X�g���폜����B
	/// </summary>
	/// <param name="go"></param>�폜����Q�[���I�u�W�F�N�g
	void DeleteGO(GameObject* go) {
			if (go->GetIsGO() == true) {
				//���������B
				//���X�g����폜����B
				//m_goList.erase();
				////�C���X�^���X���̂��폜�B
				//delete go;/
				go->RecestDelete();
				//�폜�ł����̂ŏI���B
				return;
		}
	}
	void FindGO(GameObject*go) {
		if (go->GetIsGO() == true)
		{
			return ;
		}
	}
	/// <summary>
	/// 3D���f����`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���f���[�g
	/// </summary>
	void Delete();
	//�����_�[��
	/// </summary>
	void Render();
	/// <summary>
//	static GameObjectManajer* GetInstance();		//�V���O���g���B

	void BackUp();
	/// <summary>
	/// �v�������_�����O�B
	/// </summary>
	void PreRender();
	/// <summary>
	/// �t�H���[�h�����_�����O(�ʏ�̕`�悾�ƍl����OK)
	/// </summary>
	void ForwordRender();
	/// <summary>
	/// �|�X�g�����_�����O
	/// </summary>
	void PostRender();
	/// <summary>
	/// �t�H���g�����_�����O
	/// </summary>
	void FontRender();
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̐؂�ւ��B
	/// </summary>
	/// <param name="d3dDeviceContext">D3D�f�o�C�X�R���e�L�X�g</param>
	/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
	/// <param name="viewport">�r���[�|�[�g</param>
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	/// <summary>
	/// �J�������������B
	/// </summary>
	void InitCamera();
private:
	//�����_�[
	RenderTarget m_renderTarget;
	static GameObjectManajer* m_instance;		//�Q�[���̃C���X�^���X�B//�����_�����O�^�[�Q�b�g�B
	//�ϒ��z��
	std::list<GameObject*>m_goList;			//�Q�[���I�u�W�F�N�g�̃��X�g
	std::list<GameObject*>m_delete_goList;	//�f���[�g���܂��B
	//�����_�[�^�[�Q�b�g
	Sprite m_copyMainRtToFrameBufferSprite;			//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�B
	D3D11_VIEWPORT m_frameBufferViewports;			//�t���[���o�b�t�@�̃r���[�|�[�g�B
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�B
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[�B
};

extern GameObjectManajer g_goMgr;
