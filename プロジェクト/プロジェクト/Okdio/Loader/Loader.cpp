#include "Loader.h"
#include "../etc/WavFmt.h"
#include "../Func/Func.h"

// コンストラクタ
Loader::Loader()
{
}

// デストラクタ
Loader::~Loader()
{
}

// 読み込み
int Loader::Load(const std::string& fileName)
{
	auto hr = wav::Load(fileName, data[fileName].info, data[fileName].wave);
	if (hr != 0)
	{
		Delete(fileName);
		okmonn::DebugStream(fileName + "の読み込み：失敗");
		return -1;
	}

	return 0;
}

// データ削除
void Loader::Delete(const std::string& fileName)
{
	if (data.find(fileName) != data.end())
	{
		data.erase(data.find(fileName));
	}
}

// サウンド情報取得
okmonn::Info Loader::Info(const std::string& fileName)
{
	if (data.find(fileName) != data.end())
	{
		return data[fileName].info;
	}

	return okmonn::Info();
}

// 波形データ
std::weak_ptr<std::vector<unsigned char>> Loader::Wave(const std::string& fileName)
{
	if (data.find(fileName) != data.end())
	{
		return data[fileName].wave;
	}

	return std::weak_ptr<std::vector<unsigned char>>();
}

// 処理データ量取得
size_t Loader::ProcessSize(const std::string& fileName, const unsigned char& offset)
{
	if (data.find(fileName) != data.end())
	{
		okmonn::Info& info = data[fileName].info;
		return size_t((info.sample * info.channel * (info.bit / 8)) / offset);
	}

	return size_t();
}
