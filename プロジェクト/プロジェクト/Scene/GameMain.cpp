#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Character/Player.h"
#include "../Character/EnemyManager.h"
#include "../Character/Wolf.h"

#define Enem EnemyManager::Get()

// コンストラクタ
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	cam = std::make_shared<Camera>(lib);
	bg = std::make_shared <BackGround>(lib, cam);
	pl = std::make_shared<Player>(lib, cam);
	cam->SetFocus(pl);

	// 敵さん達
	Enem.Summons(EM::Wolf, Vec2f(100.0f, 0.0f), lib, pl, cam);
	//Enem.Summons(EM::Wolf, Vec2f(260.0f, 0.0f), lib, pl, cam);
}

// デストラクタ
GameMain::~GameMain()
{
}

// 描画
void GameMain::Draw(void)
{
	bg->Draw();
	Enem.Draw();
	pl->Draw();
}

// 処理
void GameMain::UpData(void)
{
	cam->Update();
	bg->Update();
	Enem.Update();
	pl->Update();
}