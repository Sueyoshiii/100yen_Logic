#pragma once
#include "../CharaEffect.h"

class Claw :
	public CharaEffect
{
public:
	Claw(const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Claw();

	// XV
	void Update();

	// •`‰æ
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);
private:
	unsigned int attackCnt;
};