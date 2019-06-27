#pragma once
#include "Enemy.h"
#include <list>

class EnemyManager
{
public:
	~EnemyManager();

	static EnemyManager& Get();

	// XV
	void Update();

	// •`‰æ
	void Draw();

	// ¢Š«
	void Summons(const Enemies& em, const Vec2f& pos, std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam);
private:
	EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	void operator=(const EnemyManager&) = delete;

	// “G‚³‚ñ
	std::unordered_map<Enemies, std::shared_ptr<Enemy>> map;

	// “G‚³‚ñ’B
	std::list<std::shared_ptr<Enemy>> list;
};
