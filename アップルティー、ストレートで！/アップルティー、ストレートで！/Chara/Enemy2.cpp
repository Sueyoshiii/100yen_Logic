#include "Enemy2.h"
#include "../Camera/Camera.h"
#include "Player.h"
#include "../Application/Application.h"

// 移行オフセット
#define OFFSET 1.0f

// コンストラクタ
Enemy2::Enemy2(std::weak_ptr<Application> app, std::weak_ptr<Camera> cam, std::weak_ptr<Player>pl, const Vec2f& pos, const Vec2f& size)
{
	this->app	= app;
	this->cam	= cam;
	this->pl	= pl;
	this->pos	= pos;
	this->lpos	= pos;
	this->size	= size;

	hp = 10;

	LoadImg("1", "_rsc/img/Enemy2.png");
	LoadInfo("_rsc/info/enemy2.info");
	SetFunc();
}

// デストラクタ
Enemy2::~Enemy2()
{
	Delete();
}

// ステータス毎の処理のセット
void Enemy2::SetFunc(void) 
{
	func.clear();

	func["wait"]   = &Enemy2::Wait;
	func["walk"]   = &Enemy2::Walk;
	func["attack"] = &Enemy2::Attack;
	func["damage"] = &Enemy2::Damage;
	func["dead"]   = &Enemy2::Dead;
}

// 待機時の処理
void Enemy2::Wait(void) 
{
	if (state != "wait")
	{
		return;
	}

	target = pl.lock()->GetCenter();

	// 移動
	if (GetCenter().x > target.x
		|| GetCenter().x < target.x
		|| GetCenter().y > target.y
		|| GetCenter().y < target.y)
	{
		SetState("walk");
	}

	if (GetCenter().y >= pl.lock()->GetCenter().y
		&& GetCenter().y <= (pl.lock()->GetPos().y + pl.lock()->GetSize().y))
	{
		if (reverse)
		{
			if ((pos.x + size.x) <= target.x
				&& (pos.x + (size.x * 2)) >= target.x)
			{
				SetState("attack");
			}
		}
		else
		{
			if (pos.x >= target.x
				&& (pos.x - size.x) <= target.x)
			{
				SetState("attack");
			}
		}
	}
}

// 移動中の処理
void Enemy2::Walk(void) 
{
	if (state != "walk")
	{
		return;
	}
	
	if (GetCenter().x > target.x)
	{
		pos.x -= speed;
	}
	else if (GetCenter().x < target.x)
	{
		pos.x += speed;
	}

	if (GetCenter().y > target.y)
	{
		pos.y -= speed;
	}
	else if (GetCenter().y < target.y)
	{
		pos.y += speed;
	}

	target = pl.lock()->GetCenter();

	if (GetCenter().y >= target.y
		&& GetCenter().y <= (pl.lock()->GetPos().y + pl.lock()->GetSize().y))
	{
		if (reverse)
		{
			if ((pos.x + size.x) <= target.x
				&& (pos.x + (size.x * 2)) >= target.x)
			{
				SetState("attack");
			}
		}
		else
		{
			if (pos.x >= target.x
				&& (pos.x - size.x) <= target.x)
			{
				SetState("attack");
			}
		}
	}

	if (CheckAnimEnd())
	{
		SetState("wait");
	}
}

// 攻撃中の処理
void Enemy2::Attack(void) 
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
void Enemy2::Damage(void) 
{
	if (state != "damage")
	{
		return;
	}
}

// 死亡時の処理
void Enemy2::Dead(void) 
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
void Enemy2::Draw(void)
{
	DrawImg("1");

#if _DEBUG
	DrawRect();
#endif
}

// 処理
void Enemy2::UpData(void) 
{
	UpDataLocal();
	Animator();
	func[state](this);
	CheckHit();

	reverse = (target.x < GetCenter().x) ? false : true;
}