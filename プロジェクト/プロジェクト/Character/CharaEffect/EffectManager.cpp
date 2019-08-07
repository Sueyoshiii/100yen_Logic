#include "EffectManager.h"
#include "../Player/Player.h"
#include "Flower/Flower.h"
#include "Slash/Slash.h"
#include "BloodSplash/BloodSplash.h"
#include "Claw/Claw.h"

// �R���X�g���N�^
EffectManager::EffectManager()
{
}

// �f�X�g���N�^
EffectManager::~EffectManager()
{
	list.clear();
}

// �C���X�^���X
EffectManager& EffectManager::Get()
{
	static EffectManager instance;
	return instance;
}

// �X�V
void EffectManager::Update()
{
	for (auto itr = list.begin(); itr != list.end();)
	{
		if ((*itr).ptr->GetDeleteFlag())
		{
			itr = list.erase(itr);
			continue;
		}
		else
		{
			(*itr).ptr->Update();
			++itr;
		}
	}
}

// �`��
void EffectManager::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr).ptr->Draw(lib);
	}
}

// �Ԑ���
void EffectManager::CreateFlower(const Vec2f& pos, std::weak_ptr<Player> pl)
{
	map[EffectType::Flower] = std::make_shared<Flower>(pos, pl);
	list.push_back(ListParameter(EffectType::Flower, map[EffectType::Flower]));
}
// �a������
void EffectManager::CreateSlash(const std::string& state, const CharacterType& plType, const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	map[EffectType::Slashing] = std::make_shared<Slash>(state, plType, pos, size, turnFlag);
	list.push_back(ListParameter(EffectType::Slashing, map[EffectType::Slashing]));
}

// �����Ԃ�����
void EffectManager::CreateBloodSplash(const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	map[EffectType::BloodSplash] = std::make_shared<BloodSplash>(pos, size, turnFlag);
	list.push_back(ListParameter(EffectType::BloodSplash, map[EffectType::BloodSplash]));
}

// �ܐ���
void EffectManager::CreateBossClaw(std::weak_ptr<Camera> cam, const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	map[EffectType::BossClaw] = std::make_shared<Claw>(cam, pos, size, turnFlag);
	list.push_back(ListParameter(EffectType::BossClaw, map[EffectType::BossClaw]));
}

// �폜
void EffectManager::Delete()
{
	list.clear();
}

// �G�t�F�N�g���X�g�擾
std::list<ListParameter> EffectManager::GetEffectList()
{
	return list;
}

// ���W�ݒ�
void EffectManager::SetPos(const Vec2f& pos)
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		if ((*itr).type == EffectType::Slashing)
		{
			(*itr).ptr->SetPos(pos);
		}
	}
}
