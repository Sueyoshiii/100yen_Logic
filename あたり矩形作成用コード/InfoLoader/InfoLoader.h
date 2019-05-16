#pragma once
#include "../Typedef.h"
#include <memory>
#include <unordered_map>

class InfoLoader
{
public:
	// デストラクタ
	~InfoLoader();

	// インスタンス変数の取得
	static InfoLoader& Get(void) {
		static InfoLoader instance;
		return instance;
	}

	// 読み込み
	int Load(const std::string& fileName);

	// 書き込み
	int Write(const std::string& fileName, const std::vector<Info>& info);

	// 情報の取得
	std::shared_ptr<std::unordered_map<std::string, Info>> GetInfo(const std::string& fileName) {
		return info[fileName];
	}

private:
	// コンストラクタ
	InfoLoader();
	InfoLoader(const InfoLoader&) = delete;
	void operator=(const InfoLoader&) = delete;


	// 情報
	std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, Info>>>info;
};
