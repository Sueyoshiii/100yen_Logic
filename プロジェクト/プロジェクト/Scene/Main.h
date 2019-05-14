#pragma once
#include "Scene.h"

class Main :
	public Scene
{
public:
	// コンストラクタ
	Main(std::weak_ptr<MyLib>lib);
	// デストラクタ
	~Main();

	// 描画
	void Draw(void);
	
	// 処理
	void UpData(void);

private:

};
