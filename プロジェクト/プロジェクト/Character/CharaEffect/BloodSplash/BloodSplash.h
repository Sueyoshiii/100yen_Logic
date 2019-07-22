#pragma once
#include "../CharaEffect.h"

class BloodSplash :
	public CharaEffect
{
public:
	BloodSplash(const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~BloodSplash();

	// çXêV
	void Update();

	// ï`âÊ
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	void SetPos(const Vec2f& pos);
private:
	Texture blood;
	unsigned int animTime;
	unsigned int animNum;
};