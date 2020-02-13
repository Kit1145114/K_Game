#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include"Title.h";
#include"Game.h"
#include"GameObjectManajer.h"
#include"GameConst.h"
///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//�^�C�g����NewGO�Ő����B
	Title* title = g_goMgr.NewAGO<Title>();
	//�Q�[�����[�v�B
	while (DispatchWindowMessage() == true)
	{
		//�`��J�n
		g_graphicsEngine->BegineRender();
		//�Q�[���p�b�h�̍X�V�B	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//�����G���W���̍X�V�B
		g_physics.Update();
		g_goMgr.Update();
		g_physics.DebugDraw();
		g_goMgr.PostRender();
		//�Q�[���̍X�V�B
		//game->Update();
		//�^�C�g���̕\��
		//title->Update();
		//���x���̕`��
		//level.Draw();
		//�`��I���B
		g_graphicsEngine->EndRender();
	}
}