#pragma once
#include "MixVoice/MixVoice.h"
#include "Func/Func.h"
#include <memory>

class VoiceCallback;
class Manager;

class __declspec(uuid("00000000-0917-0917-0917-000000000001"))
Okdio : public Voice<Okdio, IXAudio2SourceVoice>
{
	friend Voice<Okdio, IXAudio2SourceVoice>;
	friend VoiceCallback;
	friend Manager;
public:
	// 読み込み
	long Load(const std::string& fileName);

	// ミックスボイスセット
	long SetMixVoice(MixVoice* mix, const size_t& num);

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

	// 終了フラグ取得
	bool IsPlayEnd(void) const;

private:
	// コンストラクタ
	Okdio();
	Okdio(const Okdio&) = delete;
	void operator=(const Okdio&) = delete;
	// デストラクタ
	~Okdio();

	// ソースボイス生成
	long CreateVoice(void);

	// オーディオバッファに追加
	void Submit(void);

	// 終了チェック
	void CheckEnd(void);


	// コールバック
	std::unique_ptr<VoiceCallback>callback;

	// 参照ファイル名
	std::string name;

	// 読み込み位置
	size_t read;

	// ループフラグ
	bool loop;

	// 終了フラグ
	bool end;
};
