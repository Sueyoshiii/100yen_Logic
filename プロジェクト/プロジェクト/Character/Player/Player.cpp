#include "Player.h"
#include "../../Stage/Stage.h"
#include <iostream>

// コンストラクタ
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dashFlag(false), attackFlag(false), attackCnt(0)
{
	this->lib = lib;
	this->cam = cam;

	LoadData("data/chara/player.info");
	LoadImage("img/Player/player.png");

	InitFunc();
	ChangeState(ST::Neutral);

	tex.size *= 2.0f;

	tex.pos = Vec2f(0.0f, 0.0f);
	worldPos = cam.lock()->Correction(tex.pos);

	speed   = 5.0f;
	dushPow = 10.0f;
	jumpPow = -40.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 2;

	knockBackRange = 4.0f;
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

	if (!jumpFlag && 
		worldPos.y < Stage::Get().GetGround() &&
		state != ST::Damage)
	{
		CheckFall();
	}

	UpdateLocalPos();

	InvicibleUpdate();
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
	CheckWalk();

	CheckJump();

	CheckDash();

	CheckFirstAttack();
}

// 歩行
void Player::WalkUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}
	else
	{
		ChangeState(ST::Neutral);
	}

	CheckJump();

	CheckDash();

	CheckFirstAttack();
}
void Player::CheckWalk()
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
		worldPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
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
		ChangeState(ST::Fall);
	}
}
void Player::CheckJump()
{
	if (!jumpFlag && In.IsTrigger(Key::Space))
	{
		jumpFlag = true;
		vel.y = jumpPow;
		ChangeState(ST::Jump);
	}
}

// 落下
void Player::FallUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}

	vel.y += Stage::Get().GetGravity();
	worldPos.y += vel.y;
	if (worldPos.y > Stage::Get().GetGround())
	{
		worldPos.y = Stage::Get().GetGround();
		ChangeState(ST::Neutral);
	}
}
void Player::CheckFall()
{
	ChangeState(ST::Fall);
}

// ダッシュ
void Player::DashUpdate()
{
	worldPos.x += vel.x;
	static unsigned int cnt = 0;
	if (++cnt > 15)
	{
		cnt = 0;
		dashFlag = false;
		stopFlag = false;
		ChangeState(ST::Neutral);
	}
}
void Player::CheckDash()
{
	if (!dashFlag && In.IsTrigger(Key::X))
	{
		dashFlag = true;
		stopFlag = true;
		attackCnt = 0;
		attackFlag = false;
		vel.x = turnFlag ? -dushPow : dushPow;
		ChangeState(ST::Dash);
	}
}

// 1段目の攻撃
void Player::FirstAttackUpdate()
{
	CheckNextAttack(15);
}
void Player::CheckFirstAttack()
{
	if (!attackFlag && In.IsTrigger(Key::Z))
	{
		attackFlag = true;
		attackCnt  = 0;
		ChangeState(ST::Attack1);
	}
}

// 2段目の攻撃
void Player::SecondAttackUpdate()
{
	CheckNextAttack(15);
}

// 3段目の攻撃
void Player::ThirdAttackUpdate()
{
	CheckNextAttack(25);
}

// 次の攻撃へ移る
void Player::CheckNextAttack(const unsigned int attackInterval)
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
			CheckDash();
		}
		if ((++attackCnt) > attackInterval)
		{
			attackCnt = 0;
			attackFlag = false;
			stopFlag = false;
			ChangeState(ST::Neutral);
		}
	}
	else
	{
		CheckDash();
	}
}

// 被ダメージ
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
			if (hp >= 0)
			{
				invincibleFlag = true;
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
	func[ST::Fall]    = std::bind(&Player::FallUpdate, this);
	func[ST::Dash]    = std::bind(&Player::DashUpdate, this);
	func[ST::Attack1] = std::bind(&Player::FirstAttackUpdate, this);
	func[ST::Attack2] = std::bind(&Player::SecondAttackUpdate, this);
	func[ST::Attack3] = std::bind(&Player::ThirdAttackUpdate, this);
	func[ST::Damage]  = std::bind(&Player::DamageUpdate, this);
	func[ST::Death]   = std::bind(&Player::DeathUpdate, this);
}

// ローカル座標取得
Vec2f Player::GetWorldPos() const
{
	return worldPos;
}
