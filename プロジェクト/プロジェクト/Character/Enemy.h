#pragma once
#include "Character.h"
#include "Player.h"

// “G‚³‚ñ’B
typedef
enum class Enemies
{
	Wolf
}EM;

class Enemy : 
	public Character
{
public:
	Enemy();
	~Enemy();
protected:
	// Õ“Ë”»’è
	void CheckHit();

	// ‹ŠE•`‰æ
	void DrawViewRange();

	// ó‘Ô‰Šú‰»
	virtual void InitFunc() = 0;

	// ƒvƒŒƒCƒ„[
	std::weak_ptr<Player> pl;

	// ‹ŠE
	Vec2f viewRange;

	// ‹ŠE”ÍˆÍ
	Primitive viewBox;
};