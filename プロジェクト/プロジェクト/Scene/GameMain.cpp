#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Stage/StageManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyManager.h"
#include "../Character/CharaEffect/EffectManager.h"
#include "../Game/Game.h"
#include "Clear.h"
#include "Over.h"
#include "../Stage/Stage.h"
#include "../Stage/FirstRoom/FirstRoom.h"
#include "../Stage/SecondRoom/SecondRoom.h"

#include "../Json/JsonLoader.h"

// コンストラクタ
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

	// 遷移用ボックスの設定
	box = Primitive(PrimitiveType::box);
	box.pos[0] = Vec3f();
	box.pos[1] = Vec3f(float(lib.lock()->GetWinSize().x), 0.0f, 0.0f);
	box.pos[2] = Vec3f(0.0f, float(lib.lock()->GetWinSize().y*2), 0.0f);
	box.pos[3] = Vec3f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y*2), 0.0f);
	boxAlpha   = 1.0f;

	// 生成
	cam = std::make_shared<Camera>(lib);
	bg  = std::make_shared <BackGround>(lib, cam);
	pl  = std::make_shared<Player>(lib, cam);

	// ステージデータの読み込み
	StageManager::Get().SetRange(lib.lock()->GetWinSize());
	stage.reset(new FirstRoom(lib, cam));

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
		stage->Draw();

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

	stage->DrawBox();
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

	// 死亡時はゲームオーバーへ
	if (pl->GetDeadEndFlag())
	{
		ChangeNextScene(new Over(lib));
	}

	if (INPUT.IsTrigger(Key::Q))
	{
		pl->SetPos(pl->GetFirstPos());
		stage.reset(new SecondRoom(lib, cam));
	}
	if (INPUT.IsTrigger(Key::W))
	{
		pl->SetPos(pl->GetFirstPos());
		stage.reset(new FirstRoom(lib, cam));
	}
}

// シーン切り替え
void GameMain::ChangeNextScene(Scene* scene)
{
	// 登録されているオブジェクトの解放
	EnemyManager::Get().Delete();
	EffectManager::Get().Delete();

	// シーン切り替え
	Game::Get().ChangeScene(scene);
}
