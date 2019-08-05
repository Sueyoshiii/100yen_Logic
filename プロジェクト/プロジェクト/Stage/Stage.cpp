#include "Stage.h"
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <fstream>

using namespace boost::property_tree;

// �R���X�g���N�^
Stage::Stage() :
	box(Primitive(PrimitiveType::box)), boxAlpha(1.0f), nextRoomFlag(false)
{
	Init();
}

int Stage::Init()
{
	// �}�b�v�^�C�v
	mapType["map"]    = MapType::Map;
	mapType["object"] = MapType::Object;

	// ���C���[�^�C�v
	layerType["tilelayer"]   = LayerType::Tile;
	layerType["objectlayer"] = LayerType::Object;
	layerType["imagelayer"]  = LayerType::Image;
	layerType["grouplayer"]  = LayerType::Group;

	return 0;
}

// �f�X�g���N�^
Stage::~Stage()
{
}

// �X�e�[�W�f�[�^�ǂݍ���
int Stage::Load(StageData& stage, const std::string& jsonFilePath, const std::string& imgFilePath)
{
	// json�ǂݍ���
	json_parser::read_json(jsonFilePath.c_str(), data);

	// ���C���[���擾
	int size = GetValue<int>(data, "nextlayerid") - 1;
	stage.layers.resize(size);

	// �`�b�v�̃T�C�Y
	stage.divSize = {
		GetValue<int>(data, "tilewidth"),
		GetValue<int>(data, "tileheight")
	};

	// ���C���[���
	auto layers = data.get_child("layers");
	for (auto& layer : stage.layers)
	{
		// ���C���[��
		layer.name = GetValue<std::string>(layers, ".name");

		// �}�X���擾
		layer.massNum = {
			GetValue<int>(layers, ".width"),
			GetValue<int>(layers, ".height")
		};

		// �}�b�v�f�[�^�擾
		auto data = layers.get_child(".data");
		//layer.chip.resize(data.size());
		// �c�E��
		layer.chips.resize(layer.massNum.y);
		for (auto& x : layer.chips)
		{
			x.resize(layer.massNum.x);
		}
		unsigned int index = 0;
		while (!data.empty())
		{
			// �擪
			auto front = data.front();

			// �擪�̒l
			int chipNum = atoi(front.second.data().c_str());

			// �폜
			data.pop_front();

			//auto& chip = layer.chip[index];
			// ���݂̃}�X���擾
			int massPosX = index % layer.massNum.x;
			int massPosY = int(floorf(float(index / layer.massNum.x)));
			auto& chip = layer.chips.at(massPosY).at(massPosX);

			// �`�b�v�ԍ�
			chip.data = chipNum;

			// �`�b�v�Ȃ�
			if (chip.data == 0)
			{
				++index;
				continue;
			}

			// �ǂݍ���
			chip.tex.Load(imgFilePath);

			// �����T�C�Y
			chip.tex.divSize = {
				float(stage.divSize.x),
				float(stage.divSize.y)
			};

			// �摜�̉��̕�����
			chipMax = int(chip.tex.size.x / chip.tex.divSize.x);

			// �G�f�B�^�[����1����n�܂�̂ň���
			--chipNum;

			// �����ʒu
			chip.tex.offsetPos = {
				float(chipNum % chipMax) * chip.tex.divSize.x,
				floorf(float(chipNum / chipMax)) * chip.tex.divSize.y
			};

			// �T�C�Y
			chip.tex.size = Vec2f(32.0f);
			chip.tex.size.y *= 2.0f;
			chipSize = Vec2(chip.tex.size.x, chip.tex.size.y);

			// �`��ʒu
			chip.worldPos = {
				float(massPosX) * chip.tex.size.x,
				float(massPosY) * chip.tex.size.y
			};

			++index;
		}

		// ���C���[�^�C�v
		std::string str = GetValue<std::string>(layers, ".type");
		layer.type = layerType[str];
	}

	// �X�e�[�W�̃T�C�Y
	stage.size = chipSize * stage.layers[0].massNum;

	// �}�b�v�^�C�v
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

// �}�b�v�f�[�^�`��
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

// �J�ڃ{�b�N�X�`��
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

// �ǃ`�F�b�N
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

// ���̃��[�����擾
Stage* Stage::GetNextRoom()
{
	return nullptr;
}

// �{�b�N�X�̃A���t�@�l���擾
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
