#include "Player.h"
#include "../../Stage/Stage.h"
#include <iostream>

// �R���X�g���N�^
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dashFlag(false), attackFlag(false), attackCnt(0)
{
	this->lib = lib;
	this->cam = cam;

	LoadData("data/chara/player.info");
	LoadImage("img/Player/player.png");

	InitFunc();
	ChangeState(CharacterState::Neutral);

	tex.size *= 2.0f;

	tex.pos = Vec2f(0.0f, 0.0f);
	worldPos = cam.lock()->Correction(tex.pos);

	// hp, speed, attack, defense, dush, jump
	cParam = CharacterParameter(2, 5.0f, 2, 2, 10.0f, -40.0f);
	vel     = Vec2f(cParam.speed, 0.0f);

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

	if (!jumpFlag && 
		worldPos.y < Stage::Get().GetGround() &&
		state != CharacterState::Damage)
	{
		CheckFall();
	}

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
	CheckWalk();

	CheckJump();

	CheckDash();

	CheckFirstAttack();
}

// ���s
void Player::WalkUpdate()
{
	if (INPUT.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (INPUT.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}
	else
	{
		ChangeState(CharacterState::Neutral);
	}

	CheckJump();

	CheckDash();

	CheckFirstAttack();
}
void Player::CheckWalk()
{
	if (INPUT.IsKey(Key::Num4) || INPUT.IsKey(Key::Num6))
	{
		vel.x = cParam.speed;
		ChangeState(CharacterState::Walk);
	}
}

// �W�����v
void Player::JumpUpdate()
{
	if (INPUT.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (INPUT.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}

	if (vel.y <= 0)
	{
		vel.y += Stage::Get().GetGravity();
		worldPos.y += vel.y;
	}
	else
	{
		jumpFlag = false;
		ChangeState(CharacterState::Fall);
	}
}
void Player::CheckJump()
{
	if (!jumpFlag && INPUT.IsTrigger(Key::Space))
	{
		jumpFlag = true;
		vel.y = cParam.jumpPow;
		ChangeState(CharacterState::Jump);
	}
}

// ����
void Player::FallUpdate()
{
	if (INPUT.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (INPUT.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}

	vel.y += Stage::Get().GetGravity();
	worldPos.y += vel.y;
	if (worldPos.y > Stage::Get().GetGround())
	{
		worldPos.y = Stage::Get().GetGround();
		ChangeState(CharacterState::Neutral);
	}
}
void Player::CheckFall()
{
	ChangeState(CharacterState::Fall);
}

// �_�b�V��
void Player::DashUpdate()
{
	worldPos.x += vel.x;
	static unsigned int cnt = 0;
	if (++cnt > 15)
	{
		cnt = 0;
		dashFlag = false;
		stopFlag = false;
		ChangeState(CharacterState::Neutral);
	}
}
void Player::CheckDash()
{
	if (!dashFlag && INPUT.IsTrigger(Key::X))
	{
		dashFlag = true;
		stopFlag = true;
		attackCnt = 0;
		attackFlag = false;
		vel.x = turnFlag ? -cParam.dushPow : cParam.dushPow;
		ChangeState(CharacterState::Dash);
	}
}

// 1�i�ڂ̍U��
void Player::FirstAttackUpdate()
{
	CheckNextAttack(15);
}
void Player::CheckFirstAttack()
{
	if (!attackFlag && INPUT.IsTrigger(Key::Z))
	{
		attackFlag = true;
		attackCnt  = 0;
		ChangeState(CharacterState::Attack1);
	}
}

// 2�i�ڂ̍U��
void Player::SecondAttackUpdate()
{
	CheckNextAttack(15);
}

// 3�i�ڂ̍U��
void Player::ThirdAttackUpdate()
{
	CheckNextAttack(25);
}

// ���̍U���ֈڂ�
void Player::CheckNextAttack(const unsigned int attackInterval)
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (state != CharacterState::Attack3)
		{
			if (INPUT.IsTrigger(Key::Z))
			{
				attackCnt = 0;
				stopFlag = false;
				ChangeState(CharacterState(int(state) + 1));
			}
			CheckDash();
		}
		if ((++attackCnt) > attackInterval)
		{
			attackCnt = 0;
			attackFlag = false;
			stopFlag = false;
			ChangeState(CharacterState::Neutral);
		}
	}
	else
	{
		CheckDash();
	}
}

// ��_���[�W
void Player::DamageUpdate()
{
	static unsigned int cnt = 0;
	stopFlag = false;

	vel.y += Stage::Get().GetGravity();
	worldPos.y += vel.y;
	worldPos.y = std::min(worldPos.y, Stage::Get().GetGround());

	if (worldPos.y < Stage::Get().GetGround())
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
			attackFlag = false;
			if (cParam.hp > 0)
			{
				invincibleFlag = true;
				ChangeState(CharacterState::Neutral);
			}
			else
			{
				ChangeState(CharacterState::Death);
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
		if (INPUT.IsTrigger(Key::A))
		{
			stopFlag = false;
			cParam.hp = 3;
			ChangeState(CharacterState::Neutral);
		}
	}
}

// ��ԂƊ֐����o�C���h
void Player::InitFunc()
{
	func.clear();

	func[CharacterState::Neutral] = std::bind(&Player::NeutralUpdate, this);
	func[CharacterState::Walk]    = std::bind(&Player::WalkUpdate, this);
	func[CharacterState::Jump]    = std::bind(&Player::JumpUpdate, this);
	func[CharacterState::Fall]    = std::bind(&Player::FallUpdate, this);
	func[CharacterState::Dash]    = std::bind(&Player::DashUpdate, this);
	func[CharacterState::Attack1] = std::bind(&Player::FirstAttackUpdate, this);
	func[CharacterState::Attack2] = std::bind(&Player::SecondAttackUpdate, this);
	func[CharacterState::Attack3] = std::bind(&Player::ThirdAttackUpdate, this);
	func[CharacterState::Damage]  = std::bind(&Player::DamageUpdate, this);
	func[CharacterState::Death]   = std::bind(&Player::DeathUpdate, this);
}

// ���[�J�����W�擾
Vec2f Player::GetWorldPos() const
{
	return worldPos;
}
