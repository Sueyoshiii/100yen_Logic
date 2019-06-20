#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>

// �}�b�v�^�C�v
enum class MapType
{
	Map,
	Object,
};

enum class LayerType
{
	Tile,
	Object,
	Image,
	Group
};

struct LayerData
{
	// �}�b�v�f�[�^
	std::vector<int> data;
	// ���C���[��
	std::string name;
	// ���C���[�^�C�v
	LayerType type;
	// �}�X��
	Vec2 massNum;
};

struct StageData
{
	// �X�e�[�W�S�̂̃T�C�Y
	Vec2 size;
	// �`�b�v��̃T�C�Y
	Vec2 divSize;
	// �}�b�v�^�C�v
	MapType type;
	// ���C���[
	std::vector<LayerData> layers;
};

class JsonLoader
{
public:
	~JsonLoader();
	static JsonLoader& Get();

	// .json�f�[�^�ǂݍ���
	int Load(const std::string& filePath);

	// ������𐔒l�ɕϊ�
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{ 
		return tree.get_optional<T>(str.c_str()).value();
	}

private:
	JsonLoader();
	JsonLoader(const JsonLoader&) = delete;
	void operator=(const JsonLoader&) = delete;

	// ������
	int Init();

	// .json�f�[�^
	boost::property_tree::ptree data;

	// �X�e�[�W
	StageData stage;

	// �}�b�v�^�C�v
	std::unordered_map<std::string, MapType> mapType;

	// ���C���[�^�C�v
	std::unordered_map<std::string, LayerType> layerType;
};