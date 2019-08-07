#pragma once
#include "../CharaEffect.h"
#include "../../../Camera/Camera.h"

class Claw :
	public CharaEffect
{
public:
	Claw(std::weak_ptr<Camera> cam, const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Claw();

	// çXêV
	void Update();

	// ï`âÊ
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);
	void SetPos(const Vec2f& pos);
private:
	void ClawUpdate();

	unsigned int attackCnt;
	std::weak_ptr<Camera> camera;
};