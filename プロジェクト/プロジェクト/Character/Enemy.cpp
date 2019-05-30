#include "Enemy.h"
#include "Player.h"
#include <iostream>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

// 衝突判定
void Enemy::CheckHit()
{
	for (auto& p : pl.lock()->GetRect())
	{
		for (auto& e : GetRect())
		{
			// 攻撃矩形と当たり矩形で判定
			if ((p.type == HitType::Attack && e.type == HitType::Attack) ||
				(p.type == HitType::Damage && e.type == HitType::Damage))
			{
				continue;
			}

			// 矩形の中心
			Vec2f plCenter = p.rect.pos + p.rect.size / 2.0f;
			Vec2f emCenter = e.rect.pos + e.rect.size / 2.0f;

			float dis1 = std::hypot(plCenter.x - emCenter.x, plCenter.y - emCenter.y);
			float angle = std::atan2(plCenter.x - emCenter.x, plCenter.y - emCenter.y);

			Vec2f pSize = { std::abs(p.rect.size.x), p.rect.size.y };
			pSize /= 2.0f;
			Vec2f eSize = { std::abs(e.rect.size.x), e.rect.size.y };
			eSize /= 2.0f;
			float dis2 = std::hypot((pSize.x + eSize.x) * sin(angle), (pSize.y + eSize.y) * cos(angle));
			if (dis1 <= dis2)
			//if (std::fabs(plCenter.x - emCenter.x) < p.rect.size.x / 2.0f + e.rect.size.x / 2.0f &&
			//	std::fabs(plCenter.y - emCenter.y) < p.rect.size.y / 2.0f + e.rect.size.y / 2.0f)
			{
				if (p.type == HitType::Attack)
				{
					// Enemyにダメージ
					//ChangeState(ST::Damage);
					std::cout << "Hit!" << std::endl;

					/// 攻撃されたらPlayerの方を向いてもいいかも？
					/// あとノックバックも入れないと
				}
				else
				{
					// Playerにダメージ
					//pl.lock()->ChangeState(ST::Damage);
					std::cout << "Damage!" << std::endl;
				}
			}
		}
	}
}
