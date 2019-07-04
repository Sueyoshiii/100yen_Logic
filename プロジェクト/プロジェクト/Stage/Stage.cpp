#include "Stage.h"
#include "../Camera/Camera.h"
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <iterator>

using namespace boost::property_tree;

const float Stage::ConstParam::GR = 1.9f;
const float Stage::ConstParam::GROUND = 1200.0f;

// �R���X�g���N�^
Stage::Stage()
{
	range = Stage::Rect(Vec2f(0, 0), 1280 * 10, 640);
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

// �C���X�^���X
Stage& Stage::Get()
{
	static Stage instance;
	return instance;
}

// �X�e�[�W�f�[�^�ǂݍ���
int Stage::Load(std::weak_ptr<Camera> cam, const std::string& jsonFilePath, const std::string& imgFilePath)
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
			int chipMax = int(chip.tex.size.x / chip.tex.divSize.x);

			// �����ʒu
			chip.tex.offsetPos = {
				float(chipNum % chipMax) * chip.tex.divSize.x,
					floorf(float(chipNum / chipMax)) * chip.tex.divSize.y
			};

			// �T�C�Y
			chip.tex.size = Vec2f(64.0f, 64.0f);

			// �`��ʒu
			chip.tex.pos = {
				float(index % layer.massNum.x) * chip.tex.size.x,
				floorf(float(index / layer.massNum.y)) * chip.tex.size.y
			};
			chip.tex.pos.x -= int(cam.lock()->GetPos().x) % int(chip.tex.size.x);

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

// �`��
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

// �͈͎擾
StageRange Stage::GetRange() const
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left() / 10, range.Right());
	return stRange;
}

// �n�ʎ擾
float Stage::GetGround() const
{
	return ConstParam::GROUND;
}

// �d�͎擾
float Stage::GetGravity() const
{
	return ConstParam::GR;
}
