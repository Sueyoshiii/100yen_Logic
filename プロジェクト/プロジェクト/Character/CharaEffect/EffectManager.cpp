#include "EffectManager.h"
#include "Flower.h"

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
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Update();
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
void EffectManager::Create(const EffectType& type, const Vec2f& pos)
{
	if (type == EffectType::Flower)
	{
		map[type] = std::make_shared<Flower>(pos);
		list.push_back(map[type]);
	}
	else if (type == EffectType::Slashing)
	{

	}
}
