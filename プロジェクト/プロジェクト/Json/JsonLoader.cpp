#include "JsonLoader.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <iterator>

using namespace boost::property_tree;

// �R���X�g���N�^
JsonLoader::JsonLoader()
{
	Init();
}

// �f�X�g���N�^
JsonLoader::~JsonLoader()
{
}

JsonLoader& JsonLoader::Get()
{
	static JsonLoader instance;
	return instance;
}

// �ǂݍ���
int JsonLoader::Load(const std::string& filePath)
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

// ������
int JsonLoader::Init()
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
