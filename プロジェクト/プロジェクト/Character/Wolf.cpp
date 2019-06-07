#include "Wolf.h"

// コンストラクタ
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos) :
	discovery(false)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	LoadData("data/chara/enemy_1.info");
	LoadImage("img/Enemy_1.png");

	InitFunc();
	ChangeState(ST::Walk);

	tex.size *= 2.0f;

	speed   = 2.0f;
	dushPow = 10.0f;
	jumpPow = -30.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 2;

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
	if (state != ST::Death)
	{
		DrawViewRange();
	}
#endif
}

// 待機
void Wolf::NeutralUpdate()
{
	static unsigned int cnt = 0;
	if (oldState == ST::Walk || oldState == ST::Attack1)
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
			Walk();
		}
		++cnt;
	}
}
// 徘徊
void Wolf::Loitering()
{
	worldPos.x += vel.x;

	float size = tex.size.x * 2.0f;
	if (pl.lock()->GetState() != ST::Death)
	{
		if (fulcrum.x - size >= worldPos.x ||
			worldPos.x >= fulcrum.x + size)
		{
			oldState = state;
			ChangeState(ST::Neutral);
		}
	}
	else
	{
		if (fulcrum.x - size >= worldPos.x)
		{
			turnFlag = false;
			vel.x = speed;
		}
		else if (fulcrum.x + size <= worldPos.x)
		{
			turnFlag = true;
			vel.x = -speed;
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
			vel.x = -speed;
		}
		else
		{
			turnFlag = false;
			vel.x = speed;
		}
	}

	float dis = fabs(worldPos.x - pl.lock()->GetWorldPos().x);
	if (0.0f < dis && dis < 150)
	{
		Attack();
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
		discovery = pl.lock()->GetState() != ST::Death ? true : false;
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
void Wolf::Walk()
{
	ChangeState(ST::Walk);
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
			ChangeState(ST::Neutral);
		}
	}
	else
	{
		vel.x = worldPos.x > oldPlPos.x ? -speed * 2 : speed * 2;
		worldPos.x += vel.x;
	}

	/*
	
	水平方向
		速度：vX = v0cosθ
		変位：x  = v0cosθ*t
			  t  = x / (v0cosθ)
	鉛直方向
		速度：vY = v0sinθ-gt
		変位：y  = v0sinθ*t-1/2gt^2

	xをyの式に代入
		y = (v0sinθ) * x/(v0cosθ) - 1/2g (x/v0cosθ)^2
		y = xtanθ - (g/2v0^2cos^2θ)x^2

	*/
}
void Wolf::Attack()
{
	coolTime = 0;
	vel.y = jumpPow;
	oldPlPos = pl.lock()->GetWorldPos();
	ChangeState(ST::Attack1);
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
			if (hp >= 0)
			{
				ChangeState(ST::Walk);
			}
			else
			{
				ChangeState(ST::Death);
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
			tex.Delete("img/Enemy_1.png");
		}
		alpha -= 0.02f;
		//if (In.IsTrigger(Key::S))
		//{
		//	stopFlag = false;
		//	hp = 3;
		//	ChangeState(ST::Walk);
		//}
	}
}

// 状態初期化
void Wolf::InitFunc()
{
	func.clear();

	func[ST::Neutral] = std::bind(&Wolf::NeutralUpdate, this);
	func[ST::Walk]    = std::bind(&Wolf::WalkUpdate, this);
	func[ST::Attack1] = std::bind(&Wolf::AttackUpdate, this);
	func[ST::Damage]  = std::bind(&Wolf::DamageUpdate, this);
	func[ST::Death]   = std::bind(&Wolf::DeathUpdate, this);
}
