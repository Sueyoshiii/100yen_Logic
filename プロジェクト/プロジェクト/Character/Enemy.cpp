#include "Enemy.h"
#include <iostream>

Enemy::Enemy() :
	viewLine(Primitive(PrimitiveType::line))
{
}

Enemy::~Enemy()
{
}

// è’ìÀîªíË
void Enemy::CheckHit()
{
	for (auto& p : pl.lock()->GetRect())
	{
		for (auto& e : GetRect())
		{
			// çUåÇãÈå`Ç∆ìñÇΩÇËãÈå`Ç≈îªíË
			if ((p.type == HitType::Attack && e.type == HitType::Attack) ||
				(p.type == HitType::Damage && e.type == HitType::Damage))
			{
				continue;
			}

			// ãÈå`ÇÃíÜêS
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

// éãäEîªíË
void Enemy::CheckView()
{
	auto a = tex.pos;
	for (auto& p : pl.lock()->GetRect())
	{
		if (p.type == HitType::Attack)
		{
			continue;
		}

		float length = turnFlag ? -(lib.lock()->GetWinSize().x / 2.0f) : lib.lock()->GetWinSize().x / 2.0f;
		seg = Segment(tex.pos + tex.size / 2.0f, Vec2f(seg.begin.x + length, seg.begin.y));

		Segment pSeg[4];
		pSeg[0] = Segment(p.rect.pos, Vec2f(p.rect.pos.x + p.rect.size.x, p.rect.pos.y));
		pSeg[1] = Segment(Vec2f(p.rect.pos.x + p.rect.size.x, p.rect.pos.y), p.rect.pos + p.rect.size);
		pSeg[2] = Segment(p.rect.pos + p.rect.size, Vec2f(p.rect.pos.x, p.rect.pos.y + p.rect.size.y));
		pSeg[3] = Segment(Vec2f(p.rect.pos.x, p.rect.pos.y + p.rect.size.y), p.rect.pos);

		for (unsigned int i = 0; i < _countof(pSeg); ++i)
		{
			if (CheckColSegments(pSeg[i], seg))
			{
				std::cout << "Hit" << std::endl;
			}
		}
	}

}

// ê¸ï™ìØémÇÃè’ìÀîªíË
bool Enemy::CheckColSegments(const Segment& seg1, const Segment& seg2)
{
	Vec2f v = seg2.begin - seg1.begin;

	float cross_v1v2 = seg1.vec.x * seg2.vec.y - seg1.vec.y * seg2.vec.x;

	// ïΩçsÇ©îªíË
	if (cross_v1v2 == 0.0f)
	{
		return false;
	}

	float cross_vv1 = v.x * seg1.vec.y - v.y * seg1.vec.x;
	float cross_vv2 = v.x * seg2.vec.y - v.y * seg2.vec.x;

	float t1 = cross_vv2 / cross_v1v2;
	float t2 = cross_vv1 / cross_v1v2;

	if (0.0f < t1 && t1 < 1.0f && 0.0f < t2 && t2 < 1.0f)
	{
		return true;
	}

	return false;
}

// éãäEï`âÊ
void Enemy::DrawViewRange()
{
	viewLine.pos[0] = Vec3f(seg.begin);
	viewLine.pos[1] = Vec3f(seg.end);
	lib.lock()->Draw(viewLine, Vec3f(1.0f, 1.0f, 0.0f), 0.5f);
}
