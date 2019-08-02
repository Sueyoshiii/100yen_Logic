#pragma once
#define XAUDIO2_HELPER_FUNCTIONS
#include <vector>
#include <unknwn.h>
#include <xaudio2.h>

template <typename CLASS, typename TYPE>
class Voice :
	public IUnknown
{
public:
	// 音量調節
	bool SetVolume(const float& rate)
	{
		if (voice != nullptr)
		{
			if (!(rate >= 0.0f && rate <= 1.0f))
			{
				return false;
			}

			if (FAILED(voice->SetVolume(rate)))
			{
				return false;
			}
			
			return true;
		}

		return false;
	}

	// チャンネルごとの音量調節
	bool SetLocalVolume(const float& angle)
	{
		if (voice != nullptr)
		{
			//出力チャンネル数を求める
			XAUDIO2_VOICE_DETAILS detail{};
			XAudio2::Get().Mastering()->GetVoiceDetails(&detail);

			//適応データ
			std::vector<float>volume(detail.InputChannels, 0.0f);

			//現在のチャンネル数を求める
			voice->GetVoiceDetails(&detail);
			for (unsigned __int32 ch = 0; ch < detail.InputChannels; ++ch)
			{
				voice->GetChannelVolumes(ch, &volume[ch]);
			}

			float rad = ((angle + 90.0f) / 2) * (3.14159265f / 180.0f);
			for (size_t i = 0; i < volume.size(); i += detail.InputChannels)
			{
				volume[i]     *= std::cos(rad);
				volume[i + 1] *= std::sin(rad);
			}

			//適応
			if (FAILED(voice->SetOutputMatrix(nullptr, detail.InputChannels, UINT32(volume.size()), volume.data())))
			{
				return false;
			}
			
			return true;
		}

		return false;
	}

	// ローパスフィルタ処理
	long LowPassFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::LowPassFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// ハイパスフィルタ処理
	long HighPassFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::HighPassFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// バンドパスフィルタ処理
	long BandPassFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::BandPassFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// ノッチフィルタ処理
	long NotchFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::NotchFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// 参照カウント加算
	unsigned long __stdcall AddRef(void)
	{
		return InterlockedIncrement(&ref);
	}

	// 参照カウント減少
	unsigned long __stdcall Release(void)
	{
		unsigned long hr = InterlockedDecrement(&ref);
		if (hr == 0)
		{
			delete this;
		}

		return hr;
	}

	// サポートチェック
	long __stdcall QueryInterface(const GUID& id, void** obj) 
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}

		*obj = nullptr;
		if (id == IID_IUnknown || id == __uuidof(CLASS))
		{
			*obj = (void*)this;
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

protected:
	// コンストラクタ
	Voice() : voice(nullptr), ref(1) {}
	// デストラクタ
	virtual ~Voice() {}


	// ボイス
	TYPE* voice;

private:
	// 参照カウント
	unsigned long ref;
};
