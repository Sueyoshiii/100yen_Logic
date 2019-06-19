#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Stage/Stage.h"
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

	// ステージデータの読み込み
	Stage::Get().Load("data/stage/untitled.json");

	// 対象をプレイヤーにする
	cam->SetFocus(pl);

	// 敵さん達
	Enemy.Summons(EM::Wolf, Vec2f(100.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(600.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(800.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(1000.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(1200.0f, 0.0f), lib, pl, cam);

#ifdef _DEBUG
	std::cout << "GameMain Scene" << std::endl;
#endif
}

// デストラクタ
GameMain::~GameMain()
{
}

// 描画
void GameMain::Draw()
{
	// 背景
	bg->Draw();

	// ステージ
	Stage::Get().Draw();

	// 敵
	Enemy.Draw();

	// プレイヤー
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