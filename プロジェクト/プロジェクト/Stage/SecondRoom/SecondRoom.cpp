#include "SecondRoom.h"

SecondRoom::SecondRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam)
{
	Load(lib, cam, "data/stage/map.json", "img/Stage/tileset.png");
}

SecondRoom::~SecondRoom()
{
}

void SecondRoom::Update()
{
}
