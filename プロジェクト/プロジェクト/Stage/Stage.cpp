#include "Stage.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <iterator>

using namespace boost::property_tree;

const float Stage::ConstParam::GR = 1.9f;
const float Stage::ConstParam::GROUND = 1000.0f;

// �R���X�g���N�^
Stage::Stage()
{
	range = Stage::Rect(Vec2f(0, 0), 640 * 10, 640);
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
int Stage::Load(const std::string& filePath)
{
	// �ǂݍ���
	json_parser::read_json(filePath.c_str(), data);

	// ���C���[���擾
	int size = GetValue<int>(data, "nextlayerid") - 1;
	stage.layers.resize(size);

	auto layers = data.get_child("layers");
	for (auto& layer : stage.layers)
	{
		// ���C���[��
		layer.name = GetValue<std::string>(layers, ".name");

		// �}�b�v�f�[�^�擾
		auto data = layers.get_child(".data");
		while (!data.empty())
		{
			auto front = data.front();
			layer.data.push_back(atoi(front.second.data().c_str()));
			data.pop_front();
		}

		// �}�X���擾
		layer.massNum = {
			GetValue<int>(layers, ".width"),
			GetValue<int>(layers, ".height")
		};

		// ���C���[�^�C�v
		std::string str = GetValue<std::string>(layers, ".type");
		layer.type = layerType[str];
	}

	// �`�b�v�̃T�C�Y
	stage.divSize = {
		GetValue<int>(data, "tilewidth"),
		GetValue<int>(data, "tileheight")
	};

	// �X�e�[�W�̃T�C�Y
	stage.size = stage.divSize * stage.layers[0].massNum;

	// �}�b�v�^�C�v
	std::string str = GetValue<std::string>(data, "type");
	stage.type = mapType[str];

	return 0;
}

void Stage::Draw()
{

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
