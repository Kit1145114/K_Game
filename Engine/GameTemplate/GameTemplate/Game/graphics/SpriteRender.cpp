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
	//Draw();
	//m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
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
	mProj.MakeOrthoProjectionMatrix(1280.0f, 720.0f, 0.10f, 100.0f);
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
	m_sprite.Draw(mView, mProj);
}

void SpriteRender::Render()
{
	Draw();
}