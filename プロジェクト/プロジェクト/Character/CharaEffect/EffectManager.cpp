#include "EffectManager.h"
#include "../Player/Player.h"
#include "Flower/Flower.h"
#include "Slash/Slash.h"

// コンストラクタ
EffectManager::EffectManager()
{
}

// デストラクタ
EffectManager::~EffectManager()
{
}

// インスタンス
EffectManager& EffectManager::Get()
{
	static EffectManager instance;
	return instance;
}

// 更新
void EffectManager::Update()
{
	for (auto itr = list.begin(); itr != list.end();)
	{
		//if ((*itr)->GetDeleteFlag())
		//{
		//	(*itr)->Delete();
		//	itr = list.erase(itr);
		//	continue;
		//}
		//else
		//{
		//	(*itr)->Update();
		//	++itr;
		//}
		(*itr)->Update();
		++itr;
	}
}

// 描画
void EffectManager::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Draw(lib);
	}
}

// 生成
void EffectManager::Create(const EffectType& type, const Vec2f& pos, std::weak_ptr<Player> pl)
{
	if (type == EffectType::Flower)
	{
		map[type] = std::make_shared<Flower>(pos, pl);
		list.push_back(map[type]);
	}
}

void EffectManager::CreateSlash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	map[EffectType::Slashing] = std::make_shared<Slash>(state, pos, size, turnFlag);
	list.push_back(map[EffectType::Slashing]);
}

std::weak_ptr<CharaEffect> EffectManager::GetEffect(const EffectType& type)
{
	return map[type];
}
