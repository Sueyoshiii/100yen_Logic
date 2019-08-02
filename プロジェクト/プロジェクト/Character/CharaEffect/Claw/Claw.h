#pragma once
#include "../CharaEffect.h"

class Claw :
	public CharaEffect
{
public:
	Claw(const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Claw();

	// çXêV
	void Update();

	// ï`âÊ
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);
	void SetPos(const Vec2f& pos);
private:
	unsigned int attackCnt;
};