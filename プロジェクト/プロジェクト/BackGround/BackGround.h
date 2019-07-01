#pragma once
#include <MyLib.h>
#include <memory>
#include <vector>

class Camera;

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
	// 背景画像
	std::vector<Texture> tex;

	std::vector<Texture> layer1;
	std::vector<Texture> layer2;
	std::vector<Texture> layer3;

	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// カメラ
	std::weak_ptr<Camera> cam;

	// 反転フラグ
	bool turnFlag;
};