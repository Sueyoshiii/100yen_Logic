#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>

// マップタイプ
enum class MapType
{
	Map,
	Object,
};

enum class LayerType
{
	Tile,
	Object,
	Image,
	Group
};

struct LayerData
{
	// マップデータ
	std::vector<int> data;
	// レイヤー名
	std::string name;
	// レイヤータイプ
	LayerType type;
	// マス数
	Vec2 massNum;
};

struct StageData
{
	// ステージ全体のサイズ
	Vec2 size;
	// チップ一つのサイズ
	Vec2 divSize;
	// マップタイプ
	MapType type;
	// レイヤー
	std::vector<LayerData> layers;
};

class JsonLoader
{
public:
	~JsonLoader();
	static JsonLoader& Get();

	// .jsonデータ読み込み
	int Load(const std::string& filePath);

	// 文字列を数値に変換
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{ 
		return tree.get_optional<T>(str.c_str()).value();
	}

private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// 初期化
	int Init();

	// .jsonデータ
	boost::property_tree::ptree data;

	// ステージ
	StageData stage;

	// マップタイプ
	std::unordered_map<std::string, MapType> mapType;

	// レイヤータイプ
	std::unordered_map<std::string, LayerType> layerType;
};