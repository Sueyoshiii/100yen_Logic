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

	virtual void InitFunc() = 0;

	std::weak_ptr<Player> pl;
private:
};