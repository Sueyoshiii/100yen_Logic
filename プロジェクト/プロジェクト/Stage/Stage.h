#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <MyLib.h>
#include <unordered_map>
#include <vector>
#include <memory>

class Camera;

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

struct StageRange
{
private:
	float top, bottom, left, right;
public:
	StageRange() : top(0.0f), bottom(0.0f), left(0.0f), right(0.0f) {};
	StageRange(const float top, const float bottom, const float left, const float right) :
		top(top), bottom(bottom), left(left), right(right) {};
	float Top()const { return top; };
	float Bottom()const { return bottom; };
	float Left()const { return left; };
	float Right()const { return right; };
};

class Stage
{
	// �X�e�[�W
	struct Rect
	{
		Vec2f pos;
		float w, h;
		Rect() : pos(Vec2f()), w(0.0f), h(0.0f) {};
		Rect(const Vec2f& pos, const float w, const float h) :
			pos(pos), w(w), h(h) {};
		float Top()const { return pos.y - h / 2.0f; };
		float Bottom()const { return pos.y + h / 2.0f; };
		float Left()const { return pos.x - w / 2.0f; };
		float Right()const { return pos.x + w / 2.0f; };
	};

	// �萔
	struct ConstParam
	{
		// �d��
		static const float GR;
		// �n�ʁi���j
		// �X�e�[�W�f�[�^������������Ă���悤�ɂ���
		static const float GROUND;
	};
public:
	~Stage();
	static Stage& Get();

	// �X�e�[�W�f�[�^�ǂݍ���
	int Load(std::weak_ptr<Camera> cam, const std::string& jsonFilePath, const std::string& imgFilePath);

	// �`��
	void Draw(std::weak_ptr<MyLib> lib);

	// �X�e�[�W�͈͎擾
	StageRange GetRange()const;

	// �n�ʎ擾
	float GetGround()const;

	// �d�͎擾
	float GetGravity()const;
private:
	Stage();
	Stage(const Stage&) = delete;
	void operator=(const Stage&) = delete;

	// ������𐔒l�ɕϊ�
	template<typename T>
	T GetValue(const boost::property_tree::ptree& tree, const std::string& str)
	{
		return tree.get_optional<T>(str.c_str()).value();
	}

	// ������
	int Init();

	//�͈�
	Rect range;

	// .json�f�[�^
	boost::property_tree::ptree data;

	// �X�e�[�W
	StageData stage;

	// �}�b�v�^�C�v
	std::unordered_map<std::string, MapType> mapType;

	// ���C���[�^�C�v
	std::unordered_map<std::string, LayerType> layerType;

	Vec2f pos;
};