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
	// 読み込み
	json_parser::read_json(filePath.c_str(), data);

	// 全文字列を出力
	write_json(std::cout, data);


	for (auto& i : data.get_child("Test.Nakano"))
	{
		std::cout << i.second.data() << std::endl;
	}

	// 整数値取得
	int year = data.get_optional<int>("Test.SchoolYear").value();
	std::cout << year << std::endl;

	// 真偽値取得
	bool flg = data.get_optional<bool>("Test.Kawaii").value();
	std::cout << flg << std::endl;

	std::map<std::string, std::string> map;
	for (auto& m : data.get_child("Test.Girls"))
	{
		map[m.first] = m.second.data();
	}

	for (auto itr = map.begin(); itr != map.end(); ++itr)
	{
		std::cout << itr->first << " : " << itr->second << std::endl;
	}

	// ↓自前やよー

	//std::ifstream ifs(filePath.c_str());
	//if (ifs.fail())
	//{
	//	return -1;
	//}

	// イテレータ用意
	//std::istreambuf_iterator<char> it(ifs);
	//std::istreambuf_iterator<char> last;
	//std::string str(it, last);

	// 1行ずつ取得
	//std::string str;
	//while (std::getline(ifs, str))
	//{

	//	std::cout << str << std::endl;
	//}

	//ifs.close();

	return 0;
}
