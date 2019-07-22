#pragma once
#include "../CharaEffect.h"

class Slash :
	public CharaEffect
{
public:
	Slash(const std::string& state, const CharacterType& plType, const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Slash();

	// 更新
	void Update();

	// 描画
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	void SetPos(const Vec2f& pos);
private:
	void InitState();

	void InitType();

	void TypeNormal();
	void TypeWolf();

	// 初撃
	void FirstAttackUpdate();
	// 二撃
	void SecondAttackUpdate();
	// 三撃
	void ThirdAttackUpdate();
	// 四撃
	void FourthAttackUpdate();
	// 攻撃処理
	void AttackUpdate(const unsigned int attackInterval);
	// ジャンプ攻撃
	void JumpAttackUpdate();

	// 攻撃フレーム
	unsigned int attackCnt;

	// 状態コンテナ
	std::unordered_map<std::string, std::string> stateMap;

	// 形態関数
	std::unordered_map<CharacterType, std::function<void()>> typeFunc;
};