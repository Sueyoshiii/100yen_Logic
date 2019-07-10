#include "SecondRoom.h"

SecondRoom::SecondRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam)
{
	this->lib = lib;
	this->cam = cam;

	Load("data/stage/map.json", "img/Stage/tileset.png");

	box.pos[0] = Vec3f();
	box.pos[1] = Vec3f(float(lib.lock()->GetWinSize().x), 0.0f, 0.0f);
	box.pos[2] = Vec3f(0.0f, float(lib.lock()->GetWinSize().y * 2), 0.0f);
	box.pos[3] = Vec3f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y * 2), 0.0f);
}

SecondRoom::~SecondRoom()
{
}

void SecondRoom::Update()
{
}

void SecondRoom::Draw()
{
	DrawMapData();
}
