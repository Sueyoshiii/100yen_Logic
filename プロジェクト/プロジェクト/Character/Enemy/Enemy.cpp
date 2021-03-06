#include "Enemy.h"
#include "../CharaEffect/EffectManager.h"
#include <iostream>
#include "../../Okdio/Okdio.h"
#pragma comment (lib, "Okdio.lib")


Enemy::Enemy() :
	viewLine(Primitive(PrimitiveType::line)), deleteFlag(false)
{
	okmonn::CreateObj(IID_PPV_ARGS(&damageSE));
	damageSE->Load("data/sound/se/general/damage.wav");
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
			// �U����`�ƃ_���[�W��`�Ŕ���
			if ((p.type == HitType::Attack && e.type == HitType::Attack) ||
				(p.type == HitType::Damage && e.type == HitType::Damage))
			{
				continue;
			}

			// ��`���
			Box pBox = Box(p.rect.pos, p.rect.size);
			Box eBox = Box(e.rect.pos, e.rect.size);

			if (CheckColBox(pBox, eBox))
			{
				Vec2f dir = pl.lock()->GetPos() - GetPos();
				if (p.type == HitType::Attack)
				{
					if (state == "Damage")
					{
						break;
					}

					if (type != CharacterType::EM_BOSS_WOLF)
					{
						SetTurnFlag(pl.lock()->GetTurnFlag() ? false : true);
						KnockBack(-dir);
						SetDamage(pl.lock()->GetParam().attackPow, cParam.defensePow);
						ChangeState("Damage");
						EffectManager::Get().CreateBloodSplash(tex[type].pos, tex[type].size, pl.lock()->GetTurnFlag());
						EffectManager::Get().CreateFlower(tex[type].pos, pl);
						cam.lock()->SetVibrationFlag(true, 5.0f);
					}
					else
					{
						if (!stunFlag)
						{
							if (++hitCnt > 3)
							{
								stunFlag = true;
							}

							auto& pos = pBox.pos;
							pos.y -= 50;
							if (turnFlag)
							{
								pos.x -= tex[type].size.x / 3;
							}

							EffectManager::Get().CreateBloodSplash(pos, tex[type].size / 3, pl.lock()->GetTurnFlag());
							SetDamage(pl.lock()->GetParam().attackPow, cParam.defensePow);
							cParam.hp -= damage;
							if (cParam.hp < 0)
							{
								ChangeState("Death");
							}
						}
					}
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
						cam.lock()->SetVibrationFlag(true, 5.0f);
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
		// �ԁA�폜�ς݂͑ΏۊO
		if ((*itr).type == EffectType::Flower || 
			(*itr).type == EffectType::BloodSplash ||
			(*itr).ptr->GetDeleteFlag())
		{
			continue;
		}

		if ((*itr).type == EffectType::Slashing)
		{
			for (auto& ef : (*itr).ptr->GetRect())
			{
				for (auto& em : GetRect())
				{
					// �G�̃_���[�W��`�Ɣ���
					if (em.type == HitType::Attack)
					{
						continue;
					}

					// ��`���
					Box efBox = Box(ef.rect.pos, ef.rect.size);
					Box emBox = Box(em.rect.pos, em.rect.size);

					if (CheckColBox(efBox, emBox))
					{
						if (type != CharacterType::EM_BOSS_WOLF)
						{
							//pl.lock()->SetHitFlag(true);
							Vec2f dir = pl.lock()->GetPos() - GetPos();
							SetTurnFlag(pl.lock()->GetTurnFlag() ? false : true);
							KnockBack(-dir);
							SetDamage(pl.lock()->GetParam().attackPow, cParam.defensePow);
							ChangeState("Damage");
							EffectManager::Get().CreateBloodSplash(tex[type].pos, tex[type].size, pl.lock()->GetTurnFlag());
							EffectManager::Get().CreateFlower(tex[type].pos, pl);
							cam.lock()->SetVibrationFlag(true, 5.0f);
						}
						else
						{
							if (!stunFlag)
							{
								if (++hitCnt > 3)
								{
									stunFlag = true;
									damageSE->Play(false);
								}

								auto& pos = efBox.pos;
								pos.y -= 50;
								if (turnFlag)
								{
									pos.x -= tex[type].size.x / 3;
								}

								EffectManager::Get().CreateBloodSplash(pos, tex[type].size / 3, pl.lock()->GetTurnFlag());
								SetDamage(pl.lock()->GetParam().attackPow, cParam.defensePow);
								cParam.hp -= damage;
								if (cParam.hp < 0)
								{
									ChangeState("Death");
								}
							}
						}

						break;
					}
				}
			}
		}
		else if ((*itr).type == EffectType::BossClaw)
		{
			for (auto& ef : (*itr).ptr->GetRect())
			{
				for (auto& p : pl.lock()->GetRect())
				{
					if (p.type == HitType::Attack)
					{
						continue;
					}

					// ��`���
					Box efBox = Box(ef.rect.pos, ef.rect.size);
					Box pBox = Box(p.rect.pos, p.rect.size);

					if (CheckColBox(efBox, pBox))
					{
						if (!pl.lock()->GetInvincibleFlag() && state != "Damage")
						{
							Vec2f pCenter = pl.lock()->GetPos() + pl.lock()->GetSize() / 2.0f;
							Vec2f eCenter = GetPos() + GetSize() / 2.0f;
							Vec2f dir = pCenter - eCenter;
							bool nextTurn = turnFlag;
							if (dir.x < 0)
							{
								nextTurn = false;
							}
							else if (dir.x > 0)
							{
								nextTurn = true;
							}
							pl.lock()->SetHitFlag(true);
							pl.lock()->SetTurnFlag(nextTurn);
							pl.lock()->KnockBack(dir);
							pl.lock()->SetDamage(cParam.attackPow, pl.lock()->GetParam().defensePow);
							pl.lock()->ChangeState("Damage");
							cam.lock()->SetVibrationFlag(true, 5.0f);
						}
						break;
					}
				}
			}
		}
	}
}

// ���E����
bool Enemy::CheckView()
{
	auto p = pl.lock();

	if (p->GetState() == "Death")
	{
		return false;
	}

	float length = turnFlag ? -lib.lock()->GetWinSize().x / 3.0f : lib.lock()->GetWinSize().x / 3.0f;

	// ���E��
	seg = Segment();
	seg.begin = tex[type].pos + tex[type].size / 2.0f;
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

// ��`���m�̏Փ˔���
bool Enemy::CheckColBox(const Box& box1, const Box& box2)
{
	if (fabs(box1.centor.x - box2.centor.x) < fabs(box1.half.x + box2.half.x) &&
		fabs(box1.centor.y - box2.centor.y) < fabs(box1.half.y + box2.half.y))
	{
		return true;
	}

	return false;
}

bool Enemy::CheckColBox(const Vec2f& pos1, const Vec2f& size1, const Vec2f& pos2, const Vec2f& size2)
{
	if (pos1.x <= pos2.x + size2.x && pos1.x + size1.x >= pos2.x &&
		pos1.y <= pos2.y + size2.y && pos1.y + size1.y >= pos2.y)
	{
		return true;
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
