#include "Player.h"
#include "../CharaEffect/EffectManager.h"
#include <iostream>

namespace {
	const unsigned int HP_MAX = 1;

	const float NORMAL_WALK_SPEED = 8.0f;
	const int NORMAL_ATTACK_POW = 2;
	const int NORMAL_DEFENCE_POW = 2;
	const float NORMAL_DASH_POW = 15.0f;
	const float NORMAL_JUMP_POW = -40.0f;

	const float WOLF_WALK_SPEED = 12.0f;
	const int WOLF_ATTACK_POW = 4;
	const int WOLF_DEFENCE_POW = 0;
	const float WOLF_DASH_POW = 15.0f;
	const float WOLF_JUMP_POW = -40.0f;

	const unsigned int HIT_STOP_CNT_MAX = 15;
	const unsigned int DISAPPEAR_CNT_MAX = 60;
	const unsigned int CRITICAL_CNT_MAX = 5;
}

// コンストラクタ
Player::Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	jumpFlag(false), dashFlag(false), attackFlag(false), attackCnt(0),
	hitFlag(false), hitStopCnt(0), disappearCnt(0), criticalAlpha(0.8f),
	deadEndFlag(false)
{
	this->lib = lib;
	this->cam = cam;

	type = CharacterType::PL_WOLF;
	LoadData("data/chara/player_wolf.info");
	LoadImage("img/Player/player_wolf.png");


	type = CharacterType::PL_NORMAL;
	LoadData("data/chara/player.info");
	LoadImage("img/Player/player.png");

	critical.Load("img/Player/damage_critical.png");
	critical.size.y *= 2.0f;

	InitFunc();
	ChangeState("Neutral");

	tex[type].size *= 3.0f;
	tex[CharacterType::PL_WOLF].size = tex[type].size;

	tex[type].pos = Vec2f(0.0f, 0.0f);
	worldPos = cam.lock()->Correction(tex[type].pos);
	worldPos.x -= float(lib.lock()->GetWinSize().x);
	worldPos.y = StageManager::Get().GetGround() - tex[type].size.y;

	firstPos = worldPos;

	// hp, speed, attack, defense, dush, jump
	cParam = CharacterParameter(HP_MAX, NORMAL_WALK_SPEED, NORMAL_ATTACK_POW, NORMAL_DEFENCE_POW, NORMAL_DASH_POW, NORMAL_JUMP_POW);
	vel = Vec2f(cParam.speed, 0.0f);

	knockBackRange = 4.0f;
}

// デストラクタ
Player::~Player()
{
}

// 更新
void Player::Update()
{
	if (CheckHitStop())
	{
		return;
	}

	// 状態関数
	func[state]();

	CorrectPosInStage();

	if (!jumpFlag && 
		GetFootPos().y < StageManager::Get().GetGround() &&
		state != "Damage")
	{
		CheckFall();
	}

	InvicibleUpdate();
}

