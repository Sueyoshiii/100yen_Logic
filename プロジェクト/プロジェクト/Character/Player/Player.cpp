#include "Player.h"
#include "../../Stage/Stage.h"
#include "../CharaEffect/EffectManager.h"
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
	ChangeState(CharacterState::Neutral);

	tex.size *= 2.0f;

	tex.pos = Vec2f(0.0f, 0.0f);
	worldPos = cam.lock()->Correction(tex.pos);

	// hp, speed, attack, defense, dush, jump
	cParam = CharacterParameter(2, 5.0f, 2, 2, 10.0f, -40.0f);
	vel     = Vec2f(cParam.speed, 0.0f);

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
		state != CharacterState::Damage)
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
	if (INPUT.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (INPUT.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}
	else
	{
		ChangeState(CharacterState::Neutral);
	}

	CheckJump();

	CheckDash();

	CheckFirstAttack();
}
void Player::CheckWalk()
{
	if (INPUT.IsKey(Key::Num4) || INPUT.IsKey(Key::Num6))
	{
		vel.x = cParam.speed;
		ChangeState(CharacterState::Walk);
	}
}

// ジャンプ
void Player::JumpUpdate()
{
	if (INPUT.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (INPUT.IsKey(Key::Num6))
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
		ChangeState(CharacterState::Fall);
	}
}
void Player::CheckJump()
{
	if (!jumpFlag && INPUT.IsTrigger(Key::Space))
	{
		jumpFlag = true;
		vel.y = cParam.jumpPow;
		ChangeState(CharacterState::Jump);
	}
}

// 落下
void Player::FallUpdate()
{
	if (INPUT.IsKey(Key::Num4))
	{
		turnFlag = true;
		worldPos.x -= vel.x;
	}
	else if (INPUT.IsKey(Key::Num6))
	{
		turnFlag = false;
		worldPos.x += vel.x;
	}

	vel.y += Stage::Get().GetGravity();
	worldPos.y += vel.y;
	if (worldPos.y > Stage::Get().GetGround())
	{
		worldPos.y = Stage::Get().GetGround();
		ChangeState(CharacterState::Neutral);
	}
}
void Player::CheckFall()
{
	ChangeState(CharacterState::Fall);
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
		ChangeState(CharacterState::Neutral);
	}
}
void Player::CheckDash()
{
	if (!dashFlag && INPUT.IsTrigger(Key::X))
	{
		dashFlag = true;
		stopFlag = true;
		attackCnt = 0;
		attackFlag = false;
		vel.x = turnFlag ? -cParam.dushPow : cParam.dushPow;
		ChangeState(CharacterState::Dash);
	}
}

// 1段目の攻撃
void Player::FirstAttackUpdate()
{
	CheckNextAttack(15);
}
void Player::CheckFirstAttack()
{
	if (!attackFlag && INPUT.IsTrigger(Key::Z))
	{
		attackFlag = true;
		attackCnt  = 0;
		ChangeState(CharacterState::Attack1);
		EffectManager::Get().CreateSlash(state, tex.pos, tex.size, turnFlag);
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
		if (state != CharacterState::Attack3)
		{
			if (INPUT.IsTrigger(Key::Z))
			{
				attackCnt = 0;
				stopFlag = false;
				CharacterState next = CharacterState(int(state) + 1);
				ChangeState(next);
				EffectManager::Get().CreateSlash(next, tex.pos, tex.size, turnFlag);
			}
			CheckDash();
		}
		if ((++attackCnt) > attackInterval)
		{
			attackCnt = 0;
			attackFlag = false;
			stopFlag = false;
			ChangeState(CharacterState::Neutral);
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
			if (cParam.hp > 0)
			{
				invincibleFlag = true;
				ChangeState(CharacterState::Neutral);
			}
			else
			{
				ChangeState(CharacterState::Death);
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
		if (INPUT.IsTrigger(Key::A))
		{
			stopFlag = false;
			cParam.hp = 3;
			ChangeState(CharacterState::Neutral);
		}
	}
}

// 状態と関数をバインド
void Player::InitFunc()
{
	func.clear();

	func[CharacterState::Neutral] = std::bind(&Player::NeutralUpdate, this);
	func[CharacterState::Walk]    = std::bind(&Player::WalkUpdate, this);
	func[CharacterState::Jump]    = std::bind(&Player::JumpUpdate, this);
	func[CharacterState::Fall]    = std::bind(&Player::FallUpdate, this);
	func[CharacterState::Dash]    = std::bind(&Player::DashUpdate, this);
	func[CharacterState::Attack1] = std::bind(&Player::FirstAttackUpdate, this);
	func[CharacterState::Attack2] = std::bind(&Player::SecondAttackUpdate, this);
	func[CharacterState::Attack3] = std::bind(&Player::ThirdAttackUpdate, this);
	func[CharacterState::Damage]  = std::bind(&Player::DamageUpdate, this);
	func[CharacterState::Death]   = std::bind(&Player::DeathUpdate, this);
}

// ローカル座標取得
Vec2f Player::GetWorldPos() const
{
	return worldPos;
}
