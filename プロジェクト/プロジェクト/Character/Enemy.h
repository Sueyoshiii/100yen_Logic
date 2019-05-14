#pragma once
#include "Character.h"
#include <memory>
#include <MyLib.h>

class Enemy : 
	public Character
{
public:
	Enemy(std::weak_ptr<MyLib> lib);
	~Enemy();

	// XV
	void Update();

	// •`‰æ
	void Draw();
private:
};