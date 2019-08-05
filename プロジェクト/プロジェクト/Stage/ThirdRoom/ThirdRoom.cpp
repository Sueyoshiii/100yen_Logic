#include "ThirdRoom.h"
#include "../../Camera/Camera.h"
#include "../../Character/Player/Player.h"
#include "../../Character/Enemy/EnemyManager.h"
#include <iostream>

ThirdRoom::ThirdRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam)
{
	this->lib = lib;
	this->pl = pl;
	this->cam = cam;

	nextRoomFlag = false;

	Load(back, "data/stage/StageBoss/StageBoss_Grass.json", "img/Stage/Mapchip_Ground.png");
	Load(front, "data/stage/StageBoss/StageBoss_Grass.json", "img/Stage/Mapchip_Ground.png");

	box.pos[0] = Vec3f();
	box.pos[1] = Vec3f(float(lib.lock()->GetWinSize().x), 0.0f, 0.0f);
	box.pos[2] = Vec3f(0.0f, float(lib.lock()->GetWinSize().y * 2), 0.0f);
	box.pos[3] = Vec3f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y * 2), 0.0f);

	EnemyManager::Get().Summons(Enemies::BOSS_WOLF, Vec2f(50.0f, -500.0f), lib, pl, cam);

	length = 1;
}

ThirdRoom::~ThirdRoom()
{
}

void ThirdRoom::Update()
{
	Vec2f plPos = pl.lock()->GetWorldPos();
	float right = StageManager::Get().GetRange().Right() - pl.lock()->GetSize().x;
	pl.lock()->SetPos(Vec2f(std::min(plPos.x, right), plPos.y));

	EnemyManager::Get().Update();

	if (EnemyManager::Get().GetEnemyNum() <= 0)
	{
		StageManager::Get().SetClearFlag(true);
	}
}

void ThirdRoom::Draw()
{
	DrawMapData(cam);
	EnemyManager::Get().Draw();
}

void ThirdRoom::DrawFront()
{
	DrawMapDataFront(cam);
}
