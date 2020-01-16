#pragma once
//ゲーム中でプレイヤーと敵の共通部分をここに記入す。

//グローバル定数
const float gravity = 100.0f;				//重力。
const int m_AnimClipNum = 5;				//アニメーションクリップの数指定。
const int ENAMY_NUM = 3;					//敵を生成する数をここで決める。
const CVector3 e_pos1 = { 400.0f,-50.0f,0.0f };	//敵１のポジション。
const CVector3 e_pos2 = { 0.0f,-50.0f,0.0f };	//敵２のポジション。
const CVector3 e_pos3 = { -400.0f,-50.0f,0.0f };	//敵３のポジション。
