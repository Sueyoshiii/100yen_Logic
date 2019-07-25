#include "Stage.h"
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <iterator>

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
		layer.chip.resize(data.size());
		unsigned int index = 0;
		while (!data.empty())
		{
			// �擪
			auto front = data.front();

			// �擪�̒l
			int chipNum = atoi(front.second.data().c_str());

			// �폜
			data.pop_front();

			// �e�N�X�`��
			auto& chip = layer.chip[index];

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

			// �G�f�B�^�[����1����n�܂�̂ň���
			--chipNum;

			// ���̕�����
			chipMax = int(chip.tex.size.x / chip.tex.divSize.x);

			// �����ʒu
			chip.tex.offsetPos = {
				float(chipNum % chipMax) * chip.tex.divSize.x,
				floorf(float(chipNum / chipMax)) * chip.tex.divSize.y
			};

			// �T�C�Y
			chip.tex.size = Vec2f(64.0f);

			// �`��ʒu
			chip.tex.pos = {
				float(index % layer.massNum.x) * chip.tex.size.x,
				floorf(float(index / layer.massNum.x)) * chip.tex.size.y
			};
			chip.worldPos = cam.lock()->Correction(chip.tex.pos);
			chip.worldPos.x -= float(lib.lock()->GetWinSize().x);

			++index;
		}

		// ���C���[�^�C�v
		std::string str = GetValue<std::string>(layers, ".type");
		layer.type = layerType[str];
	}

	// �X�e�[�W�̃T�C�Y
	stage.size = stage.divSize * stage.layers[0].massNum;

	// �}�b�v�^�C�v
	std::string str = GetValue<std::string>(data, "type");
	stage.type = mapType[str];

	return 0;
}

// �}�b�v�f�[�^�`��
void Stage::DrawMapData(std::weak_ptr<Camera> cam)
{
	if (back.layers.size() <= 0)
	{
		return;
	}

	auto camRange = cam.lock()->GetRange();
	for (auto& chip : back.layers[0].chip)
	{
		if (chip.data > 0)
		{
			if (chip.worldPos.x > camRange.GetLeft() && chip.worldPos.x < camRange.GetRight())
			{
				chip.tex.pos = cam.lock()->Correction(chip.worldPos);
				lib.lock()->Draw(chip.tex);
			}
		}
	}
}

// �J�ڃ{�b�N�X�`��
void Stage::DrawBox()
{
	lib.lock()->Draw(box, Vec3f(), boxAlpha);
	boxAlpha = std::min(std::max(boxAlpha, 0.0f), 1.0f);
	boxAlpha += nextRoomFlag ? 0.05f : -0.05f;
}

void Stage::DrawMapDataFront(std::weak_ptr<Camera> cam)
{
	if (front.layers.size() <= 0)
	{
		return;
	}

	auto camRange = cam.lock()->GetRange();
	for (auto& chip : front.layers[0].chip)
	{
		if (chip.data > 0)
		{
			if (chip.worldPos.x > camRange.GetLeft() && chip.worldPos.x < camRange.GetRight())
			{
				chip.tex.pos = cam.lock()->Correction(chip.worldPos);
				lib.lock()->Draw(chip.tex);
			}
		}
	}
}

// �ǃ`�F�b�N
bool Stage::CheckWall(const Vec2f& pos, const Vec2f& size, const bool turnFlag, const Dir& dir)
{
	Vec2f chipSize(64.0f);

	Vec2f tmpPos = pos;
	Vec2f tmpSize = { size.x / 2.0f, size.y };
	if (dir == Dir::Down)
	{
		if (!turnFlag)
		{
			tmpPos = {
				pos.x + tmpSize.x,
				pos.y
			};
		}

		for (auto& chips : back.layers[0].chip)
		{
			if (chips.data == 0)
			{
				continue;
			}

			if (tmpPos.y + tmpSize.y > chips.worldPos.y &&
				tmpPos.x < chips.worldPos.x + chipSize.x &&
				tmpPos.x + tmpSize.x > chips.worldPos.x)
			{
				return true;
			}
		}
	}

	if (dir == Dir::Right)
	{
		tmpPos = {
			tmpPos.x + tmpSize.x,
			tmpPos.y + tmpSize.y / 2.0f
		};

		Vec2f tmpSize2 = {
			tmpSize.x - 1.0f,
			tmpSize.y / 2.0f - 1.0f
		};

		for (auto& chips : back.layers[0].chip)
		{
			if (chips.data == 0)
			{
				continue;
			}

			Vec2f bCenter = chips.worldPos + chipSize / 2.0f;
			if (fabs(tmpPos.x - bCenter.x) < fabs(tmpSize2.x + chipSize.x) / 2.0f &&
				fabs(tmpPos.y - bCenter.y) < fabs(tmpSize2.y + chipSize.y) / 2.0f)
			{
				return true;
			}
		}
	}

	return false;
}

bool Stage::CheckMapChip(const Vec2f& pos)
{
	Vec2f chipSize(64.0f);
	int index = int(pos.x / chipSize.x) + int(pos.y / chipSize.y) * chipMax;
	if (back.layers[0].chip[index].data != 0)
	{
		return true;
	}

	return false;
}

void Stage::CheckMapCol(Vec2f& pos, const Vec2f& size, const Vec2f& vel)
{
	static bool flag = false;
	Vec2f chipSize(64.0f);
	for (auto& chips : back.layers[0].chip)
	{
		if (chips.data == 0)
		{
			continue;
		}

		Vec2f tmpPos = {
			pos.x,
			pos.y
		};

		Vec2f pCenter = tmpPos + size / 2.0f;
		Vec2f bCenter = chips.worldPos + chipSize / 2.0f;

		if (fabs(pCenter.x - bCenter.x) < fabs(size.x + chipSize.x) / 2.0f &&
			fabs(pCenter.y - bCenter.y) < fabs(size.y + chipSize.y) / 2.0f)
		{
			if (vel.x > 0)
			{
				pos.x = chips.worldPos.x - size.x;
			}
			else
			{
				pos.x = chips.worldPos.x + chipSize.x;
			}
			flag = true;
			break;
		}
	}

	if (!flag)
	{
		pos.x += vel.x;
	}
	flag = false;
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

bool Stage::GetNextRoomFlag() const
{
	return nextRoomFlag;
}

void Stage::SetNextRoomFlag(const bool flag)
{
	nextRoomFlag = flag;
}
