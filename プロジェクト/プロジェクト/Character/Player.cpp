#include "Player.h"
#include "../Camera/Camera.h"
#include <iostream>
#include <algorithm>

const float Const::SPEED        = 2.0f;
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
	tex.pos.y += vel.y;
	if (tex.pos.y > Const::GROUND)
	{
		tex.pos.y = Const::GROUND;
	}
}

// 描画
void Player::Draw()
{
	float left = cam.lock()->GetViewPort().Left();
	float right = cam.lock()->GetViewPort().Right();
	//tex.pos.x = std::min(std::max(tex.pos.x, left), right);
	//tex.pos = cam.lock()->Correction(tex.pos);
	lib.lock()->Draw(tex, 1.0f, turnFlag);
	std::cout << tex.pos.x << ", " << cam.lock()->GetPos().x << std::endl;
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
		tex.pos.x -= Const::SPEED;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		tex.pos.x += Const::SPEED;
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
		tex.pos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		tex.pos.x += vel.x;
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
