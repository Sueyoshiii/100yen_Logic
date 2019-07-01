#include "JsonLoader.h"
#include <iostream>
#include <iterator>
#include <fstream>

// コンストラクタ
JsonLoader::JsonLoader()
{
	Init();
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
	std::ifstream ifs("data/stage/test2.json");
	//std::ifstream ifs(filePath.c_str());

	if (ifs.fail())
	{
		
		return -1;
	}

	// 1行ずつ取得していく
	std::string str;
	while (std::getline(ifs, str))
	{
		// 空白の削除
		for (size_t begin = str.find_first_of(" "); begin != std::string::npos; begin = str.find_first_of(" "))
		{
			str.erase(begin, 1);
		}

		size_t index = str.find("\"");
		size_t begin = ++index;
		index = str.find("\"", index);
		std::string tmp = str.substr(begin, index - begin);

		// : ←の次から見る
		index += 2;
		begin = index;

		std::string ch = str.substr(index, 1);
		if (ch == "[")
		{
			// 配列データ
			std::unordered_map<std::string, std::vector<std::string>> map;
			while (index < str.size())
			{
				index = str.find(",", index);
				if (index >= str.size())
				{
					index = str.size() - 1;
				}
				map[tmp].push_back(str.substr(begin, index - begin));
				begin = ++index;
			}
		}
		else if (ch == "\"")
		{
			// 文字列データ
			begin = ++index;
			index = str.find("\"", begin);
			tmp = str.substr(begin, index - begin);
		}
		else
		{
			// その他データ（int, float, bool）
			index = str.size() - 1;
			if (str.substr(index, 1) != ",")
			{
				--index;
			}
			tmp = str.substr(begin, index - begin);
		}
	}

	return 0;
}

// 初期化
int JsonLoader::Init()
{
	return 0;
}
