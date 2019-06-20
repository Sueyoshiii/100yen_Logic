#include "EnemyManager.h"
#include "Wolf.h"

// コンストラクタ
EnemyManager::EnemyManager()
{
}

// デストラクタ
EnemyManager::~EnemyManager()
{
}

// インスタンス
EnemyManager& EnemyManager::Get()
{
	static EnemyManager instance;
	return instance;
}

// 更新
void EnemyManager::Update()
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Update();
	}
}

// 描画
void EnemyManager::Draw()
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

// 召喚
void EnemyManager::Summons(const EM& em, const Vec2f& pos, std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam)
{
	if (em == EM::Wolf)
	{
		map[em] = std::make_shared<Wolf>(lib, pl, cam, pos);
		list.emplace_back(map[em]);
	}
}
