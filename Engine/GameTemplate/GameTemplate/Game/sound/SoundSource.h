//#pragma once
//#include "sound/WaveFile.h"
//
///// <summary>
///// 音源クラス。
///// </summary>
///// <remarks>
///// 
///// 
///// このクラスのインスタンス化を行うと、そのインスタンスはサウンドエンジンクラスに登録されます。
///// </remarks>
//class CSoundSource {
//public:
//	/// <summary>
//	/// コンストラクタ。
//	/// </summary>
//	CSoundSource();
//	/// <summary>
//	/// デストラクタ。
//	/// </summary>
//	~CSoundSource();
//	/// <summary>
//	/// 初期化(オンメモリ再生)
//	/// </summary>
//	/// <param name="filePath">
//	/// ファイルパス。対応しているファイルフォーマット(*.wave)
//	/// </param>
//	void Init(wchar_t* filePath);
//	/// <summary>
//	/// 初期化(ストリーミング再生)
//	/// </summary>
//	/// <remarks>
//	/// リングバッファにストリーミング読み込みを行って、再生を行っていきます。</br>
//	/// 一度に読み込まれるデータの最大サイズはbufferingSizeです。< / br>
//	/// 読み込まれたデータはリングバッファにコピーされていきます。< / br>
//	/// </remarks>
//	/// <param name="filePath">ファイルパス。対応しているファイルフォーマット(*.wave)</param>
//	/// <param name="ringBufferSize">リングバッファのサイズ。(bufferSizeの倍数になっていると無駄なく活用できます。)</param>
//	/// <param name="bufferingSize">ストリーミングの最大バッファリングサイズ。</param>
//	void InitStreaming(wchar_t* filePath, unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);
//	/// <summary>
//	/// 解放
//	/// </summary>
//	/// <remarks>
//	/// デストラクタから自動的に呼ばれます。明示的に開放を行いたい場合に使用してください。
//	/// </remarks>
//	void Release();
//
//	/*!
//	* @brief	再生。
//	*@param[in]	isLoop		ループ再生フラグ。
//	* このフラグがfalseの場合はワンショット再生となります。
//	* ワンショット再生の場合は、再生が完了すると自動的にインスタンスが破棄されます。
//	* ワンショット再生のCSoundSourceのインスタンスをメンバ変数などで保持していると、破棄された
//	* インスタンスにアクセスしてしまう可能性があるので、保持しないことをお勧めします。
//	* 再生の完了を検出したい場合などはイベントリスナーを使用してください。
//	*/
//	/// <summary>
//	/// 再生。
//	/// </summary>
//	/// <param name="isLoop">
//	/// ループ再生フラグ。
//	/// このフラグがfalseの場合はワンショット再生となります。
//	/// </param>
//	void Play(bool isLoop);
//
//	/// <summary>
//	/// 停止
//	/// </summary>
//	void Stop()
//	{
//		m_sourceVoice->Stop();
//		m_isPlaying = false;
//	}
//	/// <summary>
//	/// 一時停止。
//	/// </summary>
//	void Pause()
//	{
//		m_sourceVoice->Stop(/*XAUDIO2_PLAY_TAILS*/);
//	}
//	/// <summary>
//	/// 再生中か判定
//	/// </summary>
//	/// <returns>trueが返ってきたら再生中</returns>
//	bool IsPlaying() const
//	{
//		return m_isPlaying;
//	}
//	/// <summary>
//	/// ボリュームを設定。
//	/// </summary>
//	/// <param name="vol">ボリューム</param>
//	void SetVolume(float vol)
//	{
//		m_sourceVoice->SetVolume(vol);
//	}
//
//	/// <summary>
//	/// ボリュームを取得。
//	/// </summary>
//	/// <returns>ボリューム</returns>
//	float GetVolume() const
//	{
//		float vol;
//		m_sourceVoice->GetVolume(&vol);
//		return vol;
//	}
//	/// <summary>
//	/// ループフラグを取得。
//	/// </summary>
//	/// <returns></returns>
//	bool GetLoopFlag() const
//	{
//		return m_isLoop;
//	}
//	/// <summary>
//	/// ボイスの周波数調整比
//	/// </summary>
//	/// <remarks>
//	/// 詳細はIXAudio2SourceVoiceのSetFrequencyRatioを参照してください。
//	/// </remarks>
//	/// <param name="ratio">周波数比</param>
//	void SetFrequencyRatio(float ratio)
//	{
//		if (m_sourceVoice != nullptr) {
//			m_sourceVoice->SetFrequencyRatio(ratio);
//		}
//	}
//	IXAudio2SourceVoice* GetXAudio2SourceVoice()
//	{
//		return m_sourceVoice;
//	}
//	int GetNumInputChannel()const
//	{
//		return m_waveFile->GetFormat()->nChannels;
//	}
//	FLOAT32* GetEmitterAzimuths()
//	{
//		return m_emitterAzimuths;
//	}
//	FLOAT32* GetMatrixCoefficients()
//	{
//		return m_matrixCoefficients;
//	}
//
//	/// <summary>
//	/// 更新
//	/// </summary>
//	/// <remarks>
//	/// サウンドエンジンから呼ばれるので、ユーザーは使用しないでください。
//	/// </remarks>
//	void Update();
//private:
//	void InitCommon();
//	//ストリーミング再生中の更新処理。
//	void UpdateStreaming();
//	//オンメモリ再生中の更新処理。
//	void UpdateOnMemory();
//	void Play(char* buff, unsigned int bufferSize);
//	void StartStreamingBuffring();
//
//private:
//	enum EnStreamingStatus {
//		enStreamingBuffering,	//バッファリング中。
//		enStreamingQueueing,	//キューイング中。
//	};
//	std::shared_ptr<CWaveFile>	m_waveFile;					//!<波形データ。
//	IXAudio2SourceVoice*		m_sourceVoice = nullptr;	//!<ソースボイス。
//	bool						m_isLoop = false;			//!<ループフラグ。
//	bool						m_isPlaying = false;		//!<再生中フラグ。
//	bool					m_isStreaming = false;		//!<ストリーミング再生？
//	unsigned int			m_streamingBufferSize = 0;	//!<ストリーミング用のバッファリングサイズ。
//	unsigned int			m_currentBufferingSize = 0;	//!<現在のバッファリングのサイズ。
//	unsigned int			m_readStartPos = 0;			//!<読み込み開始位置。
//	unsigned int			m_ringBufferSize = 0;		//!<リングバッファのサイズ。
//	EnStreamingStatus		m_streamingState = enStreamingBuffering;	//!<ストリーミングステータス。
//	FLOAT32 m_emitterAzimuths[INPUTCHANNELS];
//	FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
//	bool m_isAvailable = false;			//!<インスタンスが利用可能？
//};

