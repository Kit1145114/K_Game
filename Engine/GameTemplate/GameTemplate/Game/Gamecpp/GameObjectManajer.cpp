#include "stdafx.h"
#include "GameObjectManajer.h"

//GameObjectManajer�N���X�̃C���X�^���X�B
GameObjectManajer g_goMgr;

bool GameObjectManajer::Start()
{
	for (auto go : m_goList)
	{
		if (!go->GetIsStart())
		{
			go->SetIsStart(true);
			go->Start();
		}
	}
	return true;
}

void GameObjectManajer::Update()
{
	Start();
	for (auto go : m_goList) {
		go->AgoUpdate();
	}
	for (auto it = m_goList.begin(); it != m_goList.end();)
	{
		if ((*it)->IsRecestDelete())
		{
			delete* it;
			it = m_goList.erase(it);
		}
		else {
			//���N�G�X�g���󂯂ĂȂ��̂�
			it++;
		}
	}
}

//GameObjectManajer* GameObjectManajer::GetInstance()
//{
//	//�ǂ�����ł��Q�[�����Ăяo����悤��
//	//�C���X�^���X��return�ŕԂ��B
//	return m_instance;
//}

void GameObjectManajer::BackUp()
{
	//�t���[���o�b�t�@�̂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);
}

void GameObjectManajer::PreRender()
{

}

void GameObjectManajer::ForwordRender()
{
	////�����_�����O�^�[�Q�b�g�����C���ɕύX����B
	//auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//ChangeRenderTarget(d3dDeviceContext, &m_mainRenderTarget, &m_frameBufferViewports);
	////���C�������_�����O�^�[�Q�b�g���N���A����B
	//float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//m_mainRenderTarget.ClearRenderTarget(clearColor);
}

void GameObjectManajer::PostRender()
{
	for (auto go : m_goList) {
		go->PostRenderWarapper();
	}
}

void GameObjectManajer::FontRender()
{
	for (auto go : m_goList) {
		go->FontRenderWarapper();
	}
}

void GameObjectManajer::ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, RenderTarget * renderTarget, D3D11_VIEWPORT * viewport)
{
	ChangeRenderTarget(
		d3dDeviceContext,
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}

void GameObjectManajer::ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, ID3D11RenderTargetView * renderTarget, ID3D11DepthStencilView * depthStensil, D3D11_VIEWPORT * viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//�����_�����O�^�[�Q�b�g�̐؂�ւ��B
	d3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//�r���[�|�[�g���w�肳��Ă�����A�r���[�|�[�g���ύX����B
		d3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GameObjectManajer::InitCamera()
{
	//g_camera3D.SetPosition({ 0.0f, 1000.0f, 2200.0f });
	//g_camera3D.SetTarget({ 0.0f, 200.0f, 0.0f });
	//g_camera3D.Update();
	//g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	//g_camera2D.SetWidth(FRAME_BUFFER_W);
	//g_camera2D.SetHeight(FRAME_BUFFER_H);
	//g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	//g_camera2D.SetTarget(CVector3::Zero());
	//g_camera2D.Update();
}

void GameObjectManajer::Render()
{
	//�v�������_�����O
	PreRender();

	//�t�H���[�h�����_�����O�B
	ForwordRender();

	//�|�X�g�����_�����O
	PostRender();
}