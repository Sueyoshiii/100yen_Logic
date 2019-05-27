#pragma once
#include "Character.h"

class Player;

class Enemy : 
	public Character
{
public:
	Enemy();
	~Enemy();
protected:
	// Õ“Ë”»’è
	void CheckHit();

	virtual void InitFunc() = 0;

	std::weak_ptr<Player> pl;
private:
};