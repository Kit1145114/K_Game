#pragma once
class CEffektEngine
{
public:
	CEffektEngine();
	~CEffektEngine();

	bool GetEffektManeger()
	{
		return  m_effekseerManager;
	}
	//エフェクトの初期化。
	void InitEffekseer();
	//カメラ行列
	void CameraEffekseer();
	//エフェクトのドロー
	void Draw();
	//アップデート
	void Update();


	void SetPosition(Effekseer::Handle effect ,CVector3 pos)
	{
		m_effekseerManager->SetLocation(effect, pos.x , pos.y , pos.z);
	}

	void SetScale(Effekseer::Handle effect, CVector3 scale)
	{
		m_effekseerManager->SetScale(effect, scale.x, scale.y, scale.z);
	}

	void SetRotation(Effekseer::Handle effect, CQuaternion Rot)
	{
		m_effekseerManager->SetRotation(effect, Rot.x, Rot.y, Rot.z);
	}
	void Stop(Effekseer::Handle handle)
	{
		m_effekseerManager->StopEffect(handle);
	}
	//void AddPosition(Effekseer::Handle handle, const CVector3& location)
	//{
	//	m_effekseerManager->AddLocation(handle,location.x , location.y, location.z);
	//}
	Effekseer::Effect* CreateEffekseerEffect(const wchar_t* filePath);
	Effekseer::Handle Play(Effekseer::Effect* effect);

private:
	Effekseer::Manager* m_effekseerManager;
	EffekseerRenderer::Renderer*	m_effekseerRenderer = nullptr;
	Effekseer::Effect* m_attackEffect = nullptr;
	Effekseer::Handle m_playEffectHandle = -1;
};

extern CEffektEngine* g_effektEngine;

