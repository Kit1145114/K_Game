#include "stdafx.h"
#include "ITEMBox.h"


ITEMBox::ITEMBox()
{
	
}

ITEMBox::~ITEMBox()
{
}

bool ITEMBox::Start()
{
	Box.Init(L"Assets/modelData/BOX.cmo");						//モデルの呼び出し。
	box_animClip[open].Load(L"Assets/animData/box_open.tka");	//開けのロード
	box_animClip[open].SetLoopFlag(false);						//箱開け。
	box_animClip[close].Load(L"Assets/animData/box_idle.tka");	//閉じてるロード
	box_animClip[close].SetLoopFlag(false);						//箱開け。
	box_anim.Init(
		Box,				//モデル
		box_animClip,		//アニメーションクリップ
		m_AnimClipNum		//アニメーションの数
	);
	m_position = { 500.0f,-300.0f,500.0f };
	m_scale = { 3.0f,3.0f,3.0f };
	box_anim.Play(close);
	state = close;
	m_charaCon.Init(100.0f, 50.0f, m_position);	//キャラコンの設定（半径、高さ、初期位置。）
	return true;
}
//アップデート処理。
void ITEMBox::Update()
{
	Draw();
	State();
	box_anim.Update(0.25f);
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
//ステート
void ITEMBox::State()
{
	switch (state)
	{
	case ITEMBox::open:
		box_anim.Play(open);
		Open();
		break;
	case ITEMBox::close:
		Close();
		box_anim.Play(close);
		break;
	case ITEMBox::death:
		Delete();
		break;
	}
}
//削除
void ITEMBox::Delete()
{

	g_goMgr.QutavaleyaAGO(this);
}
//開けるかどうか。
void ITEMBox::Open()
{
	if (!box_anim.IsPlaying())
	{
		state = death;
		this->SetActive(false);
		m_charaCon.RemoveRigidBoby();
	}
}

void ITEMBox::Close()
{
	if (isOpen)
	{
		state = open;
	}
}