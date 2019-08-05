#include "Player.h"
#include "../CharaEffect/EffectManager.h"
#include <iostream>

namespace {
	const unsigned int HP_MAX = 10;

	const float NORMAL_WALK_SPEED = 8.0f;
	const int NORMAL_ATTACK_POW   = 2;
	const int NORMAL_DEFENCE_POW  = 2;
	const float NORMAL_DASH_POW   = 15.0f;
	const float NORMAL_JUMP_POW   = -35.0f;

	const float WOLF_WALK_SPEED = 12.0f;
	const int WOLF_ATTACK_POW   = 4;
	const int WOLF_DEFENCE_POW  = 0;
	const float WOLF_DASH_POW   = 20.0f;
	const float WOLF_JUMP_POW   = -50.0f;

	const unsigned int HIT_STOP_CNT_MAX  = 15;
	const unsigned int DISAPPEAR_CNT_MAX = 60;
	const unsigned int CRITICAL_CNT_MAX  = 5;
}

// �R���X�g���N�^
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dashFlag(false), attackFlag(false), attackCnt(0),
	hitFlag(false), hitStopCnt(0), disappearCnt(0), criticalAlpha(0.8f),
	deadEndFlag(false)
{
	this->lib = lib;
	this->cam = cam;

	type = CharacterType::PL_WOLF;
	LoadData("data/chara/player_wolf.info");
	LoadImg("img/Player/player_wolf.png");

	type = CharacterType::PL_NORMAL;
	LoadData("data/chara/player.info");
	LoadImg("img/Player/player.png");

	critical.Load("img/Player/damage_critical.png");
	critical.size.y *= 2.0f;

	InitFunc();
	ChangeState("Neutral");

	tex[type].size *= 3.0f;
	tex[type].size = Vec2f(64.0f) * 2;
	tex[type].size.y *= 2.0f;
	tex[CharacterType::PL_WOLF].size = tex[type].size;

	tex[type].pos = Vec2f(0.0f, 0.0f);
	worldPos = Vec2f();

	firstPos = worldPos;

	// hp, speed, attack, defense, dush, jump
	cParam = CharacterParameter(HP_MAX, NORMAL_WALK_SPEED, NORMAL_ATTACK_POW, NORMAL_DEFENCE_POW, NORMAL_DASH_POW, NORMAL_JUMP_POW);
	vel = Vec2f();

	hpImg.resize(cParam.hp * 2);

	std::string filePath = "img/Life/Heart_";
	for (size_t i = 0; i < hpImg.size(); ++i)
	{
		std::string num = std::to_string(i / cParam.hp);
		hpImg[i].Load(filePath + num + ".png");
		hpImg[i].size.y *= 2.0f;
		hpImg[i].pos = {
			hpImg[i].pos.x + (i % cParam.hp) * hpImg[i].size.x + 20.0f,
			hpImg[i].pos.y + 30.0f
		};
	}

	knockBackRange = 4.0f;
}

// �f�X�g���N�^
Player::~Player()
{
}

// �X�V
void Player::Update()
{
	if (CheckHitStop())
	{
		return;
	}

	CorrectPosInStage();

	CheckFall();

	InvicibleUpdate();

	// ��Ԋ֐�
	func[state]();

	Vec2f tmpSize = {
		tex[type].size.x / 4,
		tex[type].size.y
	};

	int chipSizeX = StageManager::Get().GetChipSize().x;

	// ��
	if (StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tmpSize.x, worldPos.y)) ||
		StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tmpSize.x, worldPos.y + tex[type].size.y - 1.0f)))
	{
		vel.x = 0;
		worldPos.x = float(int(worldPos.x) / chipSizeX * chipSizeX + tmpSize.x);
	}

	// �E
	if (StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tex[type].size.x - tmpSize.x - 1.0f, worldPos.y)) ||
		StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tex[type].size.x - tmpSize.x - 1.0f, worldPos.y + tex[type].size.y - 1.0f)))
	{
		vel.x = 0;
		worldPos.x = float(int(worldPos.x) / chipSizeX * chipSizeX)/* + tmpSize.x*/;
	}

	firstPos.y = worldPos.y;
}

