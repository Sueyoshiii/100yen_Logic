#pragma once
#include "../Camera/Camera.h"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>
#include <memory>

enum class Dir
{
	Up,
	Down,
	Left,
	Right
};

// �`�b�v�̎��
enum class ChipType
{
	None,
	Wall
};

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
	//std::vector<MapchipData> chip;
	std::vector<std::vector<MapchipData>> chips;
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
	virtual ~Stage();

	// �X�e�[�W�f�[�^�ǂݍ���
	int Load(StageData& stage, const std::string& jsonFilePath, const std::string& imgFilePath);

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	virtual void DrawFront() = 0;

	// �J�ڃ{�b�N�X�`��
	virtual void DrawBox();

	bool CheckWall(const Vec2f& pos, const Vec2f& size, const bool turnFlag, const Dir& dir);

	bool CheckMapChip(const Vec2f& pos);

	void CheckMapCol(Vec2f& pos, const Vec2f& size, const Vec2f& vel);

	// ���̃��[�����擾
	virtual Stage* GetNextRoom();

	// �{�b�N�X�̃A���t�@�l���擾
	float GetBoxAlpha()const;

	// �X�e�[�W�T�C�Y�擾
	Vec2f GetStageSize()const;

	Vec2 GetChipSize()const;

	// �����ړ��t���O���擾
	bool GetNextRoomFlag()const;

	// �����ړ��t���O��ݒ�
	void SetNextRoomFlag(const bool flag);
protected:
	// �}�b�v�f�[�^�`��
	void DrawMapData(std::weak_ptr<Camera> cam);
	void DrawMapDataFront(std::weak_ptr<Camera> cam);

	// .json�f�[�^
	boost::property_tree::ptree data;

	// �X�e�[�W
	StageData back;
	// ��O
	StageData front;

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

	int length;

	Vec2 chipSize;
private:
	// ������𐔒l�ɕϊ�
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{
		return tree.get_optional<T>(str.c_str()).value();
	}

	// ������
	int Init();

	int chipMax;
};