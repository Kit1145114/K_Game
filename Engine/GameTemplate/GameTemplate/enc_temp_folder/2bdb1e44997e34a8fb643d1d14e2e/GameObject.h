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

	bool Start() { return true; };
	void AgoUpdate()
	{
		if (isUpdate&isActive) {
			Update();
		}
	}
	//�|�X�g�����_�[
	//virtual void PostRender();

	void PostRenderWarapper()
	{
		if (isUpdate&isActive) {
			Render();
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
	void SetActive(bool flag)
	{
		isActive = flag;
	}
	//�X�^�[�g�֐��̃t���O��ς���B
	void SetIsStart(bool flag)
	{
		isStart = flag;
	}
	bool GetIsStart()
	{
		return isStart;
	}
private:
	bool isRecestDelete = false;	//�t���O
	bool isNewGO = false;
	bool isUpdate = true;
	bool isActive = true;
	bool isStart = false;
};

