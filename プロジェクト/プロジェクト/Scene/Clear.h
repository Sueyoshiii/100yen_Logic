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
	void Draw(void);

	// 処理
	void UpData(void);

private:

};
