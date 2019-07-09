#include "Game.h"
#include "../Scene/Scene.h"
#include "../Scene/Title.h"
#include "../Scene/GameMain.h"
#include <Windows.h>

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

// èâä˙âª
void Game::Init()
{
	Vec2 screenSize = Vec2(1280, 640);
	//Vec2 screenSize = Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	lib = std::make_shared<MyLib>(screenSize);
	lib->ChangeTitle("LittleRed");

	ChangeScene(new Title(lib));
}

// çXêV
void Game::Update()
{
	while (lib->CheckMsg() && !(Input::Get().IsKey(Key::Escape)))
	{

		Draw();
		scene->UpData();
	}
}

// ï`âÊ
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
