#pragma once
#include <MyLib.h>
#include <memory>
#include <vector>
#include <array>

class Camera;

struct BackParameter
{
	std::array<Texture, 2> tex;
	std::array<Vec2f, 2> oldPos;
};

class BackGround
{
public:
	BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
	~BackGround();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �J����
	std::weak_ptr<Camera> cam;

	// ���]�t���O
	bool turnFlag;

	std::vector<BackParameter> layer;

	Vec2f oldCamPos;
	float offset;
};