#include "Claw.h"

Claw::Claw(std::weak_ptr<Camera> cam, const Vec2f& pos, const Vec2f& size, const bool turnFlag) :
	attackCnt(0)
{
	camera = cam;

	state = "first";
	type = CharacterType::EF_EM_BOSS_CLAW;

	func.clear();
	func[state] = std::bind(&Claw::ClawUpdate, this);

	LoadData("data/chara/boss_effect.info");
	LoadImg("img/Boss/boss_effect.png");

	ChangeState(state);

	tex[type].size = size;
	this->turnFlag = turnFlag;
	float x = this->turnFlag ? pos.x : pos.x + tex[type].size.x / 2.0f;
	//float x = this->turnFlag ? pos.x : pos.x;
	//tex[type].pos = Vec2f(x, pos.y);
	worldPos = Vec2f(x, pos.y);
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

	tex[type].pos = camera.lock()->Correction(worldPos);

	AnimationUpdate();
	DrawImage();

#ifdef _DEBUG
	DrawRect();
#endif
}

void Claw::SetPos(const Vec2f& pos)
{
}

// ‚Ð‚Á‚©‚«UŒ‚
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
