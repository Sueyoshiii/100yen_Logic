#pragma once
#include "Character.h"
#include <string>
#include <memory>
#include <MyLib.h>

const float GR = 0.7f;
const float JUMP_POW = -20.0f;
const float GROUND = 500.0f;
const float SPEED = 2.0f;

class Player : 
	public Character
{
public:
	Player(std::weak_ptr<MyLib> lib);
	~Player();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// 状態
	void(Player::* update)();
	// 待機
	void NeutralUpdate();
	// 歩行
	void WalkUpdate();
	// ジャンプ
	void JumpUpdate();
	void Jump();

	// 反転フラグ
	bool turnFlag;

	Vec2f vel;
	bool jumpFlag;
};