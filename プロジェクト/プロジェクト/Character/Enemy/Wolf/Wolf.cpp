#include "Wolf.h"

namespace {
	const unsigned int	HP_MAX      = 2;
	const float			WALK_SPEED  = 2.0f;
	const float			DASH_SPEED  = 8.0f;
	const int			ATTACK_POW  = 2;
	const int			DEFENCE_POW = 2;
	const float			DASH_POW    = 10.0f;
	const float			JUMP_POW    = -30.0f;

	const float ATTACK_RANGE_MAX = 180.0f;
	const float ATTACK_RANGE_MIN = 0.0f;

	const unsigned int WAIT_TIME_MAX    = 2;
	const unsigned int HOWLING_TIME_MAX = 20;
	const unsigned int THREAT_TIME_MAX  = 10;
	const unsigned int COOL_TIME_MAX    = 120;
	const unsigned int STUN_TIME_MAX    = 40;

	const float KNOCK_BACK_RANGE = 8.0f;
}

// コンストラクタ
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos) :
	discovery(false), cnt(0), coolFlag(false), coolTime(0)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	type = CharacterType::EM_WEAK_WOLF;

	LoadData("data/chara/enemy_1.info");
	LoadImage("img/Enemy/Enemy_1.png");

	InitFunc();
	ChangeState("Neutral");

	tex[type].size *= 3.0f;
	tex[type].size.y *= 2.0f;

	// hp, speed, attack, defense, dash, jump
	cParam = CharacterParameter(HP_MAX, WALK_SPEED, ATTACK_POW, DEFENCE_POW, DASH_POW, JUMP_POW);

	vel = Vec2f(cParam.speed, 0.0f);

	turnFlag = true;

	tex[type].pos = pos;
	worldPos   = cam.lock()->Correction(tex[type].pos);
	worldPos.y = StageManager::Get().GetGround() - tex[type].size.y;
	fulcrum    = worldPos;

	knockBackRange = KNOCK_BACK_RANGE;
}

// デストラクタ
Wolf::~Wolf()
{
}

// 更新
void Wolf::Update()
{
	if (pl.lock()->GetHitFlag())
	{
		return;
	}

	func[state]();

	if (!jumpFlag && state != "Damage")
	{
		FallUpdate();
	}

	//CheckHit();
	//CheckHitEffect();
}

// 描画
void Wolf::Draw()
{
	if (!pl.lock()->GetHitFlag())
	{
		UpdateLocalPos();

		AnimationUpdate();
	}

	//DrawImage();

#ifdef _DEBUG
	DrawRect();
	if (CheckAlive())
	{
		DrawViewRange();
	}
#endif
}

// 待機
void Wolf::NeutralUpdate()
{
	static unsigned int cnt = 0;
	if (!CheckView())
	{
		if (!CheckAnimEnd())
		{
			return;
		}

		if (cnt >= WAIT_TIME_MAX)
		{
			cnt = 0;
			turnFlag = !turnFlag;
			CheckWalk();
		}
		++cnt;
	}
	else
	{
		CheckHowling();
	}
}

// 歩き
void Wolf::WalkUpdate()
{
	// 視界に捉えるまで徘徊
	if (CheckView())
	{
		CheckHowling();
	}
	else
	{
		vel.x = turnFlag ? -cParam.speed : cParam.speed;
		worldPos.x += vel.x;

		float size = tex[type].size.x * 2.0f;
		if (fulcrum.x - size >= worldPos.x ||
			worldPos.x >= fulcrum.x + size)
		{
			oldState = state;
			ChangeState("Neutral");
		}
	}
}
void Wolf::CheckWalk()
{
	ChangeState("Walk");
	cParam.speed = WALK_SPEED;
}

// 咆哮
void Wolf::HowlingUpdate()
{	
	if (cnt > HOWLING_TIME_MAX)
	{
		stopFlag = false;
		if (CheckAnimEnd())
		{
			CheckTreat();
		}
	}
	else
	{
		if (CheckAnimEnd(2))
		{
			stopFlag = true;
			++cnt;
		}
	}
}
void Wolf::CheckHowling()
{
	cnt = 0;
	ChangeState("Howling");
}

