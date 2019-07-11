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
	stage.reset(new FirstRoom(lib, pl, cam));

	// 対象をプレイヤーにする
	cam->SetFocus(pl);

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
	}

	// プレイヤー
	pl->Draw();

	if (pl->GetState() != "Death")
	{
		// エフェクト
		EffectManager::Get().Draw(lib);
	}

	// 遷移用ボックス描画
	stage->DrawBox();
}

// 処理
void GameMain::UpData()
{
	if (!stage->GetNextRoomFlag())
	{
		// カメラ
		cam->Update();

		// 背景
		bg->Update();

		// ルーム
		stage->Update();

		// プレイヤー
		pl->Update();

		// エフェクト
		EffectManager::Get().Update();
	}

	// ルーム切り替えチェック
	CheckChangeRoom();

	// クリア時はクリアシーンへ
	if (INPUT.IsTrigger(Key::P))
	{
		ChangeNextScene(new Clear(lib));
	}
	// 死亡時はゲームオーバーシーンへ
	else if (pl->GetDeadEndFlag())
	{
		ChangeNextScene(new Over(lib));
	}
}

// シーン切り替え
void GameMain::ChangeNextScene(Scene* scene)
{
	// 登録されているオブジェクトの解放
	DeleteObject();

	// シーン切り替え
	Game::Get().ChangeScene(scene);
}

// ルーム切り替え
void GameMain::CheckChangeRoom()
{
	if (pl->GetWorldPos().x + pl->GetSize().x / 2.0f > StageManager::Get().GetRange().Right())
	{
		stage->SetNextRoomFlag(true);
		if (stage->GetBoxAlpha() >= 1.0f)
		{
			DeleteObject();
			pl->SetPos(pl->GetFirstPos());
			cam->SetPos(pl->GetWorldPos());
			stage.reset(stage->GetNextRoom());
		}
	}
}

// 登録されているオブジェクトの解放
void GameMain::DeleteObject()
{
	EnemyManager::Get().Delete();
	EffectManager::Get().Delete();
}
