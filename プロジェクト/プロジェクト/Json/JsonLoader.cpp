#include "JsonLoader.h"
//#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <map>

//using namespace boost::property_tree;

// コンストラクタ
JsonLoader::JsonLoader()
{
}

// デストラクタ
JsonLoader::~JsonLoader()
{
}

JsonLoader& JsonLoader::Get()
{
	static JsonLoader instance;
	return instance;
}

// 読み込み
int JsonLoader::Load(const std::string& filePath)
{
	//json_parser::read_json(filePath.c_str(), data);

	// ↓テスト中やよー

	//write_json(std::cout, data);

	//for (auto& i : data.get_child("Test.Nakano"))
	//{
	//	std::cout << i.second.data() << std::endl;
	//}

	//int year = data.get_optional<int>("Test.SchoolYear").value();
	//std::cout << year << std::endl;

	//bool flg = data.get_optional<bool>("Test.Kawaii").value();
	//std::cout << flg << std::endl;

	//std::map<std::string, std::string> map;
	//for (auto& m : data.get_child("Test.Girls"))
	//{
	//	map[m.first] = m.second.data();
	//}

	//for (auto itr = map.begin(); itr != map.end(); ++itr)
	//{
	//	std::cout << itr->first << " : " << itr->second << std::endl;
	//}

	// ↓自前やよー

	FILE* file = nullptr;
	if (fopen_s(&file, filePath.c_str(), "rb") != 0)
	{
		return -1;
	}

	unsigned char strNum = 0;

	std::string jsonStr;

	return 0;
}
