#pragma once
#include "../etc/Singleton.h"

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2 : 
	public Singleton<XAudio2>
{
	friend Singleton<XAudio2>;
public:
	// 開始
	long EnginStart(void);

	// 終了
	void EnginEnd(void);

	// オーディオ取得
	IXAudio2* Audio(void) const;

private:
	// コンストラクタ
	XAudio2();
	// デストラクタ
	~XAudio2();


	// オーディオ生成
	long CreateAudio(void);

	// マスタリングボイス生成
	long CreateMastering(void);


	// オーディオ
	IXAudio2* audio;

	// マスタリングボイス
	IXAudio2MasteringVoice* mastering;
};
