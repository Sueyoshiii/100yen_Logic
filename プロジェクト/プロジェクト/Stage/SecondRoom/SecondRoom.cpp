#include "SecondRoom.h"
#include "../ThirdRoom/ThirdRoom.h"
#include "../../Character/Player/Player.h"
#include "../../Character/Enemy/EnemyManager.h"
#include <iostream>

SecondRoom::SecondRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	nextRoomFlag = false;

	Load("data/stage/map.json", "img/Stage/tileset.png");

	box.pos[0] = Vec3f();
	box.pos[1] = Vec3f(float(lib.lock()->GetWinSize().x), 0.0f, 0.0f);
	box.pos[2] = Vec3f(0.0f, float(lib.lock()->GetWinSize().y * 2), 0.0f);
	box.pos[3] = Vec3f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y * 2), 0.0f);

	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(100.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(600.0f, 0.0f), lib, pl, cam);

	std::cout << "SecondRoom" << std::endl;
}

SecondRoom::~SecondRoom()
{
}

void SecondRoom::Update()
{
	EnemyManager::Get().Update();
}

void SecondRoom::Draw()
{
	DrawMapData();
	EnemyManager::Get().Draw();
}

Stage* SecondRoom::GetNextRoom()
{
	return new ThirdRoom(lib, pl, cam);
}
