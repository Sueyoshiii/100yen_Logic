#include "EnemyManager.h"
#include "Wolf.h"

// �R���X�g���N�^
EnemyManager::EnemyManager()
{
}

// �f�X�g���N�^
EnemyManager::~EnemyManager()
{
}

// �C���X�^���X
EnemyManager& EnemyManager::Get()
{
	static EnemyManager instance;
	return instance;
}

// �X�V
void EnemyManager::Update()
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Update();
	}
}

// �`��
void EnemyManager::Draw()
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

// ����
void EnemyManager::Summons(const EM& em, const Vec2f& pos, std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam)
{
	if (em == EM::Wolf)
	{
		map[em] = std::make_shared<Wolf>(lib, pl, cam, pos);
		list.emplace_back(map[em]);
	}
}
