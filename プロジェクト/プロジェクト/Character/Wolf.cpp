#include "Wolf.h"

// コンストラクタ
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	LoadData("data/chara/enemy_1.info");
	LoadImage("img/Enemy_1.png");

	InitFunc();
	ChangeState(ST::Neutral);

	tex.size *= 2.0f;

	speed   = 2.0f;
	dushPow = 10.0f;
	jumpPow = -18.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 2;

	turnFlag = true;

	tex.pos = pos;
	worldPos = cam.lock()->Correction(tex.pos);

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
	DrawViewRange();
#endif
}

// 待機
void Wolf::NeutralUpdate()
{
	// 視界内に捉えたら移動
	if (CheckView())
	{
		Walk();
	}
}

// 歩き
void Wolf::WalkUpdate()
{
	static unsigned int cnt = 0;
	if ((++cnt) % 50 == 0)
	{
		if (pl.lock()->GetPos().x < tex.pos.x)
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
	worldPos.x += vel.x;

	static unsigned int trackingCnt = 0;
	if (++trackingCnt % (60 * 2) == 0)
	{
		// 何秒かに一度、追跡継続か否か判断
		if (!CheckView())
		{
			ChangeState(ST::Neutral);
		}
	}
}
void Wolf::Walk()
{
	ChangeState(ST::Walk);
}

// 攻撃
void Wolf::AttackUpdate()
{
	// 目標地点へジャンプ攻撃するんやよー
	static unsigned int cnt = 0;
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if ((++cnt) > 10)
		{
			cnt = 0;
			stopFlag = false;
			ChangeState(ST::Neutral);
		}
	}
}
void Wolf::Attack()
{
	vel.y = jumpPow;
	oldPlPos = pl.lock()->GetWorldPos();
	ChangeState(ST::Attack1);
}

// 被ダメージ
void Wolf::DamageUpdate()
{
	static unsigned int cnt = 0;
	if (worldPos.y < Const::GROUND)
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
				ChangeState(ST::Neutral);
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
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (In.IsTrigger(Key::S))
		{
			stopFlag = false;
			hp = 3;
			ChangeState(ST::Neutral);
		}
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
