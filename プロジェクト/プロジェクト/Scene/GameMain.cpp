#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Stage/Stage.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyManager.h"
#include "../Character/CharaEffect/EffectManager.h"

#include "../Json/JsonLoader.h"

// コンストラクタ
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

	cam = std::make_shared<Camera>(lib);
	bg  = std::make_shared <BackGround>(lib, cam);
	pl  = std::make_shared<Player>(lib, cam);

	// ステージデータの読み込み
	Stage::Get().Load(cam, "data/stage/map.json", "img/Stage/tileset.png");

	// 対象をプレイヤーにする
	cam->SetFocus(pl);
	
	// 敵さん達
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(100.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(600.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(800.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(1000.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(1200.0f, 0.0f), lib, pl, cam);

	// test
	//JsonLoader::Get().Load("data/stage/map.json");

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
	if (pl->GetState() != "Death")
	{
		// 背景
		bg->Draw();

		// ステージ
		Stage::Get().Draw(lib);

		// 敵
		EnemyManager::Get().Draw();
	}

	// プレイヤー
	pl->Draw();

	if (pl->GetState() != "Death")
	{
		// エフェクト
		EffectManager::Get().Draw(lib);
	}
}

// 処理
void GameMain::UpData()
{
	// カメラ
	cam->Update();

	// 背景
	bg->Update();

	// 敵
	EnemyManager::Get().Update();

	// プレイヤー
	pl->Update();

	// エフェクト
	EffectManager::Get().Update();
}