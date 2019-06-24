#include "EffectManager.h"
#include "Flower.h"

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
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Update();
	}
}

// �`��
void EffectManager::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Draw(lib);
	}
}

// ����
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
