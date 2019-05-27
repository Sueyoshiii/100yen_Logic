#pragma once
#include "Enemy.h"

class Wolf : 
	public Enemy
{
public:
	Wolf(std::weak_ptr<MyLib> lib);
	~Wolf();
private:
};