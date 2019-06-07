#include "JsonLoader.h"

// コンストラクタ
JsonLoader::JsonLoader()
{
}

// デストラクタ
JsonLoader::~JsonLoader()
{
}

// 読み込み
int JsonLoader::Load(const std::string& filePath)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filePath.c_str(), "rb") != 0)
	{
		return -1;
	}

	return 0;
}
