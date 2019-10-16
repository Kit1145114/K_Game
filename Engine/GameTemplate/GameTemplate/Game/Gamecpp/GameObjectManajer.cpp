#include "stdafx.h"
#include "GameObjectManajer.h"

//GameObjectManajerクラスのインスタンス。
GameObjectManajer g_goMgr;

void GameObjectManajer::Update()
{
	for (auto go : m_goList) {
		go->AgoUpdate();
	}
}