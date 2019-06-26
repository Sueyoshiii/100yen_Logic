#pragma once
#include "../CharaEffect.h"

class Player;

class Slash :
	public CharaEffect
{
public:
	Slash(const CharacterState& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Slash();

	// XV
	void Update();

	// •`‰æ
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);
private:
	void InitState();

	// ‰Œ‚
	void FirstAttackUpdate();
	// “ñŒ‚
	void SecondAttackUpdate();
	// OŒ‚
	void ThirdAttackUpdate();
	// UŒ‚ˆ—
	void AttackUpdate(const unsigned int attackInterval);

	std::unordered_map <CharacterState, std::function<void(void)>> stFunc;
	std::weak_ptr<Player> pl;
	unsigned int attackCnt;
	CharacterState state;
};