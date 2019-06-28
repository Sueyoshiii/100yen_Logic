#pragma once
#include "CharaEffect.h"
#include <list>

class Player;

struct ListParameter
{
	EffectType type;
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

	// 更新
	void Update();

	// 描画
	void Draw(std::weak_ptr<MyLib> lib);

	// 生成
	void Create(const EffectType& type, const Vec2f& pos, std::weak_ptr<Player> pl);
	void CreateSlash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag);

	// エフェクトリスト取得
	std::list<ListParameter> GetEffectList();
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

	std::unordered_map<EffectType, std::shared_ptr<CharaEffect>> map;
	std::list<ListParameter> list;
	//std::list<std::shared_ptr<CharaEffect>> list;
	//std::unordered_map<EffectType, std::list<std::shared_ptr<CharaEffect>>> list;
};