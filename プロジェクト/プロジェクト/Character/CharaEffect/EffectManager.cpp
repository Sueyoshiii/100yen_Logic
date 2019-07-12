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
	list.clear();
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

// 描画
void EffectManager::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr).ptr->Draw(lib);
	}
}

// 花生成
void EffectManager::CreateFlower(const Vec2f& pos, std::weak_ptr<Player> pl)
{
	map[EffectType::Flower] = std::make_shared<Flower>(pos, pl);
	list.push_back(ListParameter(EffectType::Flower, map[EffectType::Flower]));
}
// 斬撃生成
void EffectManager::CreateSlash(const std::string& state, const CharacterType& plType, const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	map[EffectType::Slashing] = std::make_shared<Slash>(state, plType, pos, size, turnFlag);
	list.push_back(ListParameter(EffectType::Slashing, map[EffectType::Slashing]));
}

// 削除
void EffectManager::Delete()
{
	list.clear();
}

// エフェクトリスト取得
std::list<ListParameter> EffectManager::GetEffectList()
{
	return list;
}
