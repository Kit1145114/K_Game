#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include"GameSystem/Title.h";
#include"Game.h"
#include"GameObjectManager.h"
#include"GameSystem/GameConst.h"
///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	//�T�E���h�G���W���̃C���X�^���X����
	g_soundEngine = new CSoundEngine();
	g_soundEngine->Init();
	//�G�t�F�N�g�̃C���X�^���X�����B
	g_effektEngine = new CEffektEngine();
	g_effektEngine->InitEffekseer();
	//�^�C�g����NewGO�Ő����B
	Title* title = g_goMgr.NewGO<Title>();
	
	CStopwatch sw;
	//�Q�[�����[�v�B
	while (DispatchWindowMessage() == true)
	{
		//1�t���[���̎��Ԍv�����J�n�B
		sw.Start();
		//�`��J�n
		g_graphicsEngine->BegineRender();
		//�Q�[���p�b�h�̍X�V�B	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		g_soundEngine->Update();
		//Effekseer�J�����s���ݒ�B
		g_effektEngine->CameraEffekseer();
		//Effekseer���X�V�B
		g_effektEngine->Update();
		//�����G���W���̍X�V�B
		g_physics.Update();
		g_goMgr.Update();
		g_graphicsEngine->RenderToShadowMap();

		g_graphicsEngine->ChangeMainRenderTarget();

		g_goMgr.Draw();

		g_effektEngine->Draw();

		g_goMgr.PostRender();
		g_goMgr.FontRender();
		//������������Ȃ��Ƃ��̓R�����g�A�E�g
		//g_physics.DebugDraw();
		
		g_goMgr.Delete();


		g_graphicsEngine->PostRender();
		//�`��I���B
		g_graphicsEngine->EndRender();
		//1�t���[���̎��Ԍv�����I���B
		sw.Stop();
		//1�t���[���ɂ����������Ԃ��L���B
		GameTime().PushFrameDeltaTime(sw.GetElapsed());
	}
}