// 威嚇
void Wolf::ThreatUpdate()
{
	if (!coolFlag)
	{
		if (CheckAnimEnd())
		{
			stopFlag = true;
			++cnt;
		}
		if (cnt > THREAT_TIME_MAX)
		{
			stopFlag = false;
			CheckRun();
		}
	}
	else
	{
		turnFlag = worldPos.x > pl.lock()->GetWorldPos().x ? true : false;
		if ((++coolTime) > COOL_TIME_MAX)
		{
			coolFlag = false;
			coolTime = 0;
			CheckRun();
		}
	}
}
void Wolf::CheckTreat()
{
	cnt = 0;
	ChangeState("Threat");
}

// 追いかける
void Wolf::RunUpdate()
{
	if (worldPos.x > pl.lock()->GetWorldPos().x)
	{
		turnFlag = true;
		vel.x = -cParam.speed;
	}
	else
	{
		turnFlag = false;
		vel.x = cParam.speed;
	}

	float dis = fabs(worldPos.x - pl.lock()->GetWorldPos().x);
	if (ATTACK_RANGE_MIN < dis && dis < ATTACK_RANGE_MAX)
	{
		CheckSave();
	}
	else
	{
		worldPos.x += vel.x;
	}
}
void Wolf::CheckRun()
{
	cnt = 0;
	cParam.speed = DASH_SPEED;
	ChangeState("Run");
}

// 溜め
void Wolf::SaveUpdate()
{
	turnFlag = worldPos.x > pl.lock()->GetWorldPos().x ? true : false;

	if (CheckAnimEnd())
	{
		CheckAttack();
	}
}
void Wolf::CheckSave()
{
	cnt = 0;
	ChangeState("Save");
}

// 攻撃
void Wolf::AttackUpdate()
{
	worldPos.x += vel.x;
	vel.y += StageManager::Get().GetGravity();
	worldPos.y += vel.y;
	if (GetFootPos().y > StageManager::Get().GetGround())
	{
		worldPos.y = StageManager::Get().GetGround() - tex[type].size.y;
		jumpFlag = false;
		coolFlag = true;
		coolTime = 0;
		CheckTreat();
	}
}
void Wolf::CheckAttack()
{
	jumpFlag = true;
	oldPlPos = pl.lock()->GetWorldPos();
	vel.x    = worldPos.x > oldPlPos.x ? -cParam.speed : cParam.speed;
	vel.y    = cParam.jumpPow;
	ChangeState("Attack");
}

// 被ダメージ
void Wolf::DamageUpdate()
{
	static unsigned int cnt = 0;
	stopFlag = false;

	vel.y += StageManager::Get().GetGravity();
	worldPos.y += vel.y;

	if (GetFootPos().y < StageManager::Get().GetGround())
	{
		worldPos.x += vel.x;
	}
	else
	{
		worldPos.y = StageManager::Get().GetGround() - tex[type].size.y;
		if ((++cnt) > STUN_TIME_MAX)
		{
			cnt = 0;
			if (cParam.hp > 0)
			{
				CheckTreat();
			}
			else
			{
				ChangeState("Death");
			}
		}
	}
}

// 死亡
void Wolf::DeathUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (alpha < 0.0f)
		{
			deleteFlag = true;
		}
		alpha -= 0.02f;
	}
}

// 状態初期化
void Wolf::InitFunc()
{
	func.clear();

	func["Neutral"] = std::bind(&Wolf::NeutralUpdate, this);
	func["walk"]    = std::bind(&Wolf::WalkUpdate, this);
	func["Howling"] = std::bind(&Wolf::HowlingUpdate, this);
	func["Threat"]  = std::bind(&Wolf::ThreatUpdate, this);
	func["Run"]     = std::bind(&Wolf::RunUpdate, this);
	func["Save"]    = std::bind(&Wolf::SaveUpdate, this);
	func["Attack"]  = std::bind(&Wolf::AttackUpdate, this);
	func["Damage"]  = std::bind(&Wolf::DamageUpdate, this);
	func["Death"]   = std::bind(&Wolf::DeathUpdate, this);
}
