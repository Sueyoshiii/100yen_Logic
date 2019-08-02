#include "Claw.h"

Claw::Claw(const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	type = CharacterType::EF_EM_BOSS_CLAW;
	LoadImage("img/Boss/boss_effect.png");

	tex[type].size = size;
	this->turnFlag = turnFlag;
	float x = this->turnFlag ? pos.x - size.x / 2.0f : pos.x + size.x / 2.0f;
	tex[type].pos = Vec2f(x, pos.y);

	attackCnt = 0;
}

Claw::~Claw()
{
}

void Claw::Update()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		++attackCnt;
	}

	if (attackCnt > 25)
	{
		attackCnt = 0;
		stopFlag = false;
		deleteFlag = true;
	}
}

void Claw::Draw()
{
}

void Claw::Draw(std::weak_ptr<MyLib> lib)
{
	lib.lock()->Draw(tex[type], 1.0f, turnFlag);
}