// 描画
void Player::Draw()
{
	if (!hitFlag)
	{
		UpdateLocalPos();

		AnimationUpdate();
	}

	DamageDraw();

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

	if (INPUT.IsTrigger(Key::F))
	{
		if (type == CharacterType::PL_NORMAL)
		{
			ChangeWolf();
		}
		else
		{
			ChangeNormal();
		}
	}
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
		ChangeState("Neutral");
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
		ChangeState("Walk");
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
		vel.y += StageManager::Get().GetGravity();
		worldPos.y += vel.y;
	}
	else
	{
		jumpFlag = false;
		ChangeState("Fall");
	}
}
void Player::CheckJump()
{
	if (!jumpFlag && INPUT.IsTrigger(Key::Space))
	{
		jumpFlag = true;
		vel.y = cParam.jumpPow;
		ChangeState("Jump");
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

	vel.y += StageManager::Get().GetGravity();
	worldPos.y += vel.y;
	if (GetFootPos().y > StageManager::Get().GetGround())
	{
		worldPos.y = StageManager::Get().GetGround() - tex[type].size.y;
		ChangeState("Neutral");
	}
}
void Player::CheckFall()
{
	ChangeState("Fall");
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
		ChangeState("Neutral");
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
		ChangeState("Dash");
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
		ChangeState("Attack1");
		EffectManager::Get().CreateSlash(state, type, tex[type].pos, tex[type].size, turnFlag);
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
		if (state != "Attack3")
		{
			if (INPUT.IsTrigger(Key::Z))
			{
				attackCnt = 0;
				stopFlag = false;

				auto itr = state.rbegin();
				char& numStr = *itr;
				int num = atoi(&numStr);
				std::string next = "Attack" + std::to_string(++num);

				ChangeState(next);
				EffectManager::Get().CreateSlash(next, type, tex[type].pos, tex[type].size, turnFlag);
			}
			CheckDash();
		}
		if ((++attackCnt) > attackInterval)
		{
			attackCnt = 0;
			attackFlag = false;
			stopFlag = false;
			ChangeState("Neutral");
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

	vel.y += StageManager::Get().GetGravity();
	worldPos.y += vel.y;

	if (GetFootPos().y < StageManager::Get().GetGround())
	{
		worldPos.x += vel.x;
	}
	else
	{
		worldPos.y = StageManager::Get().GetGround() - tex[type].size.y;
		if ((++cnt) > 40)
		{
			cnt = 0;
			jumpFlag = false;
			dashFlag = false;
			attackFlag = false;
			if (cParam.hp > 0)
			{
				invincibleFlag = true;
				ChangeState("Neutral");
			}
			else
			{
				ChangeState("Death");
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
		if ((++disappearCnt) > DISAPPEAR_CNT_MAX)
		{
			alpha -= 0.03f;
		}
		if (alpha <= 0.0f)
		{
			deadEndFlag = true;
		}
	}
}

// ヒットストップ処理
bool Player::CheckHitStop()
{
	if (hitFlag)
	{
		if ((++hitStopCnt) > HIT_STOP_CNT_MAX)
		{
			hitStopCnt = 0;
			hitFlag = false;
		}
	}

	return hitFlag;
}

// ダメージ時の描画
void Player::DamageDraw()
{
	if (state == "Damage")
	{
		lib.lock()->Draw(critical, criticalAlpha);
		criticalAlpha -= 0.05f;
	}
	else
	{
		criticalAlpha = 0.8f;
	}
}

void Player::ChangeNormal()
{
	oldType = type;
	type = CharacterType::PL_NORMAL;
	
	tex[type].pos = tex[CharacterType::PL_WOLF].pos;

	cParam = CharacterParameter(cParam.hp, NORMAL_WALK_SPEED, NORMAL_ATTACK_POW, NORMAL_DEFENCE_POW, NORMAL_DASH_POW, NORMAL_JUMP_POW);

	ChangeState(state);
}

void Player::ChangeWolf()
{
	oldType = type;
	type = CharacterType::PL_WOLF;

	tex[type].pos = tex[CharacterType::PL_NORMAL].pos;

	cParam = CharacterParameter(cParam.hp, WOLF_WALK_SPEED, WOLF_ATTACK_POW, WOLF_DEFENCE_POW, WOLF_DASH_POW, WOLF_JUMP_POW);

	ChangeState(state);
}

// 状態と関数をバインド
void Player::InitFunc()
{
	func.clear();

	func["Neutral"] = std::bind(&Player::NeutralUpdate, this);
	func["Walk"]    = std::bind(&Player::WalkUpdate, this);
	func["Jump"]    = std::bind(&Player::JumpUpdate, this);
	func["Fall"]    = std::bind(&Player::FallUpdate, this);
	func["Dash"]    = std::bind(&Player::DashUpdate, this);
	func["Attack1"] = std::bind(&Player::FirstAttackUpdate, this);
	func["Attack2"] = std::bind(&Player::SecondAttackUpdate, this);
	func["Attack3"] = std::bind(&Player::ThirdAttackUpdate, this);
	func["Damage"]  = std::bind(&Player::DamageUpdate, this);
	func["Death"]   = std::bind(&Player::DeathUpdate, this);
}

// ローカル座標取得
Vec2f Player::GetWorldPos() const
{
	return worldPos;
}

// ヒットフラグ取得
bool Player::GetHitFlag()
{
	return hitFlag;
}

// 死亡フラグ取得
bool Player::GetDeadEndFlag() const
{
	return deadEndFlag;
}

// 初期座標取得
Vec2f Player::GetFirstPos() const
{
	return firstPos;
}

// ヒットフラグを設定
void Player::SetHitFlag(const bool flag)
{
	hitFlag = flag;
}

// 座標を設定
void Player::SetPos(const Vec2f& pos)
{
	worldPos = pos;
}
