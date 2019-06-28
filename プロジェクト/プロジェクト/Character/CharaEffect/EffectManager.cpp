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
			//(*itr).ptr->Delete();
			itr = list.erase(itr);
			continue;
		}
		else
		{
			(*itr).ptr->Update();
			++itr;
		}
		//(*itr).ptr->Update();
		//++itr;
	}
}

// �`��
void EffectManager::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto itr = list.begin(); itr != list.end();)
	{
		if ((*itr).ptr->GetDeleteFlag())
		{
			//(*itr).ptr->Delete();
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
void EffectManager::Create(const EffectType& type, const Vec2f& pos, std::weak_ptr<Player> pl)
{
	if (type == EffectType::Flower)
	{
		map[type] = std::make_shared<Flower>(pos, pl);
		list.push_back(ListParameter(EffectType::Flower, map[type]));
	}
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
