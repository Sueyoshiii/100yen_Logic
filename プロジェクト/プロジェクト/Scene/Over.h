#pragma once
#include "Scene.h"

class Okdio;

class Over :
	public Scene
{
public:
	// コンストラクタ
	Over(std::weak_ptr<MyLib>lib);
	// デストラクタ
	~Over();

	// 描画
	void Draw();

	// 処理
	void Update();

private:
	bool checkNext;
};
