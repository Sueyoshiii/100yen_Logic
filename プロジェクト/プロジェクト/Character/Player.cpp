#include "Player.h"
#include "../Camera/Camera.h"
#include "../Stage/Stage.h"
#include <iostream>
#include <algorithm>

const unsigned int Const::DIV_SIZE = 64;

const float Const::SPEED    = 4.0f;
const float Const::DUSH_POW = 10.0f;
const float Const::JUMP_POW = -18.0f;
const float Const::GR       = 0.98f;
const float Const::GROUND   = 500.0f;

Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dushFlag(false), cam(cam)
{
	this->lib = lib;

	Info::Get().Load("data/chara/player.info");
	info = Info::Get().GetData("data/chara/player.info");

	tex.Load("img/player.png");
	tex.size     = info.lock()->at(state).rect[index].anim.size;
	tex.divSize  = info.lock()->at(state).rect[index].anim.size;

	InitFunc();
	ChangeState("Neutral");

	vel = Vec2f(Const::SPEED, 0.0f);
}

Player::~Player()
{
}

// 更新
void Player::Update()
{
	func[state]();

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
	float left  = Stage::Get().GetRange().Left();
	float right = Stage::Get().GetRange().Right();
	pos.x = std::min(std::max(pos.x, left), right);
	tex.pos = cam.lock()->Correction(pos);

	unsigned int animTime = unsigned int(info.lock()->at(state).animTime);
	unsigned int animNum  = unsigned int(info.lock()->at(state).rect.size());

	if (animTime > 0)
	{
		frame = (++animCnt) % animTime == 0 ? (++frame) % animNum : frame;
	}
	if (frame >= animTime)
	{
		index = (index + 1) >= info.lock()->at(state).rect.size() ? 0 : ++index;
		frame = 0;
	}

	tex.offsetPos = {
		tex.offsetPos.x = tex.divSize.x * frame,
		info.lock()->at(state).rect[index].anim.pos.y
	};

	lib.lock()->Draw(tex, 1.0f, turnFlag);
	std::cout << pos.x << ", " << cam.lock()->GetPos().x << std::endl;
}

// 待機
void Player::NeutralUpdate()
{
	if (pos.y < Const::GROUND)
	{
		jumpFlag = true;
		ChangeState("Jump");
	}

	if (In.IsKey(Key::Num4) || In.IsKey(Key::Num6))
	{
		Walk();
	}

	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		Jump();
	}

	if (!dushFlag && In.IsTrigger(Key::X))
	{
		Dush();
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
		ChangeState("Neutral");
	}

	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		Jump();
	}
}
void Player::Walk()
{
	vel.x = Const::SPEED;
	ChangeState("Walk");
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
		ChangeState("Neutral");
	}
}
void Player::Jump()
{
	jumpFlag = true;
	vel.y    = Const::JUMP_POW;
	ChangeState("Jump");
}

// ダッシュ
void Player::DushUpdate()
{
	pos.x += vel.x;
	if (CheckAnimEnd())
	{
		dushFlag = false;
		ChangeState("Neutral");
	}
}
void Player::Dush()
{
	dushFlag = true;
	vel.x = turnFlag ? -Const::DUSH_POW : Const::DUSH_POW;
	ChangeState("Dash");
}

// 状態初期化
void Player::InitFunc()
{
	func.clear();

	func["Neutral"] = std::bind(&Player::NeutralUpdate, this);
	func["Walk"]    = std::bind(&Player::WalkUpdate, this);
	func["Jump"]    = std::bind(&Player::JumpUpdate, this);
	func["Dash"]    = std::bind(&Player::DushUpdate, this);
}

Vec2f Player::GetLocalPos() const
{
	return pos;
}
