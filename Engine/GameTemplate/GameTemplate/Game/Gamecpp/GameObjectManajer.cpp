#include "stdafx.h"
#include "GameObjectManajer.h"

//GameObjectManajer�N���X�̃C���X�^���X�B
GameObjectManajer g_goMgr;

void GameObjectManajer::Update()
{
	for (auto go : m_goList) {
		go->AgoUpdate();
	}
}