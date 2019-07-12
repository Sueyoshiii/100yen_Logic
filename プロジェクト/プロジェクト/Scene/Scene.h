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
	virtual void Update() = 0;

protected:
	// ライブラリ
	std::weak_ptr<MyLib>lib;

	// 画像
	Texture tex;

	// 画像のアルファ値
	float alpha;

	// 遷移用ボックス
	Primitive box;

	// 遷移用ボックスのアルファ値
	float boxAlpha;

	// 遷移確認フラグ
	bool checkNext;
};
