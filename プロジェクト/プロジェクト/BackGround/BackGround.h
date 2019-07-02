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

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// カメラ
	std::weak_ptr<Camera> cam;

	// 反転フラグ
	bool turnFlag;

	std::vector<BackParameter> layer;

	Vec2f oldCamPos;
	float offset;
};