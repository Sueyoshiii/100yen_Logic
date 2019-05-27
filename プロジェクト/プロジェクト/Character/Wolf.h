#pragma once
#include "Enemy.h"

class Wolf : 
	public Enemy
{
public:
	Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam);
	~Wolf();
	
	void Update();
	void Draw();
private:
	void InitFunc();
	void NeutralUpdate();
};