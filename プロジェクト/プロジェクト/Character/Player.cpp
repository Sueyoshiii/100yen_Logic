#include "Player.h"
#include <iostream>

Player::Player(std::weak_ptr<MyLib> lib) :
	turnFlag(false), jumpFlag(false)
{
	this->lib = lib;

	tex.Load("img/nino8.jpg");
	tex.size = Vec2f(50, 100);

	update = &Player::NeutralUpdate;
	ChangeState(ST::Neutral);
}

Player::~Player()
{
}

// 更新
void Player::Update()
{
	(this->*update)();

	vel.y += GR * 1.2f;
	tex.pos.y += vel.y;
	if (tex.pos.y > GROUND)
	{
		tex.pos.y = GROUND;
	}
}

// 描画
void Player::Draw()
{
	lib.lock()->Draw(tex, 1.0f, turnFlag);
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
	std::cout << "neutral" << std::endl;
}

// 歩行
void Player::WalkUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		tex.pos.x -= SPEED;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		tex.pos.x += SPEED;
	}
	else {
		update = &Player::NeutralUpdate;
		ChangeState(ST::Neutral);
	}

	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		Jump();
	}
	std::cout << "walk" << std::endl;
}

// ジャンプ
void Player::JumpUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		tex.pos.x -= SPEED;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		tex.pos.x += SPEED;
	}

	if (tex.pos.y >= GROUND)
	{
		jumpFlag = false;
		update = &Player::NeutralUpdate;
		ChangeState(ST::Neutral);
	}
	std::cout << "jump" << std::endl;
}
void Player::Jump()
{
	jumpFlag = true;
	vel.y = JUMP_POW;
	update = &Player::JumpUpdate;
	ChangeState(ST::Jump);
}
