#include "GameMane.h"
#include "../Input/Input.h"
#include "../Application/Application.h"
#include "../Scene/Title.h"
#include "../Typedef.h"

const Vec2 winSize = { 1280,720 };

// コンストラクタ
GameMane::GameMane()
{
}

// デストラクタ
GameMane::~GameMane()
{
}

// シーンの移行
void GameMane::ChangeScene(Scene * scene)
{
	this->scene.reset(scene);
}

// 初期化
void GameMane::Init(void)
{
	app = std::make_shared<Application>(winSize);
	ChangeScene(new Title(app));
}

// 描画
void GameMane::Draw(void)
{
	app->Clear();
	scene->Draw();
	app->Execution();
}

// 処理
void GameMane::UpData(void)
{
	scene->UpData();
}

// 処理
void GameMane::Run(void)
{
	Init();
	while (app->CheckMsg() && Input::Get().InputKey(INPUT_ESCAPE) == false)
	{
		Draw();
		UpData();
	}
	End();
}

// 終了
void GameMane::End(void)
{
	scene.reset();
	app.reset();
}
