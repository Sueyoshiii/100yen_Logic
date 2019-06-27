#pragma once
#include "CharaEffect.h"
#include <list>

class Player;

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

	// エフェクト取得
	std::weak_ptr<CharaEffect> GetEffect(const EffectType& type);
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

	std::unordered_map<EffectType, std::shared_ptr<CharaEffect>> map;
	std::list<std::shared_ptr<CharaEffect>> list;
};