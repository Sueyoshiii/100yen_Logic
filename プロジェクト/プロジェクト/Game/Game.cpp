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

// ‰Šú‰»
void Game::Init()
{
	lib = std::make_shared<MyLib>(640);
	lib->ChangeTitle("LittleRed");

	ChangeScene(new GameMain(lib));
}

// XV
void Game::Update()
{
	while (lib->CheckMsg() && !(Input::Get().IsKey(Key::Escape)))
	{
		scene->UpData();

		Draw();
	}
}

// •`‰æ
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
