#pragma once
#include "../Stage.h"

class Player;
class Okdio;

class FirstRoom :
	public Stage
{
public:
	FirstRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam);
	~FirstRoom();

	void Update();

	void Draw();
	void DrawFront();

	Stage* GetNextRoom();
private:
	std::weak_ptr<Player> pl;
};