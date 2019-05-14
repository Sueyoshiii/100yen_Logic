#include "Enemy.h"

Enemy::Enemy(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
}

Enemy::~Enemy()
{
}

// XV
void Enemy::Update()
{
}

// •`‰æ
void Enemy::Draw()
{
	//lib.lock()->Draw(em);
}
