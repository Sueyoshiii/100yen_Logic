#pragma once
#include "../Stage.h"

class FirstRoom :
	public Stage
{
public:
	FirstRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
	~FirstRoom();

	void Update();

	void Draw();
private:
};