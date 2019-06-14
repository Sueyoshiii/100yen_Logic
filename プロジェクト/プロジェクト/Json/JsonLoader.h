#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>

struct StageData
{
	int width;
	int height;
	int eventNum;
};

struct Test
{
	Test() {}

	// 子供取得
	std::unordered_map<std::string, std::string> GetChild(const std::string& str)
	{

	}

	// 文字列を数値に変換
	template<typename T>
	T GetValue(const std::string& str) { return T; }
	template<>
	int GetValue(const std::string& str)
	{
		int value = atoi(str.c_str());
		return value;
	}
	template<>
	float GetValue(const std::string& str)
	{
		float value = atof(str.c_str());
		return value;
	}
	template<>
	bool GetValue(const std::string& str)
	{
		bool value = str == "true" ? true : false;
		return value;
	}
};

class JsonLoader
{
public:
	~JsonLoader();
	static JsonLoader& Get();

	// .jsonデータ読み込み
	int Load(const std::string& filePath);

private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// .jsonデータ
	boost::property_tree::ptree data;
};