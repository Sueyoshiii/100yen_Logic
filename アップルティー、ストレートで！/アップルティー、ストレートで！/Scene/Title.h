#pragma once
#include "Scene.h"

class Title :
	public Scene
{
public:
	// コンストラクタ
	Title(std::weak_ptr<Application>app);
	// デストラクタ
	~Title();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:

};
