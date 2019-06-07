#pragma once
#include <string>
#include <unordered_map>

class JsonLoader
{
public:
	~JsonLoader();

	// .json�f�[�^�ǂݍ���
	int Load(const std::string& filePath);
private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// .json�f�[�^
};