#include "JsonLoader.h"

// �R���X�g���N�^
JsonLoader::JsonLoader()
{
}

// �f�X�g���N�^
JsonLoader::~JsonLoader()
{
}

// �ǂݍ���
int JsonLoader::Load(const std::string& filePath)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filePath.c_str(), "rb") != 0)
	{
		return -1;
	}

	return 0;
}
