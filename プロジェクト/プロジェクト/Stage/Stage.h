#pragma once
#include "../Camera/Camera.h"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>
#include <memory>


// �}�b�v�^�C�v
enum class MapType
{
	Map,
	Object,
};

// ���C���[�^�C�v
enum class LayerType
{
	Tile,
	Object,
	Image,
	Group
};

// �}�b�v�`�b�v
struct MapchipData
{
	int data;
	Texture tex;
	Vec2f worldPos;
	MapchipData() : data(0), tex(Texture()){}
	MapchipData(const int data, const Texture& tex) :
		data(data), tex(tex) {}
};

// ���C���[�f�[�^
struct LayerData
{
	// �}�b�v�f�[�^
	std::vector<MapchipData> chip;
	// ���C���[��
	std::string name;
	// ���C���[�^�C�v
	LayerType type;
	// �}�X��
	Vec2 massNum;
};

// �X�e�[�W�f�[�^
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

class Stage
{
public:
	Stage();
	~Stage();

	// �X�e�[�W�f�[�^�ǂݍ���
	int Load(const std::string& jsonFilePath, const std::string& imgFilePath);

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �J�ڃ{�b�N�X�`��
	virtual void DrawBox();

	// ���̃��[�����擾
	virtual Stage* GetNextRoom();

	// �{�b�N�X�̃A���t�@�l���擾
	virtual float GetBoxAlpha()const;

	virtual bool GetNextRoomFlag()const;

	virtual void SetNextRoomFlag(const bool flag);
protected:
	// �}�b�v�f�[�^�`��
	void DrawMapData();

	// .json�f�[�^
	boost::property_tree::ptree data;

	// �X�e�[�W
	StageData stage;

	// �}�b�v�^�C�v
	std::unordered_map<std::string, MapType> mapType;

	// ���C���[�^�C�v
	std::unordered_map<std::string, LayerType> layerType;

	std::weak_ptr<MyLib> lib;

	std::weak_ptr<Camera> cam;

	// �J�ڗp�{�b�N�X
	Primitive box;

	// �J�ڗp�{�b�N�X�̃A���t�@�l
	float boxAlpha;

	// ���̃��[���ɍs���邩�̊m�F�t���O
	bool nextRoomFlag;
private:
	// ������𐔒l�ɕϊ�
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{
		return tree.get_optional<T>(str.c_str()).value();
	}

	// ������
	int Init();

	Vec2f pos;
};