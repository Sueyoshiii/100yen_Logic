#include "Enemy.h"
#include "Player.h"

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

			if (std::fabs(plCenter.x - emCenter.x) < p.rect.size.x / 2.0f + e.rect.size.x / 2.0f &&
				std::fabs(plCenter.y - emCenter.y) < p.rect.size.y / 2.0f + e.rect.size.y / 2.0f)
			{
				if (p.type == HitType::Attack)
				{
					// Enemyにダメージ
					ChangeState(ST::Damage);

					/// 攻撃されたらPlayerの方を向いてもいいかも？
					/// あとノックバックも入れないと
				}
				else
				{
					// Playerにダメージ
					pl.lock()->ChangeState(ST::Damage);
				}
			}
		}
	}
}
