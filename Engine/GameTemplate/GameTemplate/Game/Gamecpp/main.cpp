#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include"Title.h";
#include"Game.h"
#include"GameObjectManajer.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//ゲームをNewGOで生成。
	//Game* game = g_goMgr.NewAGO<Game>();
	//タイトルのNewGOで生成。
	Title* title = g_goMgr.NewAGO<Title>();
	//ゲームループ。
	while (DispatchWindowMessage() == true)
	{
		//描画開始
		g_graphicsEngine->BegineRender();
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//物理エンジンの更新。
		g_physics.Update();
		g_goMgr.Update();
		//ゲームの更新。
		//game->Update();
		//タイトルの表示
		//title->Update();
		//レベルの描画
		//level.Draw();
		//描画終了。
		g_graphicsEngine->EndRender();
	}
}