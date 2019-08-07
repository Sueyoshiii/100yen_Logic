#pragma once
#include "../Enemy.h"

class  BossWolf :
	public Enemy
{
public:
	BossWolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos);
	~BossWolf();

	void Update();
	void Draw();

private:
	void NeutralUpdate();
	void AttackUpdate();
	void CheckAttack();
	void JumpUpdate();
	void DeadUpdate();

	void InitFunc();

	unsigned int cnt;
	unsigned int attackCnt;
	unsigned int stunCnt;

	int stageLeft;
	int stageRight;

	bool jumpedFlag;

	Okdio* landingSE;
	Okdio* attackSE;
};