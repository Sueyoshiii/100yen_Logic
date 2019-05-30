#include "Enemy.h"
#include <iostream>

Enemy::Enemy() :
	viewBox(Primitive(PrimitiveType::box))
{
}

Enemy::~Enemy()
{
}

// Õ“Ë”»’è
void Enemy::CheckHit()
{
	for (auto& p : pl.lock()->GetRect())
	{
		for (auto& e : GetRect())
		{
			// UŒ‚‹éŒ`‚Æ“–‚½‚è‹éŒ`‚Å”»’è
			if ((p.type == HitType::Attack && e.type == HitType::Attack) ||
				(p.type == HitType::Damage && e.type == HitType::Damage))
			{
				continue;
			}

			// ‹éŒ`‚Ì’†S
			Vec2f plCenter = p.rect.pos + p.rect.size / 2.0f;
			Vec2f emCenter = e.rect.pos + e.rect.size / 2.0f;

			float dis1  = std::hypot(plCenter.x - emCenter.x, plCenter.y - emCenter.y);
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
				Vec2f dir = pl.lock()->GetPos() - GetPos();
				if (p.type == HitType::Attack)
				{
					SetTurnFlag(pl.lock()->GetTurnFlag() ? false : true);
					KnockBack(-dir);
					ChangeState(ST::Damage);
				}
				else
				{
					if (pl.lock()->GetInvincibleFlag())
					{
						return;
					}
					bool nextTurn = turnFlag;
					if (dir.x < 0)
					{
						nextTurn = false;
					}
					else if (dir.x > 0)
					{
						nextTurn = true;
					}
					pl.lock()->SetTurnFlag(nextTurn);
					pl.lock()->KnockBack(dir);
					pl.lock()->ChangeState(ST::Damage);
				}
			}
		}
	}
}

// ‹ŠE•`‰æ
void Enemy::DrawViewRange()
{
	Vec2f pos;
	Vec2f size;

	pos = Vec2f(tex.pos.x + tex.size.x / 2, tex.pos.y);
	size = viewRange;

	viewBox.pos[0] = Vec3f(Vec2f(pos));
	viewBox.pos[1] = Vec3f(Vec2f(pos.x + size.x, pos.y));
	viewBox.pos[2] = Vec3f(Vec2f(pos.x, pos.y + size.y));
	viewBox.pos[3] = Vec3f(Vec2f(pos + size));

	lib.lock()->Draw(viewBox, Vec3f(1.0f, 1.0f, 0.0f), 0.5f);
}
