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

	virtual void Draw() {}

	virtual void FontRender() {}

	virtual bool Start();

	void GoUpdate()
	{
		if (isUpdate_flag&&isActive_flag&&isStart_flag) {
			Update();
		}
	}

	void GoDraw()
	{
		if (isUpdate_flag && isActive_flag && isStart_flag) {
			Draw();
		}
	}
	//�|�X�g�����_�[
	//virtual void PostRender();

	void PostRenderWarapper()
	{
		if (isUpdate_flag&isActive_flag&&isStart_flag) {
			Render();
		}
	}
	void FontRenderWarapper()
	{
		if (isUpdate_flag&isActive_flag&&isStart_flag) {
			FontRender();
		}
	}
	void RecestDelete()
	{
		isRecestDelete_flag = true;
	}
	void SetIsNewGO()
	{
		isNewGO_flag = true;
	}
	bool IsRecestDelete()
	{
		return isRecestDelete_flag;
	}
	bool GetIsGO()
	{
		return isNewGO_flag;
	}
	//�A�b�v�f�[�g���邩�ǂ���
	bool GetisUpdate() const
	{
		return isUpdate_flag;
	}
	//�A�b�v�f�[�g�t���O��ݒ肷��
	void SetUpdateFlag(bool flag)
	{
		isUpdate_flag = flag;
	}	
	void SetActive(bool flag)
	{
		isActive_flag = flag;
	}
	//�X�^�[�g�֐��̃t���O��ς���B
	void SetIsStart(bool flag)
	{
		isStart_flag = flag;
	}
	bool GetIsStart()
	{
		return isStart_flag;
	}
private:
	bool isRecestDelete_flag = false;	//�t���O
	bool isNewGO_flag = false;
	bool isUpdate_flag = true;
	bool isActive_flag = true;
	bool isStart_flag = false;
};

