#pragma once
#include "../Typedef.h"
#include <string>
#include <memory>
#include <unordered_map>

class Info
{
public:
	~Info();
	static Info& Get();

	// .infoデータ読み込み
	int Load(const std::string& filePath);

	// .infoデータ取得
	std::shared_ptr<std::unordered_map<std::string, Data>> GetData(const std::string& filePath);
private:
	Info();
	Info(const Info&) = delete;
	void operator=(const Info&) = delete;

	// .infoデータ
	std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, Data>>> data;
};