// �`��
void Player::Draw()
{
	if (!hitFlag)
	{
		UpdateLocalPos();

		AnimationUpdate();
	}

	DamageDraw();

	DrawImage();

	// HP�摜
	if (state != "Death")
	{
		for (size_t i = 0; i < cParam.hp + HP_MAX; ++i)
		{
			lib.lock()->Draw(hpImg[i]);
		}
	}

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

	CheckTransform();
}

// ���s
void Player::WalkUpdate()
{
	if (Input::Get().IsKey(Key::Num4))
	{
		turnFlag = true;
		vel.x = -cParam.speed;
	}
	else if (Input::Get().IsKey(Key::Num6))
	{
		turnFlag = false;
		vel.x = cParam.speed;
	}
	else
	{
		vel.x = 0.0f;
		ChangeState("Neutral");
	}
	worldPos.x += vel.x;

	CheckJump();

	CheckDash();

	CheckFirstAttack();

	CheckTransform();
}
void Player::CheckWalk()
{
	if (Input::Get().IsKey(Key::Num4))
	{
		vel.x = -cParam.speed;
		ChangeState("Walk");
	}
	else if (Input::Get().IsKey(Key::Num6))
	{
		vel.x = cParam.speed;
		ChangeState("Walk");
	}
}

// �W�����v
void Player::JumpUpdate()
{
	if (Input::Get().IsKey(Key::Num4))
	{
		turnFlag = true;
		vel.x = -cParam.speed;
	}
	else if (Input::Get().IsKey(Key::Num6))
	{
		turnFlag = false;
		vel.x = cParam.speed;
	}

	worldPos.x += vel.x;
	if (vel.y <= 0)
	{
		vel.y += StageManager::Get().GetGravity();
		worldPos.y += vel.y;
	}
	else
	{
		jumpFlag = false;
	}

	CheckJumpAttack();
}
void Player::CheckJump()
{
	if (!jumpFlag && Input::Get().IsTrigger(Key::Space))
	{
		if (Input::Get().IsKey(Key::Num4))
		{
			turnFlag = true;
			vel.x = -cParam.speed;
		}
		else if (Input::Get().IsKey(Key::Num6))
		{
			turnFlag = false;
			vel.x = cParam.speed;
		}
		else
		{
			vel.x = 0.0f;
		}

		jumpFlag = true;
		vel.y = cParam.jumpPow;
		ChangeState("Jump");
	}
}

