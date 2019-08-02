#include "XAudio2.h"
#include "../Func/Func.h"
#include <xaudio2.h>

// コンストラクタ
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
}

// デストラクタ
XAudio2::~XAudio2()
{
	EnginEnd();
}

// オーディオ生成
long XAudio2::CreateAudio(void)
{
	return XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
}

// マスタリングボイス生成
long XAudio2::CreateMastering(void)
{
	if (audio != nullptr)
	{
		return audio->CreateMasteringVoice(&mastering, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, nullptr, nullptr);
	}

	return E_POINTER;
}

// 開始
long XAudio2::EnginStart(void)
{
	auto hr = CreateAudio();
	if (FAILED(hr))
	{
		okmonn::DebugStream("XAudio2の生成：失敗");
		return hr;
	}
	hr = CreateMastering();
	if (FAILED(hr))
	{
		okmonn::DebugStream("マスタリングボイスの生成：失敗");
		return hr;
	}

#ifdef _DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debug{};
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	audio->SetDebugConfiguration(&debug);
#endif
	
	return hr;
}

// 終了
void XAudio2::EnginEnd(void)
{
	if (mastering != nullptr)
	{
		mastering->DestroyVoice();
		mastering = nullptr;
	}
	if (audio != nullptr)
	{
		audio->Release();
		audio = nullptr;
	}
}

// オーディオ取得
IXAudio2* XAudio2::Audio(void) const
{
	return audio;
}
