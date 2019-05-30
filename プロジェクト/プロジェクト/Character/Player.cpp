#include "Player.h"
#include "../Stage/Stage.h"
#include <iostream>

const float Const::SPEED				  = 4.0f;
const float Const::DUSH_POW				  = 10.0f;
const float Const::JUMP_POW				  = -18.0f;
const unsigned int Const::ATTACK_INTERVAL = 60;
const float Const::GR					  = 0.98f;
const float Const::GROUND				  = 500.0f;

// �R���X�g���N�^
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dashFlag(false), AttackFlag(false)
{
	this->lib = lib;
	this->cam = cam;

	LoadData("data/player.info");
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
	// ��Ԋ֐�
	func[state]();

	CorrectPosInStage();

	FallUpdate();

	tex.pos = cam.lock()->Correction(localPos);
}

// �`��
void Player::Draw()
{
	AnimationUpdate();

	DrawImage();

#ifdef _DEBUG
	DrawRect();
#endif
}

// �ҋ@
void Player::NeutralUpdate()
{
	if (localPos.y < Const::GROUND)
	{
		jumpFlag = true;
		ChangeState(ST::Jump);
	}

	Walk();

	Jump();

	Dash();

	Attack1();
	NextAttack();
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
	else
	{
		ChangeState(ST::Neutral);
	}

	Jump();

	Dash();

	Attack1();
	NextAttack();
}
void Player::Walk()
{
	if (In.IsKey(Key::Num4) || In.IsKey(Key::Num6))
	{
		vel.x = Const::SPEED;
		ChangeState(ST::Walk);
	}
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

	if (tex.pos.y >= Const::GROUND)
	{
		jumpFlag = false;
		ChangeState(ST::Neutral);
	}
}
void Player::Jump()
{
	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		jumpFlag = true;
		vel.y = Const::JUMP_POW;
		ChangeState(ST::Jump);
	}
}

// �_�b�V��
void Player::DashUpdate()
{
	localPos.x += vel.x;
	if (CheckAnimEnd())
	{
		dashFlag = false;
		ChangeState(ST::Neutral);
	}
}
void Player::Dash()
{
	if (!dashFlag && In.IsTrigger(Key::X))
	{
		dashFlag = true;
		vel.x = turnFlag ? -Const::DUSH_POW : Const::DUSH_POW;
		ChangeState(ST::Dash);
	}
}

// �U��1
void Player::Attack1Update()
{
	if (CheckAnimEnd())
	{
		oldState = state;
		ChangeState(ST::Neutral);
	}
}
void Player::Attack1()
{
	if (!AttackFlag && In.IsTrigger(Key::Z))
	{
		AttackFlag = true;
		ChangeState(ST::Attack1);
	}
}

// �U��2
void Player::Attack2Update()
{
	if (CheckAnimEnd())
	{
		oldState = state;
		ChangeState(ST::Neutral);
	}
}

// �U��3
void Player::Attack3Update()
{
	static unsigned int cnt = 0;
	if (!CheckAnimEnd())
	{
		return;
	}
	frame = info.lock()->at(stMap[state]).rect.size() - 1;
	stopFlag = true;
	if ((++cnt) > 15)
	{
		cnt = 0;
		stopFlag = false;
		AttackFlag = false;
		oldState = state;
		ChangeState(ST::Neutral);
	}
}

// ���̍U���ֈڂ�
void Player::NextAttack()
{
	static unsigned int cnt = 0;

	if (!AttackFlag)
	{
		return;
	}

	if (cnt > Const::ATTACK_INTERVAL)
	{
		oldState = state;
		cnt = 0;
		AttackFlag = false;
	}
	else
	{
		if (In.IsTrigger(Key::Z))
		{
			ChangeState(ST(int(oldState) + 1));
		}
	}

	++cnt;
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
	func[ST::Dash]    = std::bind(&Player::DashUpdate, this);
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
