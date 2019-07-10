#pragma once
#include "../Stage.h"

class SecondRoom :
	public Stage
{
public:
	SecondRoom(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
	~SecondRoom();

	void Update();

	void Draw();
};