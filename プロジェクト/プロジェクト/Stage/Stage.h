#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>
#include <memory>

class Camera;

// マップタイプ
enum class MapType
{
	Map,
	Object,
};

// レイヤータイプ
enum class LayerType
{
	Tile,
	Object,
	Image,
	Group
};

// マップチップ
struct MapchipData
{
	int data;
	Texture tex;
	Vec2f worldPos;
	MapchipData() : data(0), tex(Texture()){}
	MapchipData(const int data, const Texture& tex) :
		data(data), tex(tex) {}
};

// レイヤーデータ
struct LayerData
{
	// マップデータ
	std::vector<MapchipData> chip;
	// レイヤー名
	std::string name;
	// レイヤータイプ
	LayerType type;
	// マス数
	Vec2 massNum;
};

// ステージデータ
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

class Stage
{
public:
	Stage();
	~Stage();

	// ステージデータ読み込み
	int Load(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam, const std::string& jsonFilePath, const std::string& imgFilePath);

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
protected:
	// .jsonデータ
	boost::property_tree::ptree data;

	// ステージ
	StageData stage;

	// マップタイプ
	std::unordered_map<std::string, MapType> mapType;

	// レイヤータイプ
	std::unordered_map<std::string, LayerType> layerType;
private:
	// 文字列を数値に変換
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{
		return tree.get_optional<T>(str.c_str()).value();
	}

	// 初期化
	int Init();

	Vec2f pos;
};