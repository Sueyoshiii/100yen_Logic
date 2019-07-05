#include "Wolf.h"

namespace {
	const float ATTACK_RANGE_MAX = 200.0f;
	const float ATTACK_RANGE_MIN = 0.0f;
}

// コンストラクタ
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos) :
	discovery(false), cnt(0), coolFlag(false), coolTime(0)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	LoadData("data/chara/enemy_1.info");
	LoadImage("img/Enemy/Enemy_1.png");

	InitFunc();
	ChangeState("Neutral");

	tex.size *= 3.0f;

	// hp, speed, attack, defense, dush, jump
	cParam = CharacterParameter(2, 2.0f, 2, 2, 10.0f, -30.0f);

	vel = Vec2f(cParam.speed, 0.0f);

	turnFlag = true;

	tex.pos    = pos;
	worldPos   = cam.lock()->Correction(tex.pos);
	worldPos.y = Stage::Get().GetGround() - tex.size.y;
	fulcrum    = worldPos;

	knockBackRange = 4.0f;

	viewRange = Vec2f(200.0f, 80.0f);
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
	CheckHitEffect();
}

// 描画
void Wolf::Draw()
{
	if (!pl.lock()->GetHitFlag())
	{
		UpdateLocalPos();

		AnimationUpdate();
	}

	DrawImage();

#ifdef _DEBUG
	DrawRect();
	if (state != "Death")
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

		if (cnt >= 2)
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

		float size = tex.size.x * 2.0f;
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
	ChangeState("walk");
	cParam.speed = 2.0f;
}

// 咆哮
void Wolf::HowlingUpdate()
{	
	if (cnt > 20)
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
		if (cnt > 10)
		{
			stopFlag = false;
			CheckRun();
		}
	}
	else
	{
		turnFlag = worldPos.x > pl.lock()->GetWorldPos().x ? true : false;
		if ((++coolTime) > 120)
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
	cParam.speed = 6.0f;
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
	vel.y += Stage::Get().GetGravity();
	worldPos.y += vel.y;
	if (GetFootPos().y > Stage::Get().GetGround())
	{
		worldPos.y = Stage::Get().GetGround() - tex.size.y;
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

	vel.y += Stage::Get().GetGravity();
	worldPos.y += vel.y;

	if (GetFootPos().y < Stage::Get().GetGround())
	{
		worldPos.x += vel.x;
	}
	else
	{
		worldPos.y = Stage::Get().GetGround() - tex.size.y;
		if ((++cnt) > 40)
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
	static unsigned int cnt = 0;
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if ((alpha < 0.0f) && (++cnt) > 10)
		{
			cnt = 0;
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
