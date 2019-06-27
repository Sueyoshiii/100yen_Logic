#include "Player.h"
#include "../../Stage/Stage.h"
#include "../CharaEffect/EffectManager.h"
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
	ChangeState("Neutral");

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
		state != "Damage")
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
		ChangeState("Neutral");
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
		ChangeState("Walk");
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
		ChangeState("Fall");
	}
}
void Player::CheckJump()
{
	if (!jumpFlag && INPUT.IsTrigger(Key::Space))
	{
		jumpFlag = true;
		vel.y = cParam.jumpPow;
		ChangeState("Jump");
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
		ChangeState("Neutral");
	}
}
void Player::CheckFall()
{
	ChangeState("Fall");
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
		ChangeState("Neutral");
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
		ChangeState("Dash");
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
		ChangeState("Attack1");
		EffectManager::Get().CreateSlash(state, tex.pos, tex.size, turnFlag);
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
		if (state != "Attack3")
		{
			if (INPUT.IsTrigger(Key::Z))
			{
				attackCnt = 0;
				stopFlag = false;
				auto itr = state.rbegin();
				char& numStr = *itr;
				int num = atoi(&numStr);
				++num;
				std::string next = "Attack" + std::to_string(num);
				ChangeState(next);
				//EffectManager::Get().CreateSlash(next, tex.pos, tex.size, turnFlag);
			}
			CheckDash();
		}
		if ((++attackCnt) > attackInterval)
		{
			attackCnt = 0;
			attackFlag = false;
			stopFlag = false;
			ChangeState("Neutral");
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
				ChangeState("Neutral");
			}
			else
			{
				ChangeState("Death");
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
			ChangeState("Neutral");
		}
	}
}

// ��ԂƊ֐����o�C���h
void Player::InitFunc()
{
	func.clear();

	func["Neutral"] = std::bind(&Player::NeutralUpdate, this);
	func["Walk"]    = std::bind(&Player::WalkUpdate, this);
	func["Jump"]    = std::bind(&Player::JumpUpdate, this);
	func["Fall"]    = std::bind(&Player::FallUpdate, this);
	func["Dash"]    = std::bind(&Player::DashUpdate, this);
	func["Attack1"] = std::bind(&Player::FirstAttackUpdate, this);
	func["Attack2"] = std::bind(&Player::SecondAttackUpdate, this);
	func["Attack3"] = std::bind(&Player::ThirdAttackUpdate, this);
	func["Damage"]  = std::bind(&Player::DamageUpdate, this);
	func["Death"]   = std::bind(&Player::DeathUpdate, this);
}

// ���[�J�����W�擾
Vec2f Player::GetWorldPos() const
{
	return worldPos;
}
