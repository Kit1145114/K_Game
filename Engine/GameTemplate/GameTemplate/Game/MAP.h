#pragma once
#include"GameObjectManajer.h"
class MAP	:	public GameObject
{
public:
	/// <summary>
	/// �C���X�g���N�^
	/// </summary>
	MAP();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MAP();
	void Update();
	void Draw();
private:
	SkinModel Map;	//�}�b�v�̃X�L�����f���B
};

