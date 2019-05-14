#include "Title.h"
#include "GameMain.h"
#include "../Game/Game.h"

// コンストラクタ
Title::Title(std::weak_ptr<MyLib>lib)
{
	this->lib = lib;
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
	if (Input::Get().IsTrigger(Key::Space))
	{
		Game::Get().ChangeScene(new GameMain(lib));
	}
}
