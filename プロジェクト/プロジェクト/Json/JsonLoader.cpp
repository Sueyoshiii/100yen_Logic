#include "JsonLoader.h"
//#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <map>

//using namespace boost::property_tree;

// �R���X�g���N�^
JsonLoader::JsonLoader()
{
}

// �f�X�g���N�^
JsonLoader::~JsonLoader()
{
}

JsonLoader& JsonLoader::Get()
{
	static JsonLoader instance;
	return instance;
}

// �ǂݍ���
int JsonLoader::Load(const std::string& filePath)
{
	//json_parser::read_json(filePath.c_str(), data);

	// ���e�X�g�����[

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

	// �����O���[

	FILE* file = nullptr;
	if (fopen_s(&file, filePath.c_str(), "rb") != 0)
	{
		return -1;
	}

	unsigned char strNum = 0;

	std::string jsonStr;

	return 0;
}
