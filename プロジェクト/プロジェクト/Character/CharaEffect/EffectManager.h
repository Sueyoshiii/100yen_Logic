#pragma once
#include "CharaEffect.h"
#include <list>

class EffectManager
{
public:
	~EffectManager();

	static EffectManager& Get();

	// XV
	void Update();

	// •`‰æ
	void Draw(std::weak_ptr<MyLib> lib);

	// ¶¬
	void Create(const EffectType& type, const Vec2f& pos);
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

	std::unordered_map<EffectType, std::shared_ptr<CharaEffect>> map;
	std::list<std::shared_ptr<CharaEffect>> list;
};