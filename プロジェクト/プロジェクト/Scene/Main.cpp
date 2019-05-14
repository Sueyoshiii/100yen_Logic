#include "Main.h"
#include <MyLib.h>

// コンストラクタ
Main::Main(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
}

// デストラクタ
Main::~Main()
{
}

// 描画
void Main::Draw(void)
{
}

// 処理
void Main::UpData(void)
{
}