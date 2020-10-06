#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include"GameSystem/Title.h";
#include"Game.h"
#include"GameObjectManager.h"
#include"GameSystem/GameConst.h"
///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	//サウンドエンジンのインスタンス生成
	g_soundEngine = new CSoundEngine();
	g_soundEngine->Init();
	//エフェクトのインスタンス生成。
	g_effektEngine = new CEffektEngine();
	g_effektEngine->InitEffekseer();
	//タイトルのNewGOで生成。
	Title* title = g_goMgr.NewGO<Title>();
	
	CStopwatch sw;
	//ゲームループ。
	while (DispatchWindowMessage() == true)
	{
		//1フレームの時間計測を開始。
		sw.Start();
		//描画開始
		g_graphicsEngine->BegineRender();
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		g_soundEngine->Update();
		//Effekseerカメラ行列を設定。
		g_effektEngine->CameraEffekseer();
		//Effekseerを更新。
		g_effektEngine->Update();
		//物理エンジンの更新。
		g_physics.Update();
		g_goMgr.Update();
		g_graphicsEngine->RenderToShadowMap();

		g_graphicsEngine->ChangeMainRenderTarget();

		g_goMgr.Draw();

		g_effektEngine->Draw();

		g_goMgr.PostRender();
		g_goMgr.FontRender();
		//物理判定を見ないときはコメントアウト
		//g_physics.DebugDraw();
		
		g_goMgr.Delete();


		g_graphicsEngine->PostRender();
		//描画終了。
		g_graphicsEngine->EndRender();
		//1フレームの時間計測を終了。
		sw.Stop();
		//1フレームにかかった時間を記憶。
		GameTime().PushFrameDeltaTime(sw.GetElapsed());
	}
}