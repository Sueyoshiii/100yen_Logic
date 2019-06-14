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

class JsonLoader
{
public:
	~JsonLoader();
	static JsonLoader& Get();

	// .json�f�[�^�ǂݍ���
	int Load(const std::string& filePath);

	// ������𐔒l�ɕϊ�
	template<typename T>
	T GetValue(const std::string& str) { return T; }
	// int
	template<>
	int GetValue(const std::string& str)
	{
		int value = atoi(str.c_str());
		return value;
	}
	// float
	template<>
	float GetValue(const std::string& str)
	{
		float value = float(atof(str.c_str()));
		return value;
	}
	// bool
	template<>
	bool GetValue(const std::string& str)
	{
		bool value = str == "true" ? true : false;
		return value;
	}

private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// .json�f�[�^
	boost::property_tree::ptree data;
};