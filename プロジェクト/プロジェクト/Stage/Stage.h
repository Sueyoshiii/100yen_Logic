#pragma once
#include "../Camera/Camera.h"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>
#include <memory>

enum class Dir
{
	Up,
	Down,
	Left,
	Right
};

// チップの種類
enum class ChipType
{
	None,
	Wall
};

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
	//std::vector<MapchipData> chip;
	std::vector<std::vector<MapchipData>> chips;
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
	virtual ~Stage();

	// ステージデータ読み込み
	int Load(StageData& stage, const std::string& jsonFilePath, const std::string& imgFilePath);

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	virtual void DrawFront() = 0;

	// 遷移ボックス描画
	virtual void DrawBox();

	bool CheckWall(const Vec2f& pos, const Vec2f& size, const bool turnFlag, const Dir& dir);

	bool CheckMapChip(const Vec2f& pos);

	void CheckMapCol(Vec2f& pos, const Vec2f& size, const Vec2f& vel);

	// 次のルームを取得
	virtual Stage* GetNextRoom();

	// ボックスのアルファ値を取得
	float GetBoxAlpha()const;

	// ステージサイズ取得
	Vec2f GetStageSize()const;

	Vec2 GetChipSize()const;

	// 部屋移動フラグを取得
	bool GetNextRoomFlag()const;

	// 部屋移動フラグを設定
	void SetNextRoomFlag(const bool flag);
protected:
	// マップデータ描画
	void DrawMapData(std::weak_ptr<Camera> cam);
	void DrawMapDataFront(std::weak_ptr<Camera> cam);

	// .jsonデータ
	boost::property_tree::ptree data;

	// ステージ
	StageData back;
	// 手前
	StageData front;

	// マップタイプ
	std::unordered_map<std::string, MapType> mapType;

	// レイヤータイプ
	std::unordered_map<std::string, LayerType> layerType;

	std::weak_ptr<MyLib> lib;

	std::weak_ptr<Camera> cam;

	// 遷移用ボックス
	Primitive box;

	// 遷移用ボックスのアルファ値
	float boxAlpha;

	// 次のルームに行けるかの確認フラグ
	bool nextRoomFlag;

	int length;

	Vec2 chipSize;
private:
	// 文字列を数値に変換
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{
		return tree.get_optional<T>(str.c_str()).value();
	}

	// 初期化
	int Init();

	int chipMax;
};