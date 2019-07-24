#include "BloodSplash.h"

namespace {
	const unsigned int ANIM_NUM_MAX  = 5;
	const unsigned int ANIM_TIME_MAX = 6;
}

BloodSplash::BloodSplash(const Vec2f& pos, const Vec2f& size, const bool turnFlag) :
	animTime(0), animNum(0)
{
	blood.Load("img/Blood/Blood_all.png");
	this->turnFlag = turnFlag;
	blood.size = size / 2.0f;
	blood.pos  = this->turnFlag ? Vec2f(pos.x + size.x - blood.size.x, pos.y) : pos;
	blood.divSize = Vec2f(64.0f);
}

BloodSplash::~BloodSplash()
{
}

void BloodSplash::Update()
{
	if (++animTime % ANIM_TIME_MAX == 0)
	{
		if (animNum < ANIM_NUM_MAX)
		{
			blood.offsetPos.x = blood.divSize.x * animNum;
			++animNum;
		}
		else
		{
			deleteFlag = true;
		}
	}
}

void BloodSplash::Draw()
{
}
void BloodSplash::Draw(std::weak_ptr<MyLib> lib)
{
	lib.lock()->Draw(blood, 1.0f, turnFlag);
}

void BloodSplash::SetPos(const Vec2f& pos)
{
	blood.pos = pos;
}
