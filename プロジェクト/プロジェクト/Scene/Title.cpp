#include "Title.h"
#include <MyLib.h>

// コンストラクタ
Title::Title(std::weak_ptr<MyLib>lib)
{
	this->lib = lib;
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	lib.lock()->Clear();

	lib.lock()->Execution();
}

// 処理
void Title::UpData(void)
{
}
