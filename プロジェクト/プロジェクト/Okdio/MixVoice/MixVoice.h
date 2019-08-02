#pragma once
#include "../etc/Voice.h"

class Manager;
class Okdio;

class __declspec(uuid("00000000-0917-0917-0917-000000000002"))
MixVoice : public Voice<MixVoice, IXAudio2SubmixVoice>
{
	friend Manager;
	friend Okdio;
public:
	// 初期化
	long Initialize(const unsigned short& sample, const unsigned char& channel);

private:
	// コンストラクタ
	MixVoice();
	MixVoice(const MixVoice&) = delete;
	void operator=(const MixVoice&) = delete;
	// デストラクタ
	~MixVoice();

	// 宛先音声定義取得
	XAUDIO2_SEND_DESCRIPTOR Send(void) const;
};
