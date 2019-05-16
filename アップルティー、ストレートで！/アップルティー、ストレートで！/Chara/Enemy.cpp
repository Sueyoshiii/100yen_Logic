#include "Enemy.h"
#include "../Camera/Camera.h"
#include "Player.h"
#include "../Application/Application.h"
#include <random>

// コンストラクタ
Enemy::Enemy() : 
	target(0.0f)
{
}

// デストラクタ
Enemy::~Enemy()
{
}

// ランダムなターゲット座標の生成
Vec2f Enemy::GetRandomPos(const Vec2f & range)
{
	std::random_device dev;
	std::mt19937 mt(dev());

	//ランダム範囲
	std::uniform_real_distribution<float> offsetX(0.0f, range.x);
	std::uniform_real_distribution<float> offsetY(0.0f, range.y);


	return Vec2f(offsetX(mt), offsetY(mt));
}

// あたり判定
void Enemy::CheckHit(void)
{
	for (auto& p : pl.lock()->GetRect())
	{
		for (auto& e : GetRect())
		{
			if (!(p.type == HitType::attack && e.type == HitType::damage
				|| p.type == HitType::damage && e.type == HitType::attack))
			{
				continue;
			}

			auto plCenter = p.rect.pos + p.rect.size / 2.0f;
			auto enCenter = e.rect.pos + e.rect.size / 2.0f;
			float dis1 = std::hypot(plCenter.x - enCenter.x, plCenter.y - enCenter.y);
			float angle = std::atan2(plCenter.x - enCenter.x, plCenter.y - enCenter.y);

			Vec2f pSize = { std::abs(p.rect.size.x), p.rect.size.y };
			pSize /= 2.0f;
			Vec2f eSize = { std::abs(e.rect.size.x), e.rect.size.y };
			eSize /= 2.0f;
			float dis2 = std::hypot((pSize.x + eSize.x) * sin(angle), (pSize.y + eSize.y) * cos(angle));
			if (dis1 <= dis2)
			{
				if (p.type == HitType::attack)
				{
					SetState("damage");
					if (pl.lock()->GetState() == "attack1")
					{
						KnockBack(0.7f);
					}
					else if (pl.lock()->GetState() == "attack2")
					{
						KnockBack(8.7f);

					}
					else
					{
						KnockBack(3.7f);
					}

					if (pl.lock()->GetReverse() == true)
					{
						SetReverse(true);
					}
					else
					{
						SetReverse(false);
					}
				}
				else
				{
					pl.lock()->SetState("damage");
				}

				return;
			}
		}
	}
}
