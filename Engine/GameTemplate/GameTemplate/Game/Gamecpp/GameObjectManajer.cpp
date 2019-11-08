#include "stdafx.h"
#include "GameObjectManajer.h"

//GameObjectManajerクラスのインスタンス。
GameObjectManajer g_goMgr;

void GameObjectManajer::Update()
{
	for (auto go : m_goList) {
		go->AgoUpdate();
	}
	for (auto it = m_goList.begin(); it != m_goList.end();)
	{
		if ((*it)->IsRecestDelete())
		{
			delete* it;
			it = m_goList.erase(it);
		}
		else {
			//リクエストを受けてないので
			it++;
		}
	}
}