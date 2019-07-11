#include "Stage.h"
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <iterator>

using namespace boost::property_tree;

// コンストラクタ
Stage::Stage() :
	box(Primitive(PrimitiveType::box)), boxAlpha(1.0f), nextRoomFlag(false)
{
	Init();
}

int Stage::Init()
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

// デストラクタ
Stage::~Stage()
{
}

// ステージデータ読み込み
int Stage::Load(const std::string& jsonFilePath, const std::string& imgFilePath)
{
	// json読み込み
	json_parser::read_json(jsonFilePath.c_str(), data);

	// レイヤー数取得
	int size = GetValue<int>(data, "nextlayerid") - 1;
	stage.layers.resize(size);

	// チップのサイズ
	stage.divSize = {
		GetValue<int>(data, "tilewidth"),
		GetValue<int>(data, "tileheight")
	};

	// レイヤー情報
	auto layers = data.get_child("layers");
	for (auto& layer : stage.layers)
	{
		// レイヤー名
		layer.name = GetValue<std::string>(layers, ".name");

		// マス数取得
		layer.massNum = {
			GetValue<int>(layers, ".width"),
			GetValue<int>(layers, ".height")
		};

		// マップデータ取得
		auto data = layers.get_child(".data");
		layer.chip.resize(data.size());
		unsigned int index = 0;
		while (!data.empty())
		{
			// 先頭
			auto front = data.front();

			// 先頭の値
			int chipNum = atoi(front.second.data().c_str());

			// 削除
			data.pop_front();

			// テクスチャ
			auto& chip = layer.chip[index];

			// チップ番号
			chip.data = chipNum;

			// チップなし
			if (chip.data == 0)
			{
				++index;
				continue;
			}

			// 読み込み
			chip.tex.Load(imgFilePath);

			// 分割サイズ
			chip.tex.divSize = {
				float(stage.divSize.x),
				float(stage.divSize.y)
			};

			// エディター側が1から始まるので引く
			--chipNum;

			// 横の分割数
			int chipMax = int(chip.tex.size.x / chip.tex.divSize.x);

			// 分割位置
			chip.tex.offsetPos = {
				float(chipNum % chipMax) * chip.tex.divSize.x,
				floorf(float(chipNum / chipMax)) * chip.tex.divSize.y
			};

			// サイズ
			chip.tex.size = Vec2f(64.0f);

			// 描画位置
			chip.tex.pos = {
				float(index % layer.massNum.x) * chip.tex.size.x,
				floorf(float(index / layer.massNum.x)) * chip.tex.size.y
			};
			chip.worldPos = cam.lock()->Correction(chip.tex.pos);
			chip.worldPos.x -= float(lib.lock()->GetWinSize().x);

			++index;
		}

		// レイヤータイプ
		std::string str = GetValue<std::string>(layers, ".type");
		layer.type = layerType[str];
	}

	// ステージのサイズ
	stage.size = stage.divSize * stage.layers[0].massNum;

	// マップタイプ
	std::string str = GetValue<std::string>(data, "type");
	stage.type = mapType[str];

	return 0;
}

// マップデータ描画
void Stage::DrawMapData()
{
	if (stage.layers.size() <= 0)
	{
		return;
	}

	for (auto& chip : stage.layers[0].chip)
	{
		if (chip.data > 0)
		{
			chip.tex.pos = cam.lock()->Correction(chip.worldPos);
			lib.lock()->Draw(chip.tex);
		}
	}
}

// 遷移ボックス描画
void Stage::DrawBox()
{
	lib.lock()->Draw(box, Vec3f(), boxAlpha);
	boxAlpha = std::min(std::max(boxAlpha, 0.0f), 1.0f);
	boxAlpha += nextRoomFlag ? 0.05f : -0.05f;
}

// 次のルームを取得
Stage* Stage::GetNextRoom()
{
	return nullptr;
}

// ボックスのアルファ値を取得
float Stage::GetBoxAlpha() const
{
	return boxAlpha;
}

bool Stage::GetNextRoomFlag() const
{
	return nextRoomFlag;
}

void Stage::SetNextRoomFlag(const bool flag)
{
	nextRoomFlag = flag;
}
