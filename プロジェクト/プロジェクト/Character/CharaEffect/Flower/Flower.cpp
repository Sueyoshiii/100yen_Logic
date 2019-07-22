#include "Flower.h"
#include "../../Player/Player.h"
#include "../../../Stage/StageManager.h"
#include <random>

// コンストラクタ
Flower::Flower(const Vec2f& pos, std::weak_ptr<Player> pl)
{
	this->pl = pl;

	path.push_back("img/Item/itemB.png");
	path.push_back("img/Item/itemR.png");
	path.push_back("img/Item/itemY.png");

	texs.resize(3);
	unsigned int index = 0;
	for (auto& t : texs)
	{
		t.Load(path[index]);
		t.pos  = pos;
		t.size = Vec2f(64.0f, 64.0f);
		++index;
	}

	cParam.speed = 8.0f;
	vel = Vec2f(3.0f, -10.0f);

	st = Status::Drop;
	stFunc[Status::Drop]    = std::bind(&Flower::DropUpdate, this);
	stFunc[Status::Suction] = std::bind(&Flower::SuctionUpdate, this);
}

// デストラクタ
Flower::~Flower()
{
}

// 更新
void Flower::Update()
{
	if (texs.size() == 0)
	{
		return;
	}

	if (pl.lock()->GetHitFlag())
	{
		return;
	}

	stFunc[st]();
}

// 描画
void Flower::Draw()
{
}
void Flower::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto& t : texs)
	{
		lib.lock()->Draw(t, alpha);
	}
}

void Flower::SetPos(const Vec2f& pos)
{
	tex[type].pos = pos;
}

// ドロップ
void Flower::DropUpdate()
{
	texs[0].pos.x -= vel.x;
	texs[1].pos.x += vel.x;

	vel.y += StageManager::Get().GetGravity();
	for (auto& t : texs)
	{
		t.pos.y += vel.y;
	}

	if (texs[0].pos.y > StageManager::Get().GetGround() + texs[0].size.y &&
		texs[1].pos.y > StageManager::Get().GetGround() + texs[1].size.y &&
		texs[2].pos.y > StageManager::Get().GetGround() + texs[2].size.y)
	{
		st = Status::Suction;
	}
}

// 吸収
void Flower::SuctionUpdate()
{
	Vec2f pHalf = pl.lock()->GetSize() / 4.0f;
	Vec2f pCenter = pl.lock()->GetPos() + pHalf;

	for (auto& t : texs)
	{
		Vec2f half = t.size / 4.0f;
		Vec2f center = t.pos + half;
		Vec2f dis = pCenter - center;

		Vec2f vec = Vec2f(dis.x / fabs(dis.x), dis.y / fabs(dis.y));
		t.pos += vec * cParam.speed;

		if (fabs(dis.x) < fabs(pHalf.x + half.x) &&
			fabs(dis.y) < fabs(pHalf.y + half.y))
		{
			alpha = 0.0f;
			deleteFlag = true;
		}
	}
}
