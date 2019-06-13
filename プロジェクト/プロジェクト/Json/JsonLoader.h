#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>

struct StageData
{
	int width;
	int height;
	int eventNum;
};

class JsonLoader
{
public:
	~JsonLoader();
	static JsonLoader& Get();

	// .json�f�[�^�ǂݍ���
	int Load(const std::string& filePath);
private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// .json�f�[�^
	boost::property_tree::ptree data;
};