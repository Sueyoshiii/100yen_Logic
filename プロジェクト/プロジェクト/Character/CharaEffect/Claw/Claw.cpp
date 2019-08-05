#include "Claw.h"

Claw::Claw(const Vec2f& pos, const Vec2f& size, const bool turnFlag) :
	attackCnt(0)
{
	state = "first";

	func.clear();
	func[state] = std::bind(&Claw::ClawUpdate, this);

	type = CharacterType::EF_EM_BOSS_CLAW;
	LoadData("data/chara/boss_effect.info");
	LoadImage("img/Boss/boss_effect.png");

	ChangeState(state);

	tex[type].size = size;
	this->turnFlag = turnFlag;
	float x = this->turnFlag ? pos.x : pos.x;
	tex[type].pos = Vec2f(x, pos.y);
}

Claw::~Claw()
{
}

void Claw::Update()
{
	func[state]();
}

void Claw::Draw()
{
}

void Claw::Draw(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

	if (deleteFlag)
	{
		return;
	}

	AnimationUpdate();
	DrawImage();

	DrawRect();
#ifdef _DEBUG
#endif
}

void Claw::SetPos(const Vec2f& pos)
{
}

void Claw::ClawUpdate()
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
