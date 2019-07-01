#include "EffectManager.h"
#include "../Player/Player.h"
#include "Flower/Flower.h"
#include "Slash/Slash.h"

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
	for (auto itr = list.begin(); itr != list.end();)
	{
		if ((*itr).ptr->GetDeleteFlag())
		{
			itr = list.erase(itr);
			continue;
		}
		else
		{
			(*itr).ptr->Draw(lib);
			++itr;
		}
	}
}

// ����
void EffectManager::CreateFlower(const Vec2f& pos, std::weak_ptr<Player> pl)
{
	map[EffectType::Flower] = std::make_shared<Flower>(pos, pl);
	list.push_back(ListParameter(EffectType::Flower, map[EffectType::Flower]));
}

void EffectManager::CreateSlash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	map[EffectType::Slashing] = std::make_shared<Slash>(state, pos, size, turnFlag);
	list.push_back(ListParameter(EffectType::Slashing, map[EffectType::Slashing]));
}

// �G�t�F�N�g���X�g�擾
std::list<ListParameter> EffectManager::GetEffectList()
{
	return list;
}
