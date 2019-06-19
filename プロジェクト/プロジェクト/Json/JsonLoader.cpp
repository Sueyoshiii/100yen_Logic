#include "JsonLoader.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <iterator>

using namespace boost::property_tree;

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
	// 読み込み
	json_parser::read_json(filePath.c_str(), data);

	// レイヤー数取得
	int size = GetValue<int>(data, "nextlayerid") - 1;
	stage.layers.resize(size);

	auto layers = data.get_child("layers");
	for (auto& layer : stage.layers)
	{
		// レイヤー名
		layer.name = GetValue<std::string>(layers, ".name");
		
		// マップデータ取得
		auto data = layers.get_child(".data");
		while (!data.empty())
		{
			auto front = data.front();
			layer.data.push_back(atoi(front.second.data().c_str()));
			data.pop_front();
		}
		
		// マス数取得
		layer.massNum = { 
			GetValue<int>(layers, ".width"),
			GetValue<int>(layers, ".height")
		};

		// レイヤータイプ
		std::string str = GetValue<std::string>(layers, ".type");
		layer.type = layerType[str];
	}

	// チップのサイズ
	stage.divSize = {
		GetValue<int>(data, "tilewidth"),
		GetValue<int>(data, "tileheight")
	};

	// ステージのサイズ
	stage.size = stage.divSize * stage.layers[0].massNum;

	// マップタイプ
	std::string str = GetValue<std::string>(data, "type");
	stage.type = mapType[str];

	return 0;
}

// 初期化
int JsonLoader::Init()
{
	// マップタイプ
	mapType["map"]    = MapType::Map;
	mapType["object"] = MapType::Object;

	// レイヤータイプ
	layerType["tilelayer"]   = LayerType::Tile;
	layerType["objectlayer"] = LayerType::Object;
	layerType["imagelayer"]  = LayerType::Image;
	layerType["grouplayer"]  = LayerType::Group;

	return 0;
}
