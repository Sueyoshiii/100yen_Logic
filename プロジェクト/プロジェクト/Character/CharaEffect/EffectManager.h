#pragma once
#include "CharaEffect.h"
#include <list>

class Player;

struct ListParameter
{
	// 種別
	EffectType type;
	// データ
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

	// 花生成
	void CreateFlower(const Vec2f& pos, std::weak_ptr<Player> pl);
	// 斬撃生成
	void CreateSlash(const std::string& state, const CharacterType& plType, const Vec2f& pos, const Vec2f& size, const bool turnFlag);

	// 削除
	void Delete();

	// エフェクトリスト取得
	std::list<ListParameter> GetEffectList();
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

	std::unordered_map<EffectType, std::shared_ptr<CharaEffect>> map;
	std::list<ListParameter> list;
};