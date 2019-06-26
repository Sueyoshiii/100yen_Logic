#pragma once
#include "CharaEffect.h"
#include <list>

class Player;

class EffectManager
{
public:
	~EffectManager();

	static EffectManager& Get();

	// çXêV
	void Update();

	// ï`âÊ
	void Draw(std::weak_ptr<MyLib> lib);

	// ê∂ê¨
	void Create(const EffectType& type, const Vec2f& pos, std::weak_ptr<Player> pl);
	void CreateSlash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag);
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

	std::unordered_map<EffectType, std::shared_ptr<CharaEffect>> map;
	std::list<std::shared_ptr<CharaEffect>> list;
};