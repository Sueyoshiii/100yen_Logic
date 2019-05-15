#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../Character/Player.h"

// コンストラクタ
GameMain::GameMain(std::weak_ptr<MyLib> lib) :
	t("img/brides.jpg")
{
	this->lib = lib;
	cam = std::make_shared<Camera>();
	pl = std::make_shared<Player>(lib, cam);
	cam->SetFocus(pl);
}

// デストラクタ
GameMain::~GameMain()
{
}

// 描画
void GameMain::Draw(void)
{
	lib.lock()->Draw(t);
	pl->Draw();
}

// 処理
void GameMain::UpData(void)
{
	cam->Update();
	pl->Update();
}