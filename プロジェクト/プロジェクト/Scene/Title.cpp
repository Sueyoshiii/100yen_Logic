#include "Title.h"
#include "GameMain.h"
#include "../Game/Game.h"
#include "../Json/JsonLoader.h"

// コンストラクタ
Title::Title(std::weak_ptr<MyLib>lib)
{
	this->lib = lib;

	JsonLoader::Get().Load("data/stage/test.json");

#ifdef _DEBUG
	std::cout << "Title Scene" << std::endl;
#endif
}

// デストラクタ
Title::~Title()
{
}
// 描画
void Title::Draw()
{
}

// 処理
void Title::UpData()
{
	if (Input::Get().IsTrigger(Key::Space))
	{
		Game::Get().ChangeScene(new GameMain(lib));
	}
}
