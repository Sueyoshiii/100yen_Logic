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

struct StageRange
{
private:
	float top, bottom, left, right;
public:
	StageRange() : top(0.0f), bottom(0.0f), left(0.0f), right(0.0f) {};
	StageRange(const float top, const float bottom, const float left, const float right) :
		top(top), bottom(bottom), left(left), right(right) {};
	float Top()const { return top; };
	float Bottom()const { return bottom; };
	float Left()const { return left; };
	float Right()const { return right; };
};

class Stage
{
	// ステージ
	struct Rect
	{
		Vec2f pos;
		float w, h;
		Rect() : pos(Vec2f()), w(0.0f), h(0.0f) {};
		Rect(const Vec2f& pos, const float w, const float h) :
			pos(pos), w(w), h(h) {};
		float Top()const { return pos.y - h / 2.0f; };
		float Bottom()const { return pos.y + h / 2.0f; };
		float Left()const { return pos.x - w / 2.0f; };
		float Right()const { return pos.x + w / 2.0f; };
	};

	// 定数
	struct ConstParam
	{
		// 重力
		static const float GR;
		// 地面（仮）
		// ステージデータから引っ張ってくるようにする
		static const float GROUND;
	};
public:
	~Stage();
	static Stage& Get();

	// ステージデータ読み込み
	int Load(std::weak_ptr<Camera> cam, const std::string& jsonFilePath, const std::string& imgFilePath);

	// 描画
	void Draw(std::weak_ptr<MyLib> lib);

	// ステージ範囲取得
	StageRange GetRange()const;

	// 地面取得
	float GetGround()const;

	// 重力取得
	float GetGravity()const;
private:
	Stage();
	Stage(const Stage&) = delete;
	void operator=(const Stage&) = delete;

	// 文字列を数値に変換
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{
		return tree.get_optional<T>(str.c_str()).value();
	}

	// 初期化
	int Init();

	//範囲
	Rect range;

	// .jsonデータ
	boost::property_tree::ptree data;

	// ステージ
	StageData stage;

	// マップタイプ
	std::unordered_map<std::string, MapType> mapType;

	// レイヤータイプ
	std::unordered_map<std::string, LayerType> layerType;

	Vec2f pos;
};