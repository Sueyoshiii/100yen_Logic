#include "Enemy1.h"
#include "../Camera/Camera.h"
#include "Player.h"
#include "../Application/Application.h"
#include "../InfoLoader/InfoLoader.h"
#include "../Typedef.h"

// 移行オフセット
#define OFFSET 1.0f

// コンストラクタ
Enemy1::Enemy1(std::weak_ptr<Application> app, std::weak_ptr<Camera> cam, std::weak_ptr<Player> pl, const Vec2f & pos, const Vec2f & size)
{
	this->app  = app;
	this->cam  = cam;
	this->pl   = pl;
	this->pos  = pos;
	this->lpos = pos;
	this->size = size;

	hp = 3;

	LoadImg("1", "_rsc/img/Enemy1.png");
	LoadInfo("_rsc/info/enemy1.info");
	SetFunc();
}

// デストラクタ
Enemy1::~Enemy1()
{
	Delete();
}

// ステータス毎の処理のセット
void Enemy1::SetFunc(void)
{
	func.clear();

	func["wait"]   = &Enemy1::Wait;
	func["walk"]   = &Enemy1::Walk;
	func["attack"] = &Enemy1::Attack;
	func["damage"] = &Enemy1::Damage;
	func["dead"]   = &Enemy1::Dead;
}

// 待機時の処理
void Enemy1::Wait(void)
{
	if (state != "wait")
	{
		return;
	}

	//プレイヤーとの距離
	Vec2f tmp = pl.lock()->GetCenter() - GetCenter();
	float distance = std::hypot(tmp.x, tmp.y);

	if (distance <= size.x / 2.0f)
	{
		SetState("attack");
	}
	else
	{
		if (!CheckAnimEnd())
		{
			return;
		}
		target = pl.lock()->GetCenter();
		SetState("walk");
	}
}

// 移動時の処理
void Enemy1::Walk(void)
{
	if (state != "walk")
	{
		return;
	}

	if (target.x > GetCenter().x)
	{
		pos.x += speed;
	}
	else if(target.x < GetCenter().x)
	{
		pos.x -= speed;
	}

	if (target.y > GetCenter().y)
	{
		pos.y += speed;
	}
	else if(target.y < GetCenter().y)
	{
		pos.y -= speed;
	}

	Vec2f tmp = pl.lock()->GetCenter() - GetCenter();
	float distance = std::hypot(tmp.x, tmp.y);
	if (distance <= size.x / 2.0f)
	{
		SetState("attack");
	}
	else
	{
		tmp = target - GetCenter();
		distance = std::hypot(tmp.x, tmp.y);
		if (distance <= OFFSET)
		{
			SetState("wait");
		}
	}
}

// 攻撃時の処理
void Enemy1::Attack(void)
{
	if (state != "attack")
	{
		return;
	}

	if (CheckAnimEnd())
	{
		SetState("wait");
	}
}

// ダメージ時の処理
void Enemy1::Damage(void)
{
	if (state != "damage")
	{
		return;
	}	
	KnockBack(0);


	if (hp <= 0)
	{
		SetState("dead");
		return;
	}
	if (CheckAnimEnd())
	{
		SetState("wait");
	}
}

// 死亡時の処理
void Enemy1::Dead(void)
{
	if (state != "dead")
	{
		return;
	}

	if (CheckAnimEnd())
	{
		dead = true;
	}
}

// 描画
void Enemy1::Draw(void)
{
	DrawImg("1");

#if _DEBUG
	DrawRect();
#endif
}

// 処理
void Enemy1::UpData(void)
{
	UpDataLocal();
	Animator();
	func[state](this);
	CheckHit();

	reverse = (target.x < GetCenter().x) ? false : true;
}
