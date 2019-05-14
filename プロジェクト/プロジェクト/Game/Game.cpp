#include "Game.h"

Game::Game() :
	lib(std::make_shared<MyLib>(640))
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
	lib->ChangeTitle("LittleRed");
}

// XV
void Game::Update()
{
	Texture img("img/nino8.jpg");
	while (lib->CheckMsg() && !(Input::Get().IsKey(Key::Escape)))
	{
		lib->Clear();
		lib->Draw(img);
		lib->Execution();
		if (Input::Get().IsKey(Key::Num4))
		{
			img.pos.x--;
		}
		if (Input::Get().IsKey(Key::Num6))
		{
			img.pos.x++;
		}
		if (Input::Get().IsKey(Key::Num8))
		{
			img.pos.y--;
		}
		if (Input::Get().IsKey(Key::Num2))
		{
			img.pos.y++;
		}
	}
}
