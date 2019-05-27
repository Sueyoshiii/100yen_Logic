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

	std::weak_ptr<Player> pl;
private:
};