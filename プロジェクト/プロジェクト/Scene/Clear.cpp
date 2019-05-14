#include "Clear.h"
#include <MyLib.h>

// コンストラクタ
Clear::Clear(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
}

// デストラクタ
Clear::~Clear()
{
}

// 描画
void Clear::Draw(void)
{
	lib.lock()->Clear();

	lib.lock()->Execution();
}

// 処理
void Clear::UpData(void)
{
}
