#include "Wolf.h"

// コンストラクタ
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos) :
	discovery(false)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	LoadData("data/chara/enemy_1.info");
	LoadImage("img/Enemy/Enemy_1.png");

	InitFunc();
	ChangeState("Neutral");

	tex.size *= 2.0f;

	// hp, speed, attack, defense, dush, jump
	cParam = CharacterParameter(1, 2.0f, 2, 2, 10.0f, -30.0f);

	vel = Vec2f(cParam.speed, 0.0f);

	turnFlag = false;

	tex.pos = pos;
	worldPos = cam.lock()->Correction(tex.pos);
	fulcrum = worldPos;

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
	func[state]();

	FallUpdate();

	UpdateLocalPos();

	CheckHit();
}

// 描画
void Wolf::Draw()
{
	AnimationUpdate();

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
	if (oldState == "Walk" || oldState == "Attack1")
	{
		if (!CheckAnimEnd())
		{
			return;
		}

		if (cnt >= 2)
		{
			cnt = 0;
			turnFlag = !turnFlag;
			vel.x = -vel.x;
			CheckWalk();
		}
		++cnt;
	}
}
// 徘徊
void Wolf::Loitering()
{
	worldPos.x += vel.x;

	float size = tex.size.x * 2.0f;
	if (pl.lock()->GetState() != "Death")
	{
		if (fulcrum.x - size >= worldPos.x ||
			worldPos.x >= fulcrum.x + size)
		{
			oldState = state;
			ChangeState("Neutral");
		}
	}
	else
	{
		if (fulcrum.x - size >= worldPos.x)
		{
			turnFlag = false;
			vel.x = cParam.speed;
		}
		else if (fulcrum.x + size <= worldPos.x)
		{
			turnFlag = true;
			vel.x = -cParam.speed;
		}
	}
}
// 発見
void Wolf::Alert()
{
	static unsigned int cnt = 0;
	if ((++cnt) % 10 == 0)
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
	}

	float dis = fabs(worldPos.x - pl.lock()->GetWorldPos().x);
	if (0.0f < dis && dis < 150)
	{
		CheckAttack();
	}
	else
	{
		worldPos.x += vel.x;
	}
}

// 歩き
void Wolf::WalkUpdate()
{
	// 視界に捉えるまで徘徊
	if (CheckView())
	{
		discovery = pl.lock()->GetState() != "Death" ? true : false;
	}

	if (discovery)
	{
		Alert();
	}
	else
	{
		Loitering();
	}
}
void Wolf::CheckWalk()
{
	ChangeState("Walk");
}

// 威嚇
void Wolf::ThreatUpdate()
{
}

// 咆哮
void Wolf::HowlingUpdate()
{
}

// 追いかける
void Wolf::RunUpdate()
{
}

// 溜め
void Wolf::SaveUpdate()
{
}

// 攻撃
void Wolf::AttackUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if ((++coolTime) > 80)
		{
			stopFlag = false;
			ChangeState("Neutral");
		}
	}
	else
	{
		vel.x = worldPos.x > oldPlPos.x ? -cParam.speed * 2.0f : cParam.speed * 2.0f;
		worldPos.x += vel.x;
	}
}
void Wolf::CheckAttack()
{
	coolTime = 0;
	vel.y = cParam.jumpPow;
	oldPlPos = pl.lock()->GetWorldPos();
	ChangeState("Attack1");
}

// 被ダメージ
void Wolf::DamageUpdate()
{
	static unsigned int cnt = 0;
	stopFlag = false;
	if (worldPos.y < Stage::Get().GetGround())
	{
		worldPos.x += vel.x;
	}
	else
	{
		if ((++cnt) > 40)
		{
			cnt = 0;
			if (cParam.hp > 0)
			{
				ChangeState("Walk");
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
	func["Walk"]    = std::bind(&Wolf::WalkUpdate, this);
	func["Threat"]  = std::bind(&Wolf::ThreatUpdate, this);
	func["Howling"] = std::bind(&Wolf::HowlingUpdate, this);
	func["Run"]     = std::bind(&Wolf::RunUpdate, this);
	func["Save"]    = std::bind(&Wolf::SaveUpdate, this);
	func["Attack"]  = std::bind(&Wolf::AttackUpdate, this);
	func["Damage"]  = std::bind(&Wolf::DamageUpdate, this);
	func["Death"]   = std::bind(&Wolf::DeathUpdate, this);
}
