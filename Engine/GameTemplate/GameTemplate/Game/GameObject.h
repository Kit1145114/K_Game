#pragma once
class GameObject
{
public:
	//���N���X�ɂ͉��z�f�X�g���N�^���K�v
	virtual ~GameObject(){};
	/// �X�V�֐��B
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �����_�֐�
	/// </summary>
	virtual void Render();

	void AgoUpdate()
	{
		if (isUpdate) {
			Update();
		}
	}
	void RecestDelete()
	{
		isRecestDelete = true;
	}
	void SetIsNewGO()
	{
		isNewGO = true;
	}
	bool IsRecestDelete()
	{
		return isRecestDelete;
	}
	bool GetIsGO()
	{
		return isNewGO;
	}
	//�A�b�v�f�[�g���邩�ǂ���
	bool GetisUpdate() const
	{
		return isUpdate;
	}
	//�A�b�v�f�[�g�t���O��ݒ肷��
	void SetUpdateFlag(bool flag)
	{
		isUpdate = flag;
	}
private:
	bool isRecestDelete = false;	//�t���O
	bool isNewGO = false;
	bool isUpdate = true;
};

