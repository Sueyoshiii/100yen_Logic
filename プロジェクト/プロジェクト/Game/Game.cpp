#include "Game.h"
#include "../Scene/Scene.h"
#include "../Scene/Title.h"
#include "../Scene/GameMain.h"

Game::Game()
{
}

Game::~Game()
{
}

Game& Game::Get()
{
	static Game instance;
	return instance;
}

// 初期化
void Game::Init()
{
	lib = std::make_shared<MyLib>(640);
	lib->ChangeTitle("LittleRed");

	ChangeScene(new GameMain(lib));
}

// 更新
void Game::Update()
{
	while (lib->CheckMsg() && !(Input::Get().IsKey(Key::Escape)))
	{
		scene->UpData();

		Draw();
	}
}

// 描画
void Game::Draw()
{
	lib->Clear();

	scene->Draw();

	lib->Execution();
}

void Game::ChangeScene(Scene* scene)
{
	this->scene.reset(scene);
}
