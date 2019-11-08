#pragma once
#include<vector>
#include"GameObject.h"

class GameObjectManajer 
{
public:
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
	/// <summary>
	/// �Q�[���I�u�W�N�g��ǉ�
	/// </summary>
	template<class T>
	T* NewAGO()
	{
		T* newObj = new T();
		m_goList.push_back(newObj);
		newObj->SetIsNewGO();
		return newObj;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���烊�X�g���폜����B
	/// </summary>
	/// <param name="go"></param>�폜����Q�[���I�u�W�F�N�g
	void QutavaleyaAGO(GameObject* go) {
			if (go->GetIsGO() == true) {
				//���������B
				//���X�g����폜����B
				//m_goList.erase();
				////�C���X�^���X���̂��폜�B
				//delete go;/
				go->RecestDelete();
				//�폜�ł����̂ŏI���B
				return;
		}
	}
	void FindGO(GameObject*go) {
		if (go->GetIsGO() == true)
		{
			return ;
		}
	}
private:
	//�ϒ��z��
	std::vector<GameObject*>m_goList;			//�Q�[���I�u�W�F�N�g�̃��X�g
	std::vector<GameObject*>m_delete_goList;	//�f���[�g���܂��B
};

extern GameObjectManajer g_goMgr;
