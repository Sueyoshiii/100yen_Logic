#include "GameMain.h"
#include "../Character/Player.h"
#include <MyLib.h>

// コンストラクタ
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	pl = std::make_shared<Player>(lib);
}

// デストラクタ
GameMain::~GameMain()
{
}

// 描画
void GameMain::Draw(void)
{
	pl->Draw();
}

// 処理
void GameMain::UpData(void)
{
	pl->Update();
}