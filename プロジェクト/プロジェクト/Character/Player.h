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

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// ���
	void(Player::* update)();
	// �ҋ@
	void NeutralUpdate();
	// ���s
	void WalkUpdate();
	// �W�����v
	void JumpUpdate();
	void Jump();

	// ���]�t���O
	bool turnFlag;

	Vec2f vel;
	bool jumpFlag;
};