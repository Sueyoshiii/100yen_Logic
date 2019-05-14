#pragma once
#include "Scene.h"

class Over :
	public Scene
{
public:
	// コンストラクタ
	Over(std::weak_ptr<MyLib>lib);
	// デストラクタ
	~Over();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:

};
