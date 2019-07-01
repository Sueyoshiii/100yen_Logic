#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>

class JsonLoader
{
public:
	~JsonLoader();
	static JsonLoader& Get();

	// .jsonデータ読み込み
	int Load(const std::string& filePath);

	// 文字列を数値に変換
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{ 
		return tree.get_optional<T>(str.c_str()).value();
	}

private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// 初期化
	int Init();
};