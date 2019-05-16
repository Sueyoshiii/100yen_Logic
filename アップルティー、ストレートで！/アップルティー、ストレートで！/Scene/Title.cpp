#include "Title.h"
#include "../Input/Input.h"
#include "../GameMane/GameMane.h"
#include "../Application/Application.h"
#include "Play.h"

// コンストラクタ
Title::Title(std::weak_ptr<Application> app)
{
	this->app = app;
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
}

// 処理
void Title::UpData(void)
{
	if (Input::Get().Triger(INPUT_RETURN))
	{
		GameMane::Get().ChangeScene(new Play(app));
	}
}
