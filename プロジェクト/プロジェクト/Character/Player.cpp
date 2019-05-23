#include "Player.h"
#include "../Camera/Camera.h"
#include "../Stage/Stage.h"
#include <iostream>
#include <algorithm>

const float Const::SPEED    = 4.0f;
const float Const::DUSH_POW = 10.0f;
const float Const::JUMP_POW = -18.0f;
const float Const::GR       = 0.98f;
const float Const::GROUND   = 500.0f;

// コンストラクタ
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dushFlag(false), cam(cam)
{
	this->lib = lib;

	LoadData("data/chara/player.info");
	LoadImage("img/player.png");

	InitFunc();
	ChangeState(ST::Neutral);

	vel = Vec2f(Const::SPEED, 0.0f);
}

// デストラクタ
Player::~Player()
{
}

// 更新
void Player::Update()
{
	func[state]();

	float left  = Stage::Get().GetRange().Left();
	float right = Stage::Get().GetRange().Right();
	localPos.x = std::min(std::max(localPos.x, left), right - tex.size.x);

	vel.y += Const::GR;
	localPos.y += vel.y;
	localPos.y = std::min(localPos.y, Const::GROUND);

	tex.pos = cam.lock()->Correction(localPos);
}

// 描画
void Player::Draw()
{
	AnimationUpdate();

	DrawImage();
	DrawRect();
}

// 待機
void Player::NeutralUpdate()
{
	if (localPos.y < Const::GROUND)
	{
		jumpFlag = true;
		ChangeState(ST::Jump);
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
		localPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		localPos.x += vel.x;
	}
	else {
		ChangeState(ST::Neutral);
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
void Player::Walk()
{
	vel.x = Const::SPEED;
	ChangeState(ST::Walk);
}

// ジャンプ
void Player::JumpUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		localPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		localPos.x += vel.x;
	}

	if (!dushFlag && In.IsTrigger(Key::X))
	{
		Dush();
	}

	if (tex.pos.y >= Const::GROUND)
	{
		jumpFlag = false;
		ChangeState(ST::Neutral);
	}
}
void Player::Jump()
{
	jumpFlag = true;
	vel.y    = Const::JUMP_POW;
	ChangeState(ST::Jump);
}

// ダッシュ
void Player::DushUpdate()
{
	localPos.x += vel.x;
	if (CheckAnimEnd())
	{
		dushFlag = false;
		ChangeState(ST::Neutral);
	}
}
void Player::Dush()
{
	dushFlag = true;
	vel.x = turnFlag ? -Const::DUSH_POW : Const::DUSH_POW;
	ChangeState(ST::Dash);
}

// 攻撃1
void Player::Attack1Update()
{
}

// 攻撃2
void Player::Attack2Update()
{
}

// 攻撃3
void Player::Attack3Update()
{
}

// ダメージ
void Player::DamageUpdate()
{
}

// 死亡
void Player::DeathUpdate()
{
}

// 状態と関数をバインド
void Player::InitFunc()
{
	func.clear();

	func[ST::Neutral] = std::bind(&Player::NeutralUpdate, this);
	func[ST::Walk]    = std::bind(&Player::WalkUpdate, this);
	func[ST::Jump]    = std::bind(&Player::JumpUpdate, this);
	func[ST::Dash]    = std::bind(&Player::DushUpdate, this);
	func[ST::Attack1] = std::bind(&Player::Attack1Update, this);
	func[ST::Attack2] = std::bind(&Player::Attack2Update, this);
	func[ST::Attack3] = std::bind(&Player::Attack3Update, this);
	func[ST::Damage]  = std::bind(&Player::DamageUpdate, this);
	func[ST::Death]   = std::bind(&Player::DeathUpdate, this);
}

Vec2f Player::GetLocalPos() const
{
	return localPos;
}
