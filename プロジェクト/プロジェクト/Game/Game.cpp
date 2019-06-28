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

// 初期化
void Game::Init()
{
	Vec2 screenSize = Vec2(1280, 720);
	//Vec2 screenSize = Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	lib = std::make_shared<MyLib>(screenSize);
	lib->ChangeTitle("LittleRed");

	ChangeScene(new GameMain(lib));
}

// 更新
void Game::Update()
{
	while (lib->CheckMsg() && !(Input::Get().IsKey(Key::Escape)))
	{

		Draw();
		scene->UpData();
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
