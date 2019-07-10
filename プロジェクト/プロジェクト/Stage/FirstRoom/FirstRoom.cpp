#include "FirstRoom.h"

FirstRoom::FirstRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam)
{
	Load(lib, cam, "data/stage/map.json", "img/Stage/tileset.png");
}

FirstRoom::~FirstRoom()
{
}

void FirstRoom::Update()
{
}
