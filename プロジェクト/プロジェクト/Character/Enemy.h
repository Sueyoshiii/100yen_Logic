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
	// �Փ˔���
	void CheckHit();

	std::weak_ptr<Player> pl;
private:
};