#pragma once
#include "../CharaEffect.h"

class Slash :
	public CharaEffect
{
public:
	Slash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Slash();

	// 更新
	void Update();

	// 描画
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	// 削除
	void Delete();
private:
	void InitState();

	// 初撃
	void FirstAttackUpdate();
	// 二撃
	void SecondAttackUpdate();
	// 三撃
	void ThirdAttackUpdate();
	// 攻撃処理
	void AttackUpdate(const unsigned int attackInterval);

	// 攻撃フレーム
	unsigned int attackCnt;

	// 状態コンテナ
	std::unordered_map<std::string, std::string> stateMap;
};