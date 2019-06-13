#include "Clear.h"
#include <MyLib.h>

// コンストラクタ
Clear::Clear(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
#ifdef _DEBUG
	std::cout << "GameClear Scene" << std::endl;
#endif
}

// デストラクタ
Clear::~Clear()
{
}

// 描画
void Clear::Draw()
{
}

// 処理
void Clear::UpData()
{
}
