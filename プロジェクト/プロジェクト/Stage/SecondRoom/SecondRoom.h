#pragma once
#include "../Stage.h"

class Player;

class SecondRoom :
	public Stage
{
public:
	SecondRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam);
	~SecondRoom();

	void Update();

	void Draw();

	Stage* GetNextRoom();
private:
	std::weak_ptr<Player> pl;
};