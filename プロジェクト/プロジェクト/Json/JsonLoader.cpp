#include "JsonLoader.h"
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <map>
#include <fstream>
#include <iterator>

using namespace boost::property_tree;

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
	json_parser::read_json(filePath.c_str(), data);

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

	std::ifstream ifs(filePath.c_str());
	if (ifs.fail())
	{
		return -1;
	}

	// イテレータ用意
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;

	std::string str(it, last);

	ifs.close();

	std::cout << str << std::endl;

	return 0;
}
