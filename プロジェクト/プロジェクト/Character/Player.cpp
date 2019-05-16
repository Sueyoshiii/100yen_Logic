#include "Player.h"
#include "../Camera/Camera.h"
#include "../Stage/Stage.h"
#include <iostream>
#include <algorithm>

const float Const::SPEED        = 4.0f;
const float Const::JUMP_POW     = -18.0f;
const float Const::GR           = 0.98f;
const float Const::GROUND       = 500.0f;

Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), cam(cam)
{
	this->lib = lib;

	tex.Load("img/nino8.jpg");
	tex.size = Vec2f(50, 100);

	update = &Player::NeutralUpdate;
	ChangeState(ST::Neutral);

	vel = Vec2f(Const::SPEED, 0.0f);

	turnFlag = false;
}

Player::~Player()
{
}

// 更新
void Player::Update()
{
	(this->*update)();

	vel.y += Const::GR;
	pos.y += vel.y;
	pos.y = std::min(pos.y, Const::GROUND);

	float left = Stage::Get().GetRange().Left();
	float right = Stage::Get().GetRange().Right();
	pos.x = std::min(std::max(pos.x, left), right - tex.size.x);
}

// 描画
void Player::Draw()
{
	float left = Stage::Get().GetRange().Left();
	float right = Stage::Get().GetRange().Right();
	pos.x = std::min(std::max(pos.x, left), right);
	tex.pos = cam.lock()->Correction(pos);
	lib.lock()->Draw(tex, 1.0f, turnFlag);
	std::cout << pos.x << ", " << cam.lock()->GetPos().x << std::endl;
}

// 待機
void Player::NeutralUpdate()
{
	if (In.IsKey(Key::Num4) || In.IsKey(Key::Num6))
	{
		update = &Player::WalkUpdate;
		ChangeState(ST::Walk);
	}

	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		Jump();
	}
}

// 歩行
void Player::WalkUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		pos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		pos.x += vel.x;
	}
	else {
		update = &Player::NeutralUpdate;
		ChangeState(ST::Neutral);
	}

	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		Jump();
	}
}

// ジャンプ
void Player::JumpUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		pos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		pos.x += vel.x;
	}

	if (tex.pos.y >= Const::GROUND)
	{
		jumpFlag = false;
		update   = &Player::NeutralUpdate;
		ChangeState(ST::Neutral);
	}
}
void Player::Jump()
{
	jumpFlag = true;
	vel.y    = Const::JUMP_POW;
	update   = &Player::JumpUpdate;
	ChangeState(ST::Jump);
}

Vec2f Player::GetLocalPos() const
{
	return pos;
}
