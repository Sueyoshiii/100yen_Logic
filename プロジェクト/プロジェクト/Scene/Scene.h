#pragma once
#include <MyLib.h>
#include <memory>
#include <iostream>

class Scene
{
public:
	// コンストラクタ
	Scene() {}
	// デストラクタ
	~Scene() {}

	// 描画
	virtual void Draw() = 0;

	// 処理
	virtual void UpData() = 0;

protected:
	// ライブラリ
	std::weak_ptr<MyLib>lib;

	// 画像
	Texture tex;

	// アルファ値
	float alpha;
};
