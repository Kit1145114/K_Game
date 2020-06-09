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

	Effekseer::Effect* CreateEffekseerEffect(const wchar_t* filePath);

	Effekseer::Handle Play(Effekseer::Effect* effect);

private:
	Effekseer::Manager* m_effekseerManager;
	EffekseerRenderer::Renderer*	m_effekseerRenderer = nullptr;
	Effekseer::Effect* m_attackEffect = nullptr;
	Effekseer::Handle m_playEffectHandle = -1;
};

extern CEffektEngine* g_effektEngine;

