#include "Stage.h"
#include "../Camera/Camera.h"
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <iterator>

using namespace boost::property_tree;

const float Stage::ConstParam::GR = 1.9f;
const float Stage::ConstParam::GROUND = 1200.0f;

// コンストラクタ
Stage::Stage()
{
	range = Stage::Rect(Vec2f(0, 0), 1280 * 10, 640);
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

// インスタンス
Stage& Stage::Get()
{
	static Stage instance;
	return instance;
}

// ステージデータ読み込み
int Stage::Load(std::weak_ptr<Camera> cam, const std::string& jsonFilePath, const std::string& imgFilePath)
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
			chip.tex.size = Vec2f(64.0f, 64.0f);

			// 描画位置
			chip.tex.pos = {
				float(index % layer.massNum.x) * chip.tex.size.x,
				floorf(float(index / layer.massNum.y)) * chip.tex.size.y
			};
			chip.tex.pos.x -= int(cam.lock()->GetPos().x) % int(chip.tex.size.x);

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

// 描画
void Stage::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto& chip : stage.layers[0].chip)
	{
		if (chip.data > 0)
		{
			lib.lock()->Draw(chip.tex);
		}
	}
}

// 範囲取得
StageRange Stage::GetRange() const
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left() / 10, range.Right());
	return stRange;
}

// 地面取得
float Stage::GetGround() const
{
	return ConstParam::GROUND;
}

// 重力取得
float Stage::GetGravity() const
{
	return ConstParam::GR;
}
