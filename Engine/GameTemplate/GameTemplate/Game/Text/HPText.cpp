#include "stdafx.h"
#include "HPText.h"
#include"Chara/Player.h"

HPText::HPText()
{
	m_sprite = g_goMgr.NewGO<SpriteRender>();
	m_sprite->Init(L"Assets/sprite/Midori.dds", 5, 30);
	m_sprite->SetPosition(m_position);
	m_sprite->SetPivot(PIVOT);
	m_scale = { 1.0,1.0f,0.0f };		//HPŒ¸‚Á‚½‚è‘‚¦‚½‚è‚·‚é—p
}

HPText::~HPText()
{
	g_goMgr.DeleteGO(m_sprite);
}

void HPText::Update()
{
	HPUpdate();
}

void HPText::HPUpdate()
{
	m_scale = { m_HP, 1.0f,1.0f };
	m_sprite->SetScale(m_scale);
}