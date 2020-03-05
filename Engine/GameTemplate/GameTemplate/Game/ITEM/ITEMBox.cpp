#include "stdafx.h"
#include "ITEMBox.h"


ITEMBox::ITEMBox()
{
	Box.Init(L"Assets/modelData/BOX.cmo");			//モデルの呼び出し。
	box_animClip[open].Load(L"Assets/animData/box_open.tka");	//開けのロード
	box_animClip[open].SetLoopFlag(false);						//箱開け。
	box_anim.Init(
		Box,				//モデル
		box_animClip,		//アニメーションクリップ
		m_AnimClipNum		//アニメーションの数
	);
	m_position = { 0.0f,-50.0f,0.0f };
	m_scale = {3.0f,3.0f,3.0f};
	m_charaCon.Init(100.0f, 50.0f, m_position);	//キャラコンの設定（半径、高さ、初期位置。）
}

ITEMBox::~ITEMBox()
{
}
//アップデート処理。
void ITEMBox::Update()
{
	Draw();
	State();
	if (isOpen)
	{
		box_anim.Update(0.025f);	//アニメーションをフレーム単位で描画。
	}
	Box.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
//ドロー関数。
void ITEMBox::Draw()
{
	Box.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix(),
		1
	);
}

void ITEMBox::State()
{
	if (isOpen)
	{
		box_anim.Play(open);
	}
}