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

	speed   = 2.0f;
	dushPow = 10.0f;
	jumpPow = -18.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 2;

	turnFlag = true;

	localPos = pos;

	knockBackRange = 4.0f;
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
#endif
}

// 待機
void Wolf::NeutralUpdate()
{
}

// 歩き
void Wolf::WalkUpdate()
{
}
void Wolf::Walk()
{
}

// 攻撃
void Wolf::AttackUpdate()
{
}
void Wolf::Attack()
{
}

// 被ダメージ
void Wolf::DamageUpdate()
{
	static unsigned int cnt = 0;
	if (tex.pos.y < Const::GROUND)
	{
		localPos.x += vel.x;
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
