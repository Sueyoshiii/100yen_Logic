#include "Over.h"
#include <MyLib.h>

// コンストラクタ
Over::Over(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
}

// デストラクタ
Over::~Over()
{
}

// 描画
void Over::Draw(void)
{
}

// 処理
void Over::UpData(void)
{
}
