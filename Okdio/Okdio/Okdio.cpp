#include "Okdio.h"
#include "XAudio2/XAudio2.h"
#include "Loader/Loader.h"
#include "XAudio2/VoiceCallback.h"
#include <ks.h>
#include <ksmedia.h>

#pragma comment(lib, "xaudio2.lib")

// スピーカ一覧
const unsigned long spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

// コンストラクタ
Okdio::Okdio() : 
	read(0), loop(false), end(true)
{
	callback = std::make_unique<VoiceCallback>(this);
}

// デストラクタ
Okdio::~Okdio()
{
	if (voice != nullptr)
	{
		Stop();
		voice->DestroyVoice();
		voice = nullptr;
	}
}

// ソースボイス生成
long Okdio::CreateVoice(void)
{
	okmonn::Info info = Loader::Get().Info(name);

	WAVEFORMATEXTENSIBLE fmt{};
	fmt.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	fmt.Format.nSamplesPerSec  = info.sample;
	fmt.Format.wBitsPerSample  = info.bit;
	fmt.Format.nChannels       = info.channel;
	fmt.Format.nBlockAlign     = fmt.Format.nChannels * fmt.Format.wBitsPerSample / 8;
	fmt.Format.nAvgBytesPerSec = fmt.Format.nSamplesPerSec * fmt.Format.nBlockAlign;
	fmt.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	fmt.dwChannelMask               = spk[fmt.Format.nChannels - 1];
	fmt.Samples.wValidBitsPerSample = fmt.Format.wBitsPerSample;
	fmt.SubFormat                   = KSDATAFORMAT_SUBTYPE_PCM;

	return XAudio2::Get().Audio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)& fmt, XAUDIO2_VOICE_USEFILTER, XAUDIO2_DEFAULT_FREQ_RATIO, &(*callback));
}

// 読み込み
long Okdio::Load(const std::string& fileName)
{
	auto hr = Loader::Get().Load(fileName);
	if (FAILED(hr))
	{
		return hr;
	}

	name = fileName;
	hr = CreateVoice();
	if (FAILED(hr))
	{
		okmonn::DebugStream("ソースボイス生成：失敗");
	}

	return hr;
}

// ミックスボイスセット
long Okdio::SetMixVoice(MixVoice* mix, const size_t& num)
{
	if (mix != nullptr)
	{
		std::vector<XAUDIO2_SEND_DESCRIPTOR>desc(num);
		for (size_t i = 0; i < num; ++i)
		{
			desc[i] = mix[i].Send();
		}

		XAUDIO2_VOICE_SENDS send{ unsigned __int32(num), desc.data() };
		auto hr = voice->SetOutputVoices(&send);
		if (FAILED(hr))
		{
			okmonn::DebugStream("送信先ミックスボイスのセット：失敗");
		}

		return hr;
	}

	return E_INVALIDARG;
}

// 再生
long Okdio::Play(const bool& loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
		okmonn::DebugStream("再生開始：失敗");
		return hr;
	}

	this->loop = loop; 
	end        = false;

	return hr;
}

// 停止
long Okdio::Stop(void)
{
	auto hr = voice->Stop(XAUDIO2_PLAY_TAILS);
	if (FAILED(hr))
	{
		okmonn::DebugStream("再生停止：失敗");
	}

	return hr;
}

// オーディオバッファに追加
void Okdio::Submit(void)
{
	XAUDIO2_VOICE_STATE st{};
	voice->GetState(&st);
	if (st.BuffersQueued != 0)
	{
		return;
	}

	auto wave = Loader::Get().Wave(name);
	size_t size = read + Loader::Get().ProcessSize(name) > wave.lock()->size()
		? (read + Loader::Get().ProcessSize(name)) - wave.lock()->size() : Loader::Get().ProcessSize(name);

	XAUDIO2_BUFFER buf{};
	buf.AudioBytes = unsigned __int32(sizeof(wave.lock()->at(0)) * size);
	buf.pAudioData = (unsigned char*)&wave.lock()->at(read);
	auto hr = voice->SubmitSourceBuffer(&buf);
	if (FAILED(hr))
	{
		okmonn::DebugStream("オーディオバッファに追加：失敗");
		return;
	}

	read += size;
}

// 終了チェック
void Okdio::CheckEnd(void)
{
	if (read >= Loader::Get().Wave(name).lock()->size())
	{
		if (loop == false)
		{
			Stop();
			end = true;
		}

		read = 0;
	}
}

// 終了フラグ取得
bool Okdio::IsPlayEnd(void) const
{
	return end;
}
