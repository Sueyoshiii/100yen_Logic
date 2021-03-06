#include "FirstRoom.h"
#include "../SecondRoom/SecondRoom.h"
#include "../../Character/Player/Player.h"
#include "../../Character/Enemy/EnemyManager.h"
#include "../../Scene/GameMain.h"
#include "../../SoundManager.h"

FirstRoom::FirstRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam)
{
	this->lib = lib;
	this->pl = pl;
	this->cam = cam;

	nextRoomFlag = false;

	Load(back, "data/stage/Stage1/Stage1_Ground.json", "img/Stage/Mapchip_Ground.png");
	Load(front, "data/stage/Stage1/Stage1_Grass.json", "img/Stage/Mapchip_Grass.png");

	box.pos[0] = Vec3f();
	box.pos[1] = Vec3f(float(lib.lock()->GetWinSize().x), 0.0f, 0.0f);
	box.pos[2] = Vec3f(0.0f, float(lib.lock()->GetWinSize().y * 2), 0.0f);
	box.pos[3] = Vec3f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y * 2), 0.0f);

	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(600.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(1400.0f, 0.0f), lib, pl, cam);

	length = 3;

	SoundManager::Get().Play(0);
}

FirstRoom::~FirstRoom()
{
}

void FirstRoom::Update()
{
	EnemyManager::Get().Update();
}

void FirstRoom::Draw()
{
	DrawMapData(cam);
	EnemyManager::Get().Draw();
}

void FirstRoom::DrawFront()
{
	DrawMapDataFront(cam);
}

Stage* FirstRoom::GetNextRoom()
{
	//pl.lock()->ChangeState("Neutral");
	pl.lock()->SoundStop();
	return new SecondRoom(lib, pl, cam);
}
