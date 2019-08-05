#include "Stage.h"
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <fstream>

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
int Stage::Load(StageData& stage, const std::string& jsonFilePath, const std::string& imgFilePath)
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
		//layer.chip.resize(data.size());
		// 縦・横
		layer.chips.resize(layer.massNum.y);
		for (auto& x : layer.chips)
		{
			x.resize(layer.massNum.x);
		}
		unsigned int index = 0;
		while (!data.empty())
		{
			// 先頭
			auto front = data.front();

			// 先頭の値
			int chipNum = atoi(front.second.data().c_str());

			// 削除
			data.pop_front();

			//auto& chip = layer.chip[index];
			// 現在のマスを取得
			int massPosX = index % layer.massNum.x;
			int massPosY = int(floorf(float(index / layer.massNum.x)));
			auto& chip = layer.chips.at(massPosY).at(massPosX);

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

			// 画像の横の分割数
			chipMax = int(chip.tex.size.x / chip.tex.divSize.x);

			// エディター側が1から始まるので引く
			--chipNum;

			// 分割位置
			chip.tex.offsetPos = {
				float(chipNum % chipMax) * chip.tex.divSize.x,
				floorf(float(chipNum / chipMax)) * chip.tex.divSize.y
			};

			// サイズ
			chip.tex.size = Vec2f(32.0f);
			chip.tex.size.y *= 2.0f;
			chipSize = Vec2(chip.tex.size.x, chip.tex.size.y);

			// 描画位置
			chip.worldPos = {
				float(massPosX) * chip.tex.size.x,
				float(massPosY) * chip.tex.size.y
			};

			++index;
		}

		// レイヤータイプ
		std::string str = GetValue<std::string>(layers, ".type");
		layer.type = layerType[str];
	}

	// ステージのサイズ
	stage.size = chipSize * stage.layers[0].massNum;

	// マップタイプ
	std::string str = GetValue<std::string>(data, "type");
	stage.type = mapType[str];

	std::string map;
	for (auto& y : back.layers[0].chips)
	{
		for (auto& x : y)
		{
			std::string str;
			if (x.data < 10)
			{
				str = " " + std::to_string(x.data);
			}
			else
			{
				str = std::to_string(x.data);
			}
			map += str;
		}
		map += "\n";
	}

	//std::ofstream outText("mapchip.txt");
	//outText << map;
	//outText.close();

	return 0;
}

// マップデータ描画
void Stage::DrawMapData(std::weak_ptr<Camera> cam)
{
	if (back.layers.size() <= 0)
	{
		return;
	}

	auto camera = cam.lock();
	for (auto& y : back.layers[0].chips)
	{
		for (auto& x : y)
		{
			if (x.data > 0)
			{
				if (x.worldPos.x > camera->GetRange().GetLeft() && x.worldPos.x < camera->GetRange().GetRight())
				{
					x.tex.pos = camera->Correction(x.worldPos);
					lib.lock()->Draw(x.tex);
				}
			}
		}
	}
}

// 遷移ボックス描画
void Stage::DrawBox()
{
	lib.lock()->Draw(box, Vec3f(), boxAlpha);
	boxAlpha = std::fmin(std::fmax(boxAlpha, 0.0f), 1.0f);
	boxAlpha += nextRoomFlag ? 0.05f : -0.05f;
}

void Stage::DrawMapDataFront(std::weak_ptr<Camera> cam)
{
	if (front.layers.size() <= 0)
	{
		return;
	}

	auto camera = cam.lock();
	std::for_each(front.layers[0].chips.begin(), front.layers[0].chips.end(), [this, camera](std::vector<MapchipData> & x) {
		std::for_each(x.begin(), x.end(), [this, camera](MapchipData & chip) {
			if (chip.data > 0)
			{
				if (chip.worldPos.x > camera->GetRange().GetLeft() && chip.worldPos.x < camera->GetRange().GetRight())
				{
					chip.tex.pos = camera->Correction(chip.worldPos);
					lib.lock()->Draw(chip.tex);
				}
			}
		});
	});
}

// 壁チェック
bool Stage::CheckWall(const Vec2f& pos, const Vec2f& size, const bool turnFlag, const Dir& dir)
{
	return false;
}

bool Stage::CheckMapChip(const Vec2f& pos)
{
	int y = pos.y / chipSize.y;
	int x = pos.x / chipSize.x;

	if (y >= back.layers[0].massNum.y ||
		x >= back.layers[0].massNum.x ||
		y < 0 ||
		x < 0)
	{
		return false;
	}

	int data = back.layers[0].chips.at(y).at(x).data;
	if (data != 0 && data != 53)
	{
  		return true;
	}

	return false;
}

void Stage::CheckMapCol(Vec2f& pos, const Vec2f& size, const Vec2f& vel)
{
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

int Stage::GetMapChip(const Vec2f& pos)
{
	int y = pos.y / chipSize.y;
	int x = pos.x / chipSize.x;

	if (y >= back.layers[0].massNum.y ||
		x >= back.layers[0].massNum.x ||
		y < 0 ||
		x < 0)
	{
		return -1;
	}

	return back.layers[0].chips.at(y).at(x).data;
}

Vec2f Stage::GetStageSize() const
{
	return Vec2f(back.size.x/* * length*/, back.size.y);
}

Vec2 Stage::GetChipSize() const
{
	return chipSize;
}

bool Stage::GetNextRoomFlag() const
{
	return nextRoomFlag;
}

void Stage::SetNextRoomFlag(const bool flag)
{
	nextRoomFlag = flag;
}
