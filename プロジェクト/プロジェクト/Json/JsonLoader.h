#pragma once
#include <string>
//#include <boost/property_tree/ptree.hpp>
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
	//boost::property_tree::ptree data;
	std::unordered_map<std::string, 
		std::unordered_map<std::string, std::string>> data;
};