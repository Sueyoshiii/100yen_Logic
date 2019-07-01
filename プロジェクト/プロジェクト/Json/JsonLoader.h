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

	// .json�f�[�^�ǂݍ���
	int Load(const std::string& filePath);

	// ������𐔒l�ɕϊ�
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{ 
		return tree.get_optional<T>(str.c_str()).value();
	}

private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// ������
	int Init();
};