// ����
void Player::FallUpdate()
{
	if (Input::Get().IsKey(Key::Num4))
	{
		turnFlag = true;
		vel.x = -cParam.speed;
	}
	else if (Input::Get().IsKey(Key::Num6))
	{
		turnFlag = false;
		vel.x = cParam.speed;
	}

	worldPos.x += vel.x;

	vel.y += StageManager::Get().GetGravity();
	worldPos.y += vel.y;
	Vec2f tmpSize = {
		tex[type].size.x / 4,
		tex[type].size.y
	};

	int chipSizeY = StageManager::Get().GetChipSize().y;
	if (StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tmpSize.x, worldPos.y + tex[type].size.y - 1.0f)) ||
		StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tex[type].size.x - tmpSize.x - 1.0f, worldPos.y + tex[type].size.y - 1.0f)))
	{
		vel.y = 0.0f;
		worldPos.y = float(int(worldPos.y) / chipSizeY * chipSizeY);
		ChangeState("Neutral");
	}

	CheckJumpAttack();

	// ���ɗ�����
	if (worldPos.x < lib.lock()->GetWinSize().x)
	{
		if (worldPos.y - tex[type].size.y * 3 > lib.lock()->GetWinSize().y * 2)
		{
			deadEndFlag = true;
		}
	}
}
void Player::CheckFall()
{
	Vec2f tmpSize = {
		tex[type].size.x / 4,
		tex[type].size.y
	};

	if (!StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tmpSize.x, worldPos.y + tex[type].size.y)) &&
		!StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tex[type].size.x - tmpSize.x, worldPos.y + tex[type].size.y)) &&
		!jumpFlag &&
		state != "Damage" &&
		state != "JumpAttack")
	{
		ChangeState("Fall");
	}
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
	if (!dashFlag && Input::Get().IsTrigger(Key::S))
	{
		dashFlag = true;
		stopFlag = true;
		attackCnt = 0;
		attackFlag = false;
		vel.x = turnFlag ? -cParam.dashPow : cParam.dashPow;
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
	if (!attackFlag && Input::Get().IsTrigger(Key::D))
	{
		attackFlag = true;
		attackCnt  = 0;
		ChangeState("Attack1");
		EffectManager::Get().CreateSlash(state, type, tex[type].pos, tex[type].size, turnFlag);
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

// 4�i�ڂ̍U��
void Player::FourthAttackUpdate()
{
	CheckNextAttack(25);
}

// ���̍U���ֈڂ�
void Player::CheckNextAttack(const unsigned int attackInterval)
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		std::string lastAttack = type == CharacterType::PL_NORMAL ? "Attack3" : "Attack4";

		if (state != lastAttack)
		{
			if (Input::Get().IsTrigger(Key::D))
			{
				attackCnt = 0;
				stopFlag = false;

				auto itr = state.rbegin();
				char& numStr = *itr;
				int num = atoi(&numStr);
				std::string next = "Attack" + std::to_string(++num);

				ChangeState(next);
				EffectManager::Get().CreateSlash(next, type, tex[type].pos, tex[type].size, turnFlag);
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

// �W�����v�U��
void Player::JumpAttackUpdate()
{
	if (!CheckAnimEnd())
	{
		if (Input::Get().IsKey(Key::Num4))
		{
			turnFlag = true;
			vel.x = -cParam.speed;
		}
		else if (Input::Get().IsKey(Key::Num6))
		{
			turnFlag = false;
			vel.x = cParam.speed;
		}

		worldPos.x += vel.x;

		vel.y += StageManager::Get().GetGravity();
		worldPos.y += vel.y;

		EffectManager::Get().SetPos(tex[type].pos);
	}
	else
	{
		if (!StageManager::Get().CheckMapChip(Vec2f(worldPos.x, worldPos.y + tex[type].size.y)) &&
			!StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tex[type].size.y - 1.0f, worldPos.y + tex[type].size.y)))
		{
			ChangeState("Fall");
		}
	}

	int chipSizeY = StageManager::Get().GetChipSize().y;
	if (StageManager::Get().CheckMapChip(Vec2f(worldPos.x, worldPos.y + tex[type].size.y - 1.0f)) ||
		StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tex[type].size.y - 1.0f, worldPos.y + tex[type].size.y - 1.0f)))
	{
		vel.y = 0.0f;
		worldPos.y = float(int(worldPos.y) / chipSizeY * chipSizeY);
		ChangeState("Neutral");
	}
}
void Player::CheckJumpAttack()
{
	if (Input::Get().IsTrigger(Key::D))
	{
		jumpFlag = false;
		ChangeState("JumpAttack");
		EffectManager::Get().CreateSlash(state, type, tex[type].pos, tex[type].size, turnFlag);
	}
}

