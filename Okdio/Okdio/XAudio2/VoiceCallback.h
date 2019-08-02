#pragma once
#include "../Okdio.h"

class VoiceCallback :
	public IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	VoiceCallback(Okdio* okdio) : okdio(okdio) {}
	// デストラクタ
	~VoiceCallback() {}

	// データ読み込み前に呼び出し
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired)
	{
		okdio->Submit();
	}

	// 新しいバッファの処理開始時に呼び出し
	void __stdcall OnBufferStart(void* pBufferContext) {
		int i = 0;
	}

	// バッファの処理終了時に呼び出し
	void __stdcall OnBufferEnd(void* pBufferContext) {
		okdio->CheckEnd();
	}

	// 音声の処理パス終了時に呼び出し
	void __stdcall OnVoiceProcessingPassEnd(void) {
	}

	// 連続したストリーム再生終了時に呼び出し
	void __stdcall OnStreamEnd(void) {
	}

	// ループ終了位置到達時に呼び出し
	void __stdcall OnLoopEnd(void* pBufferContext) {
	}

	// エラー発生時に呼び出し
	void __stdcall OnVoiceError(void* pBufferContext, long Error) {
	}

private:
	// サウンドライブラリ
	Okdio* okdio;
};
