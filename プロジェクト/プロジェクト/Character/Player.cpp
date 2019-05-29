#include "Player.h"
#include "../Stage/Stage.h"
#include <iostream>

// コンストラクタ
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dashFlag(false), attackFlag(false), attackCnt(0)
{
	this->lib = lib;
	this->cam = cam;

	LoadData("data/chara/プレイヤー矩形新作/player.info");
	LoadImage("img/player.png");

	InitFunc();
	ChangeState(ST::Neutral);

	speed = 4.0f;
	dushPow = 10.0f;
	jumpPow = -18.0f;
	vel = Vec2f(speed, 0.0f);

	hp = 3;
}

// デストラクタ
Player::~Player()
{
}

// 更新
void Player::Update()
{
	// 状態関数
	func[state]();

	CorrectPosInStage();

	FallUpdate();

	UpdateLocalPos();
}

// 描画
void Player::Draw()
{
	AnimationUpdate();

	DrawImage();

#ifdef _DEBUG
	DrawRect();
#endif
}

// 待機
void Player::NeutralUpdate()
{
	if (localPos.y < Const::GROUND)
	{
		jumpFlag = true;
		ChangeState(ST::Jump);
	}

	Walk();

	Jump();

	Dash();

	Attack1();
}

// 歩行
void Player::WalkUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		localPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		localPos.x += vel.x;
	}
	else
	{
		ChangeState(ST::Neutral);
	}

	Jump();

	Dash();

	Attack1();
}
void Player::Walk()
{
	if (In.IsKey(Key::Num4) || In.IsKey(Key::Num6))
	{
		vel.x = speed;
		ChangeState(ST::Walk);
	}
}

// ジャンプ
void Player::JumpUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		localPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		localPos.x += vel.x;
	}

	if (tex.pos.y >= Const::GROUND)
	{
		jumpFlag = false;
		ChangeState(ST::Neutral);
	}
}
void Player::Jump()
{
	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		jumpFlag = true;
		vel.y = jumpPow;
		ChangeState(ST::Jump);
	}
}

// ダッシュ
void Player::DashUpdate()
{
	localPos.x += vel.x;
	if (CheckAnimEnd())
	{
		dashFlag = false;
		ChangeState(ST::Neutral);
	}
}
void Player::Dash()
{
	if (!dashFlag && In.IsTrigger(Key::X))
	{
		dashFlag = true;
		vel.x = turnFlag ? -dushPow : dushPow;
		ChangeState(ST::Dash);
	}
}

// 攻撃1
void Player::Attack1Update()
{
	NextAttack(20);
}
void Player::Attack1()
{
	if (!attackFlag && In.IsTrigger(Key::Z))
	{
		attackFlag = true;
		attackCnt  = 0;
		ChangeState(ST::Attack1);
	}
}

// 攻撃2
void Player::Attack2Update()
{
	NextAttack(20);
}

// 攻撃3
void Player::Attack3Update()
{
	NextAttack(25);
}

// 次の攻撃へ移る
void Player::NextAttack(const unsigned int attackInterval)
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (state != ST::Attack3)
		{
			if (In.IsTrigger(Key::Z))
			{
				attackCnt = 0;
				stopFlag = false;
				ChangeState(ST(int(state) + 1));
			}
		}
		if ((++attackCnt) > attackInterval)
		{
			attackCnt = 0;
			attackFlag = false;
			stopFlag = false;
			ChangeState(ST::Neutral);
		}
	}
}

// ダメージ
void Player::DamageUpdate()
{
	static unsigned int cnt = 0;
	if (tex.pos.y < Const::GROUND)
	{
		localPos.x += vel.x;
	}

	if ((++cnt) > 60)
	{
		cnt = 0;
		jumpFlag = false;
		dashFlag = false;
		attackFlag = 0;
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

// 死亡
void Player::DeathUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (In.IsTrigger(Key::A))
		{
			stopFlag = false;
			hp = 3;
			ChangeState(ST::Neutral);
		}
	}
}

// 状態と関数をバインド
void Player::InitFunc()
{
	func.clear();

	func[ST::Neutral] = std::bind(&Player::NeutralUpdate, this);
	func[ST::Walk]    = std::bind(&Player::WalkUpdate, this);
	func[ST::Jump]    = std::bind(&Player::JumpUpdate, this);
	func[ST::Dash]    = std::bind(&Player::DashUpdate, this);
	func[ST::Attack1] = std::bind(&Player::Attack1Update, this);
	func[ST::Attack2] = std::bind(&Player::Attack2Update, this);
	func[ST::Attack3] = std::bind(&Player::Attack3Update, this);
	func[ST::Damage]  = std::bind(&Player::DamageUpdate, this);
	func[ST::Death]   = std::bind(&Player::DeathUpdate, this);
}

Vec2f Player::GetLocalPos() const
{
	return localPos;
}
