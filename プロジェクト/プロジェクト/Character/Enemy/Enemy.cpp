#include "Enemy.h"
#include "../CharaEffect/EffectManager.h"
#include <iostream>

Enemy::Enemy() :
	viewLine(Primitive(PrimitiveType::line)), deleteFlag(false)
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
			// 攻撃矩形とダメージ矩形で判定
			if ((p.type == HitType::Attack && e.type == HitType::Attack) ||
				(p.type == HitType::Damage && e.type == HitType::Damage))
			{
				continue;
			}

			// 矩形情報
			Box pBox = Box(p.rect.pos, p.rect.size);
			Box eBox = Box(e.rect.pos, e.rect.size);

			if (CheckColBox(pBox, eBox))
			{
				Vec2f dir = pl.lock()->GetPos() - GetPos();
				if (p.type == HitType::Attack)
				{
					pl.lock()->SetHitFlag(true);
					SetTurnFlag(pl.lock()->GetTurnFlag() ? false : true);
					KnockBack(-dir);
					SetDamage(pl.lock()->GetParam().attackPow, cParam.defensePow);
					ChangeState("Damage");
					EffectManager::Get().CreateFlower(tex[type].pos, pl);
					cam.lock()->SetVibrationFlag(true);
				}
				else
				{
					if (!pl.lock()->GetInvincibleFlag() && state != "Damage")
					{
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
						pl.lock()->SetDamage(cParam.attackPow, pl.lock()->GetParam().defensePow);
						pl.lock()->ChangeState("Damage");
						cam.lock()->SetVibrationFlag(true);
					}
				}
				break;
			}
		}
	}
}

void Enemy::CheckHitEffect()
{
	auto list = EffectManager::Get().GetEffectList();

	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		// 花、削除済みは対象外
		if ((*itr).type == EffectType::Flower || (*itr).ptr->GetDeleteFlag())
		{
			continue;
		}

		for (auto& ef : (*itr).ptr->GetRect())
		{
			for (auto& em : GetRect())
			{
				// 敵のダメージ矩形と判定
				if (em.type == HitType::Attack)
				{
					continue;
				}

				// 矩形情報
				Box efBox = Box(ef.rect.pos, ef.rect.size);
				Box emBox = Box(em.rect.pos, em.rect.size);

				if (CheckColBox(efBox, emBox))
				{
					pl.lock()->SetHitFlag(true);
					Vec2f dir = pl.lock()->GetPos() - GetPos();
					SetTurnFlag(pl.lock()->GetTurnFlag() ? false : true);
					KnockBack(-dir);
					SetDamage(pl.lock()->GetParam().attackPow, cParam.defensePow);
					ChangeState("Damage");
					EffectManager::Get().CreateFlower(tex[type].pos, pl);
					cam.lock()->SetVibrationFlag(true);

					break;
				}
			}
		}
	}
}

// 視界判定
bool Enemy::CheckView()
{
	auto p = pl.lock();

	if (p->GetState() == "Death")
	{
		return false;
	}

	float length = turnFlag ? -lib.lock()->GetWinSize().x / 3.0f : lib.lock()->GetWinSize().x / 3.0f;

	// 視界線
	seg = Segment();
	seg.begin = tex[type].pos + tex[type].size / 2.0f;
	seg.end   = Vec2f(seg.begin.x + (length), seg.begin.y);
	seg.vec   = seg.end - seg.begin;

	// 矩形線分
	pSeg[0] = Segment(p->GetPos(), Vec2f(p->GetPos().x + p->GetSize().x, p->GetPos().y));
	pSeg[1] = Segment(Vec2f(p->GetPos().x + p->GetSize().x, p->GetPos().y), p->GetPos() + p->GetSize());
	pSeg[2] = Segment(p->GetPos() + p->GetSize(), Vec2f(p->GetPos().x, p->GetPos().y + p->GetSize().y));
	pSeg[3] = Segment(Vec2f(p->GetPos().x, p->GetPos().y + p->GetSize().y), p->GetPos());

	for (auto& s : pSeg)
	{
		if (CheckColSegments(s, seg))
		{
			return true;
		}
	}

	return false;
}

// 矩形同士の衝突判定
bool Enemy::CheckColBox(const Box& box1, const Box& box2)
{
	if (fabs(box1.centor.x - box2.centor.x) < fabs(box1.half.x + box2.half.x) &&
		fabs(box1.centor.y - box2.centor.y) < fabs(box1.half.y + box2.half.y))
	{
		return true;
	}

	return false;
}

// 線分同士の衝突判定
bool Enemy::CheckColSegments(const Segment& seg1, const Segment& seg2)
{
	Vec2f v = seg2.begin - seg1.begin;

	float cross_v1v2 = seg1.vec.x * seg2.vec.y - seg1.vec.y * seg2.vec.x;

	// 平行か判定
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

// 視界描画
void Enemy::DrawViewRange()
{
	viewLine.pos[0] = Vec3f(seg.begin);
	viewLine.pos[1] = Vec3f(seg.end);
	lib.lock()->Draw(viewLine, Vec3f(1.0f, 1.0f, 0.0f), 0.5f);
}

// 削除フラグ取得
bool Enemy::GetDeleteFlag() const
{
	return deleteFlag;
}
