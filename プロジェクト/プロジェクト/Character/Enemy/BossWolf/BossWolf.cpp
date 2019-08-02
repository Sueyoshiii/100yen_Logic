#include "BossWolf.h"
#include "../../CharaEffect/EffectManager.h"
#include <algorithm>

namespace {
	const unsigned int	HP_MAX = 20;
	const int			ATTACK_POW = 4;
	const int			DEFENCE_POW = 2;
	const float			JUMP_POW = -100.0f;

	const float ATTACK_RANGE_MAX = 180.0f;
	const float ATTACK_RANGE_MIN = 0.0f;

	const unsigned int ATTACK_CNT_MAX = 3;
	const unsigned int ATTACK_COOL_TIME_MAX = 5 * 60;
}

/*
	memo
	5•b‚²‚Æ‚ÉUŒ‚
	3‰ñUŒ‚‚µ‚½‚ç”½‘Î‘¤‚ÖƒWƒƒƒ“ƒv‚µ‚ÄˆÚ“®‚·‚é
*/

BossWolf::BossWolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos) :
	cnt(0), attackCnt(0)
{
	this->lib = lib;
	this->pl = pl;
	this->cam = cam;

	type = CharacterType::EM_BOSS_WOLF;

	LoadData("data/chara/boss.info");
	LoadImage("img/Boss/boss.png");

	InitFunc();
	ChangeState("Neutral");

	tex[type].size = Vec2f(64.0f) * 10;
	tex[type].size.y *= 2.0f;

	// hp, speed, attack, defense, dash, jump
	cParam = CharacterParameter(HP_MAX, 0.0f, ATTACK_POW, DEFENCE_POW, 0.0f, JUMP_POW);

	vel = Vec2f(cParam.speed, 0.0f);

	turnFlag = false;
	stageLeft = 0.0f;
	stageRight = 896;
	worldPos = Vec2f(stageRight, pos.y);

	alpha = 1.0f;
}

BossWolf::~BossWolf()
{
}

void BossWolf::Update()
{
	func[state]();

	float leftPosX  = worldPos.x + tex[type].size.x / 2.0f;
	float rightPosX = worldPos.x + tex[type].size.x - 1.0f;
	float topPosY   = worldPos.y + tex[type].size.y / 3.0f;
	float footPosY  = worldPos.y + tex[type].size.y - 1.0f;

	if (turnFlag)
	{
		leftPosX = worldPos.x;
		rightPosX = worldPos.x + tex[type].size.x / 2.0f;
	}

	if (state != "Jump")
	{
		if (!StageManager::Get().CheckMapChip(Vec2f(leftPosX, footPosY + 1.0f)) &&
			!StageManager::Get().CheckMapChip(Vec2f(rightPosX, footPosY + 1.0f)))
		{
			vel.y += StageManager::Get().GetGravity();
			worldPos.y += vel.y;
		}
		int chipSizeY = StageManager::Get().GetChipSize().y;
		if (StageManager::Get().CheckMapChip(Vec2f(leftPosX, footPosY)) ||
			StageManager::Get().CheckMapChip(Vec2f(rightPosX, footPosY)))
		{
			vel.y = 0;
			worldPos.y = int(worldPos.y) / chipSizeY * chipSizeY - 60;
		}
	}
	if (Input::Get().IsTrigger(Key::Q))
	{
		ChangeState("Death");
	}
}

void BossWolf::Draw()
{
	if (!pl.lock()->GetHitFlag())
	{
		UpdateLocalPos();

		AnimationUpdate();
	}

	DrawImage();

	static Primitive b(PrimitiveType::box);
	float topPosY = worldPos.y + tex[type].size.y / 3;
	if (turnFlag)
	{
		b.pos[0] = Vec3f(tex[type].pos.x, topPosY, 0);
		b.pos[1] = Vec3f(tex[type].pos.x + tex[type].size.x / 2.0f, topPosY, 0);
		b.pos[2] = Vec3f(tex[type].pos.x, tex[type].pos.y + tex[type].size.y, 0);
		b.pos[3] = Vec3f(tex[type].pos.x + tex[type].size.x / 2.0f, tex[type].pos.y + tex[type].size.y, 0);
	}
	else
	{
		b.pos[0] = Vec3f(tex[type].pos.x + tex[type].size.x / 2.0f, topPosY, 0);
		b.pos[1] = Vec3f(tex[type].pos.x + tex[type].size.x, topPosY, 0);
		b.pos[2] = Vec3f(tex[type].pos.x + tex[type].size.x / 2.0f, tex[type].pos.y + tex[type].size.y, 0);
		b.pos[3] = Vec3f(tex[type].pos.x + tex[type].size.x, tex[type].pos.y + tex[type].size.y, 0);
	}
	lib.lock()->Draw(b, Vec3f(1.0f, 0.0f, 0.0f), 0.5f);
}

// ‘Ò‹@
void BossWolf::NeutralUpdate()
{
	if (++cnt >= ATTACK_COOL_TIME_MAX)
	{
		cnt = 0;
		if (++attackCnt >= ATTACK_CNT_MAX)
		{
			attackCnt = 0;
			ChangeState("Jump");
		}
		else
		{
			CheckAttack();
		}
	}
}

// UŒ‚
void BossWolf::AttackUpdate()
{
	if (CheckAnimEnd())
	{
		ChangeState("Neutral");
	}
}

void BossWolf::CheckAttack()
{
	EffectManager::Get().CreateBossClaw(worldPos, tex[type].size, turnFlag);
	ChangeState("Attack");
}

// ƒWƒƒƒ“ƒv
void BossWolf::JumpUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		vel.y = cParam.jumpPow;
	}
	worldPos.y += vel.y;

	if (worldPos.y + tex[type].size.y * 2 < 0.0f)
	{
		vel.y = 0.0f;
		stopFlag = false;
		turnFlag = !turnFlag;
		stageLeft = StageManager::Get().GetChipSize().x * 2;
		stageRight = StageManager::Get().GetStageSize().x - StageManager::Get().GetChipSize().x * 2 - tex[type].size.x;
		worldPos.x = turnFlag ? stageLeft : stageRight;
		ChangeState("Neutral");
	}
}

// €–S
void BossWolf::DeadUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;

		alpha = std::max(alpha -= 0.02f, 0.0f);

		if (alpha <= 0.0f)
		{
			deleteFlag = true;
		}
	}
}

// Œ‹‡
void BossWolf::InitFunc()
{
	func["Neutral"] = std::bind(&BossWolf::NeutralUpdate, this);
	func["Attack"]  = std::bind(&BossWolf::AttackUpdate, this);
	func["Jump"]    = std::bind(&BossWolf::JumpUpdate, this);
	func["Death"]    = std::bind(&BossWolf::DeadUpdate, this);
}
