#include "Enemy.h"

Enemy::Enemy(std::weak_ptr<MyLib> lib) :
	em(Texture())
{
	this->lib = lib;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	//lib.lock()->Draw(em);
}
