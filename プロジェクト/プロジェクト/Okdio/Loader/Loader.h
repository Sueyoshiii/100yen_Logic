#pragma once
#include "../etc/Singleton.h"
#include "../etc/Info.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Loader :
	public Singleton<Loader>
{
	friend Singleton<Loader>;

	struct Data {
		okmonn::Info info;
		std::shared_ptr<std::vector<unsigned char>>wave;
	};

public:
	// 読み込み
	int Load(const std::string& fileName);

	// データ削除
	void Delete(const std::string& fileName);
	
	// サウンド情報取得
	okmonn::Info Info(const std::string& fileName);

	// 波形データ
	std::weak_ptr<std::vector<unsigned char>>Wave(const std::string& fileName);

	// 処理データ量取得
	size_t ProcessSize(const std::string& fileName, const unsigned char& offset = 100);

private:
	// コンストラクタ
	Loader();
	// デストラクタ
	~Loader();


	// データ
	std::unordered_map<std::string, Data>data;
};
