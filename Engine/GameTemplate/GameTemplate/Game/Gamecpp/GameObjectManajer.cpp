#include "stdafx.h"
#include "GameObjectManajer.h"

//GameObjectManajer�N���X�̃C���X�^���X�B
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
			//���N�G�X�g���󂯂ĂȂ��̂�
			it++;
		}
	}
}