#include "stdafx.h"
#include "Title.h"
#include"Game.h"

Title::Title()
{
	m_title.Init(L"Assets/sprite/title1.dds", 1280, 720);
}

Title::~Title()
{
}

void Title::Update()
{
	m_title.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	Draw();
	if (g_pad[0].IsTrigger(enButtonA))
	{
		Death();
		this->SetUpdateFlag(false);
	}
}
void Title::Draw()
{
	CMatrix mView;
	CMatrix mProj;
	mView.MakeLookAt(
		{ 0, 0, 1 },
		{ 0, 0, 0 },
		{ 0,1,0 }
	);
	mProj.MakeOrthoProjectionMatrix(1280, 720, 0.1, 100);
	m_title.Draw(mView, mProj);
}
void Title::Death()
{
	game = g_goMgr.NewAGO<Game>();
	g_goMgr.QutavaleyaAGO(this);
}
