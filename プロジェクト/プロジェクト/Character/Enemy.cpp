#include "Enemy.h"
#include "Player.h"
#include <iostream>

Enemy::Enemy()
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

			// ��`�̒��S
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
					// Enemy�Ƀ_���[�W
					//ChangeState(ST::Damage);
					std::cout << "Hit!" << std::endl;

					/// �U�����ꂽ��Player�̕��������Ă����������H
					/// ���ƃm�b�N�o�b�N������Ȃ���
				}
				else
				{
					// Player�Ƀ_���[�W
					//pl.lock()->ChangeState(ST::Damage);
					std::cout << "Damage!" << std::endl;
				}
			}
		}
	}
}
