#pragma once
#include "Scene.h"

class Clear :
	public Scene
{
public:
	// コンストラクタ
	Clear(std::weak_ptr<MyLib>lib);
	// デストラクタ
	~Clear();

	// 描画
	void Draw();

	// 処理
	void Update();

private:
};
