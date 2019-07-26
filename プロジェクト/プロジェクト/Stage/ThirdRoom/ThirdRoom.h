#pragma once
#include "../Stage.h"

class Camera;
class Player;

class ThirdRoom :
	public Stage
{
public:
	ThirdRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam);
	~ThirdRoom();

	void Update();
	void Draw();
	void DrawFront();

private:
	std::weak_ptr<Player> pl;
};