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

// �Փ˔���
void Enemy::CheckHit()
{
	for (auto& p : pl.lock()->GetRect())
	{
		for (auto& e : GetRect())
		{
			// �U����`�Ɠ������`�Ŕ���
			if ((p.type == HitType::Attack && e.type == HitType::Attack) ||
				(p.type == HitType::Damage && e.type == HitType::Damage))
			{
				continue;
			}

			// ��`�̔����̃T�C�Y
			Vec2f plHalf = p.rect.size / 2.0f;
			Vec2f emHalf = e.rect.size / 2.0f;

			// ��`�̒��S
			Vec2f plCenter = p.rect.pos + plHalf;
			Vec2f emCenter = e.rect.pos + emHalf;

			// ���S�Ԃ̋����ƕӂ̒����Ŕ���
			if (std::fabs(plCenter.x - emCenter.x) < fabs(plHalf.x + emHalf.x) &&
				std::fabs(plCenter.y - emCenter.y) < fabs(plHalf.y + emHalf.y))
			{
				Vec2f dir = pl.lock()->GetPos() - GetPos();
				if (p.type == HitType::Attack)
				{
					SetTurnFlag(pl.lock()->GetTurnFlag() ? false : true);
					KnockBack(-dir);
					SetDamage(pl.lock()->GetParam().attackPow, cParam.defensePow);
					ChangeState(CharacterState::Damage);
					EffectManager::Get().Create(EffectType::Flower, tex.pos, pl);
				}
				else
				{
					if (!pl.lock()->GetInvincibleFlag())
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
						pl.lock()->ChangeState(CharacterState::Damage);
					}
				}
				break;
			}
		}
	}
}

// ���E����
bool Enemy::CheckView()
{
	auto p = pl.lock();

	float length = turnFlag ? -lib.lock()->GetWinSize().x / 3.0f : lib.lock()->GetWinSize().x / 3.0f;

	// ���E��
	seg = Segment();
	seg.begin = tex.pos + tex.size / 2.0f;
	seg.end   = Vec2f(seg.begin.x + (length), seg.begin.y);
	seg.vec   = seg.end - seg.begin;

	// ��`����
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

// �������m�̏Փ˔���
bool Enemy::CheckColSegments(const Segment& seg1, const Segment& seg2)
{
	Vec2f v = seg2.begin - seg1.begin;

	float cross_v1v2 = seg1.vec.x * seg2.vec.y - seg1.vec.y * seg2.vec.x;

	// ���s������
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

// ���E�`��
void Enemy::DrawViewRange()
{
	viewLine.pos[0] = Vec3f(seg.begin);
	viewLine.pos[1] = Vec3f(seg.end);
	lib.lock()->Draw(viewLine, Vec3f(1.0f, 1.0f, 0.0f), 0.5f);
}

// �폜�t���O�擾
bool Enemy::GetDeleteFlag() const
{
	return deleteFlag;
}