// ��_���[�W
void Player::DamageUpdate()
{
	static unsigned int cnt = 0;
	stopFlag = false;

	vel.y += StageManager::Get().GetGravity();
	worldPos.y += vel.y;


	Vec2f tmpSize = {
		tex[type].size.x / 4,
		tex[type].size.y
	};
	static bool hitFlag = false;
	int chipSizeY = StageManager::Get().GetChipSize().y;
	if (StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tmpSize.x, worldPos.y + tex[type].size.y - 1.0f)) ||
		StageManager::Get().CheckMapChip(Vec2f(worldPos.x + tex[type].size.x - tmpSize.x - 1.0f, worldPos.y + tex[type].size.y - 1.0f)))
	{
		vel.y = 0.0f;
		worldPos.y = float(int(worldPos.y) / chipSizeY * chipSizeY);
		hitFlag = true;
	}

	if (hitFlag)
	{
		if ((++cnt) > 40)
		{
			cnt = 0;
			jumpFlag = false;
			dashFlag = false;
			attackFlag = false;
			hitFlag = false;
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
	else
	{
		worldPos.x += vel.x;
	}
}

// ���S
void Player::DeathUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if ((++disappearCnt) > DISAPPEAR_CNT_MAX)
		{
			alpha -= 0.03f;
		}
		if (alpha <= 0.0f)
		{
			deadEndFlag = true;
		}
	}
}

// �ϐg
void Player::TransformUpdate()
{
	if (CheckAnimEnd())
	{
		if (type == CharacterType::PL_NORMAL)
		{
			Transform(CharacterType::PL_WOLF, type, CharacterParameter(cParam.hp, WOLF_WALK_SPEED, WOLF_ATTACK_POW, WOLF_DEFENCE_POW, WOLF_DASH_POW, WOLF_JUMP_POW));
		}
		else
		{
			Transform(CharacterType::PL_NORMAL, type, CharacterParameter(cParam.hp, NORMAL_WALK_SPEED, NORMAL_ATTACK_POW, NORMAL_DEFENCE_POW, NORMAL_DASH_POW, NORMAL_JUMP_POW));
		}
	}
}
void Player::CheckTransform()
{
	if (Input::Get().IsTrigger(Key::A))
	{
		ChangeState("Change");
	}
}
void Player::Transform(const CharacterType& next, const CharacterType& now, const CharacterParameter& param)
{
	oldType = type;
	type = next;

	tex[type].pos = tex[now].pos;

	cParam = param;

	ChangeState("Neutral");
}

// �q�b�g�X�g�b�v����
bool Player::CheckHitStop()
{
	if (hitFlag)
	{
		if ((++hitStopCnt) > HIT_STOP_CNT_MAX)
		{
			hitStopCnt = 0;
			hitFlag = false;
		}
	}

	return hitFlag;
}

// �_���[�W���̕`��
void Player::DamageDraw()
{
	if (state == "Damage")
	{
		lib.lock()->Draw(critical, criticalAlpha);
		criticalAlpha -= 0.05f;
	}
	else
	{
		criticalAlpha = 0.8f;
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
	func["Attack4"] = std::bind(&Player::FourthAttackUpdate, this);
	func["JumpAttack"] = std::bind(&Player::JumpAttackUpdate, this);
	func["Damage"]  = std::bind(&Player::DamageUpdate, this);
	func["Death"]   = std::bind(&Player::DeathUpdate, this);
	func["Change"]  = std::bind(&Player::TransformUpdate, this);
}

// ���[�J�����W�擾
Vec2f Player::GetWorldPos() const
{
	return worldPos;
}

// �q�b�g�t���O�擾
bool Player::GetHitFlag()
{
	return hitFlag;
}

// ���S�t���O�擾
bool Player::GetDeadEndFlag() const
{
	return deadEndFlag;
}

// �������W�擾
Vec2f Player::GetFirstPos() const
{
	return firstPos;
}

// �q�b�g�t���O��ݒ�
void Player::SetHitFlag(const bool flag)
{
	hitFlag = flag;
}

// ���W��ݒ�
void Player::SetPos(const Vec2f& pos)
{
	worldPos = pos;
}
