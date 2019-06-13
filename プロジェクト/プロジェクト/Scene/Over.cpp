#include "Over.h"
#include <MyLib.h>

// コンストラクタ
Over::Over(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
#ifdef _DEBUG
	std::cout << "GameOver Scene" << std::endl;
#endif
}

// デストラクタ
Over::~Over()
{
}

// 描画
void Over::Draw()
{
}

// 処理
void Over::UpData()
{
}
