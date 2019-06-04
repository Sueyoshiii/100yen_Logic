#include "Player.h"
#include "../Stage/Stage.h"
#include <iostream>

// �R���X�g���N�^
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dashFlag(false), attackFlag(false), attackCnt(0)
{
	this->lib = lib;
	this->cam = cam;

	LoadData("data/chara/player.info");
	LoadImage("img/player.png");

	InitFunc();
	ChangeState(ST::Neutral);

	tex.size *= 2.0f;

	tex.pos = Vec2f(0.0f, 0.0f);
	worldPos = cam.lock()->Correction(tex.pos);

	speed   = 5.0f;
	dushPow = 13.0f;
	jumpPow = -40.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 3;

	knockBackRange = 4.0f;
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

	UpdateLocalPos();

	InvicibleUpdate();
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
	if (worldPos.y < Const::GROUND)
	{
		jumpFlag = true;
		ChangeState(ST::Jump);
	}

	Walk();

	Jump();

	Dash();

	Attack1();
}

// ���s
void Player::WalkUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}
	else
	{
		ChangeState(ST::Neutral);
	}

	Jump();

	Dash();

	Attack1();
}
void Player::Walk()
{
	if (In.IsKey(Key::Num4) || In.IsKey(Key::Num6))
	{
		vel.x = speed;
		ChangeState(ST::Walk);
	}
}

// �W�����v
void Player::JumpUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}

	if (worldPos.y >= Const::GROUND)
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
		vel.y = jumpPow;
		ChangeState(ST::Jump);
	}
}

// �_�b�V��
void Player::DashUpdate()
{
	worldPos.x += vel.x;
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
		vel.x = turnFlag ? -dushPow : dushPow;
		ChangeState(ST::Dash);
	}
}

// �U��1
void Player::Attack1Update()
{
	NextAttack(15);
}
void Player::Attack1()
{
	if (!attackFlag && In.IsTrigger(Key::Z))
	{
		attackFlag = true;
		attackCnt  = 0;
		ChangeState(ST::Attack1);
	}
}

// �U��2
void Player::Attack2Update()
{
	NextAttack(15);
}

// �U��3
void Player::Attack3Update()
{
	NextAttack(25);
}

// ���̍U���ֈڂ�
void Player::NextAttack(const unsigned int attackInterval)
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (state != ST::Attack3)
		{
			if (In.IsTrigger(Key::Z))
			{
				attackCnt = 0;
				stopFlag = false;
				ChangeState(ST(int(state) + 1));
			}
		}
		if ((++attackCnt) > attackInterval)
		{
			attackCnt = 0;
			attackFlag = false;
			stopFlag = false;
			ChangeState(ST::Neutral);
		}
	}
}

// ��_���[�W
void Player::DamageUpdate()
{
	static unsigned int cnt = 0;
	if (tex.pos.y < Const::GROUND)
	{
		worldPos.x += vel.x;
	}
	else
	{
		if ((++cnt) > 40)
		{
			cnt = 0;
			jumpFlag = false;
			dashFlag = false;
			attackFlag = 0;
			if (hp >= 0)
			{
				invincibleFlag = true;
				ChangeState(ST::Neutral);
			}
			else
			{
				ChangeState(ST::Death);
			}
		}
	}

}

// ���S
void Player::DeathUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (In.IsTrigger(Key::A))
		{
			stopFlag = false;
			hp = 3;
			ChangeState(ST::Neutral);
		}
	}
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

// ���[�J�����W�擾
Vec2f Player::GetWorldPos() const
{
	return worldPos;
}
