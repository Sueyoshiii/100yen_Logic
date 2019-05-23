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

// �R���X�g���N�^
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

// �f�X�g���N�^
Player::~Player()
{
}

// �X�V
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

// �`��
void Player::Draw()
{
	AnimationUpdate();

	DrawImage();
	DrawRect();
}

// �ҋ@
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

// ���s
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

// �W�����v
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

// �_�b�V��
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

// �U��1
void Player::Attack1Update()
{
}

// �U��2
void Player::Attack2Update()
{
}

// �U��3
void Player::Attack3Update()
{
}

// �_���[�W
void Player::DamageUpdate()
{
}

// ���S
void Player::DeathUpdate()
{
}

// ��ԂƊ֐����o�C���h
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
