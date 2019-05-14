#pragma once
#include <MyLib.h>
#include <memory>
#pragma comment(lib, "Lib.lib")

//class MyLib;

class Game
{
public:
	~Game();
	static Game& Get();

	// 初期化
	void Init();

	// 更新
	void Update();
private:
	Game();
	Game(const Game&) = delete;
	void operator=(const Game&) = delete;

	std::shared_ptr<MyLib> lib;
};
