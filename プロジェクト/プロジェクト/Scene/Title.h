#pragma once
#include "Scene.h"
#include <MyLib.h>
#include <wrl.h>

class Okdio;

class Title :
	public Scene
{
public:
	// コンストラクタ
	Title(std::weak_ptr<MyLib>lib);
	// デストラクタ
	~Title();

	// 描画
	void Draw();

	// 処理
	void Update();
private:
	bool checkNext;
	Okdio* okdio;
};
