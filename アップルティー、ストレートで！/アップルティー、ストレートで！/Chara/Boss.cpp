#include "Boss.h"
#include "../Camera/Camera.h"
#include "Player.h"
#include "../Application/Application.h"



Boss::Boss(std::weak_ptr<Application>app, std::weak_ptr<Camera> cam, std::weak_ptr<Player>pl,const Vec2f& pos, const Vec2f& size)
{
	this->app	= app;
	this->cam	= cam;
	this->pl	= pl;
	this->pos	= pos;
	this->lpos	= pos;
	this->size	= size;

	hp = 30;

	LoadImg("3", "_rsc/img/ボス待機_export0000.png");
	//LoadInfo()
	SetFunc();
}


Boss::~Boss()
{
	Delete();
}


void Boss::SetFunc(void)
{
	func.clear();

	func["wait"]		= &Boss::Wait;
	func["walk"]		= &Boss::Walk;
	func["attack1"]		= &Boss::Attack1;
	func["attack2"]		= &Boss::Attack2;
	func["attack3"]		= &Boss::Attack3;
	func["damage"]		= &Boss::Damage;
	func["bigdamage"]	= &Boss::BigDamage;
	func["dead"]		= &Boss::Dead;
}

void Boss::Wait(void)
{
	if (state != "wait")
	{
		return;
	}

	////プレイヤーとの距離
	//Vec2f tmp = pl.lock()->GetCenter() - GetCenter();
	//float distance = std::hypot(tmp.x, tmp.y);

	//if (distance <= size.x / 2)
	//{
	//	SetState("attack");
	//}
	//else
	//{
	//	if (!CheckAnimEnd())
	//	{
	//		return;
	//	}
	//	target = pl.lock()->GetCenter();
	//	SetState("walk");
	//}
}

void Boss::Walk(void)
{
}

void Boss::Attack1(void)
{
}

void Boss::Attack2(void)
{
}

void Boss::Attack3(void)
{
}

void Boss::Damage(void)
{
}

void Boss::BigDamage(void)
{
}

void Boss::Dead(void)
{
}

void Boss::Draw(void)
{
	DrawImg("3");

#if _DEBUG
	DrawRect();
#endif
}

void Boss::UpData(void)
{
	UpDataLocal();
	Animator();
	func[state](this);
	CheckHit();

	reverse = (target.x < GetCenter().x) ? false : true;
}