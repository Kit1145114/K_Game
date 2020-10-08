#pragma once
//ゲーム中共通部分をここに記入す。

//グローバル定数

const float gravity = 20.0f;							//重力。
const float UpPhyGhostObjPosition = 200.0f;				//ゴーストオブジェクトの生成位置を決める。
const int m_AnimClipNum = 10;							//アニメーションクリップの数指定。
const int ENAMY_NUM = 3;								//敵を生成する数をここで決める。
const int ZERO = 0;
const CVector3 e_pos1 = { 400.0f,-50.0f,0.0f };			//敵１のポジション。
const CVector3 e_pos2 = { 0.0f,-50.0f,0.0f };			//敵２のポジション。
const CVector3 e_pos3 = { -400.0f,-50.0f,0.0f };		//敵３のポジション。
const CVector2 PIVOT = { 0.0f,0.5f };					//ピボット。
const float	 SPRITERE_SOLUTION_W = 1280.0f;				//画像横
const float	 SPRITERE_SOLUTION_H = 720.0f;				//画像縦
	//ステージの振り分け。
const enum StageNum
{
	First,	//初期ステージ
	Second	//ボスステージ
};