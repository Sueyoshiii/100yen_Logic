#pragma once
#include "CharaEffect.h"
#include <list>

class Player;

struct ListParameter
{
	// ���
	EffectType type;
	// �f�[�^
	std::shared_ptr<CharaEffect> ptr;

	ListParameter() : type(EffectType::Flower), ptr(nullptr) {}
	ListParameter(const EffectType& type, std::shared_ptr<CharaEffect> ptr) :
		type(type), ptr(ptr) {}
};

class EffectManager
{
public:
	~EffectManager();

	static EffectManager& Get();

	// �X�V
	void Update();

	// �`��
	void Draw(std::weak_ptr<MyLib> lib);

	// �Ԑ���
	void CreateFlower(const Vec2f& pos, std::weak_ptr<Player> pl);
	// �a������
	void CreateSlash(const std::string& state, const CharacterType& plType, const Vec2f& pos, const Vec2f& size, const bool turnFlag);

	// �폜
	void Delete();

	// �G�t�F�N�g���X�g�擾
	std::list<ListParameter> GetEffectList();
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

	std::unordered_map<EffectType, std::shared_ptr<CharaEffect>> map;
	std::list<ListParameter> list;
};