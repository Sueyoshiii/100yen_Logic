#pragma once
#include "Enemy.h"
#include <list>

class EnemyManager
{
public:
	~EnemyManager();

	static EnemyManager& Get();

	// �X�V
	void Update();

	// �`��
	void Draw();

	// ����
	void Summons(const Enemies& em, const Vec2f& pos, std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam);
private:
	EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	void operator=(const EnemyManager&) = delete;

	// �G����
	std::unordered_map<Enemies, std::shared_ptr<Enemy>> map;

	// �G����B
	std::list<std::shared_ptr<Enemy>> list;
};
