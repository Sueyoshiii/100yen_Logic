#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Character/Player.h"
#include "../Character/Wolf.h"

// コンストラクタ
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	cam = std::make_shared<Camera>();
	bg = std::make_shared <BackGround>(lib, cam);
	pl = std::make_shared<Player>(lib, cam);
	wolf = std::make_shared<Wolf>(lib, pl, cam);
	cam->SetFocus(pl);
}

// デストラクタ
GameMain::~GameMain()
{
}

// 描画
void GameMain::Draw(void)
{
	bg->Draw();
	pl->Draw();
	wolf->Draw();
}

// 処理
void GameMain::UpData(void)
{
	cam->Update();
	bg->Update();
	pl->Update();
	wolf->Update();
}