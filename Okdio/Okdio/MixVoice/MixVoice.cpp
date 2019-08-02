#include "MixVoice.h"
#include "../XAudio2/XAudio2.h"
#include "../Func/Func.h"

// コンストラクタ
MixVoice::MixVoice()
{
}

// デストラクタ
MixVoice::~MixVoice()
{
}

// 初期化
long MixVoice::Initialize(const unsigned short& sample, const unsigned char& channel)
{
	auto hr = XAudio2::Get().Audio()->CreateSubmixVoice(&voice, channel, sample);
	if (FAILED(hr))
	{
		okmonn::DebugStream("サブミックスボイス生成：失敗");
	}

	return hr;
}

// 宛先音声定義取得
XAUDIO2_SEND_DESCRIPTOR MixVoice::Send(void) const
{
	return { XAUDIO2_SEND_USEFILTER, voice };
}
