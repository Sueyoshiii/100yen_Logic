#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Character/Player.h"
#include "../Character/EnemyManager.h"

#define Enemy EnemyManager::Get()

// コンストラクタ
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	cam = std::make_shared<Camera>(lib);
	bg = std::make_shared <BackGround>(lib, cam);
	pl = std::make_shared<Player>(lib, cam);
	cam->SetFocus(pl);

	// 敵さん達
	Enemy.Summons(EM::Wolf, Vec2f(100.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(600.0f, 0.0f), lib, pl, cam);
}

// デストラクタ
GameMain::~GameMain()
{
}

// 描画
void GameMain::Draw()
{
	bg->Draw();
	Enemy.Draw();
	pl->Draw();
}

// 処理
void GameMain::UpData()
{
	cam->Update();
	bg->Update();
	Enemy.Update();
	pl->Update();
}