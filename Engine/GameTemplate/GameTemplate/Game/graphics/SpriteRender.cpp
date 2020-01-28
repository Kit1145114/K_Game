#include "stdafx.h"
#include "SpriteRender.h"


SpriteRender::SpriteRender()
{
}

SpriteRender::~SpriteRender()
{
}

void SpriteRender::Init(const wchar_t* texFilePath, float w, float h, bool isDraw3D)
{
	m_isDraw3D = isDraw3D;
	m_sprite.Init(texFilePath, w, h);
}

void SpriteRender::Update()
{
	//m_sprite.UpdateWorldMatrix
	//(
	//	m_position,
	//	m_rotation,
	//	m_scale
	//);
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
	Draw();
}

void SpriteRender::Draw()
{
	CMatrix mView;
	CMatrix mProj;
	mView.MakeLookAt(
		{ 0, 0, 1 },
		{ 0, 0, 0 },
		{ 0,1,0 }
	);
	mProj.MakeOrthoProjectionMatrix(1280, 720, 0.1, 100);
	m_sprite.Draw(mView, mProj);
}