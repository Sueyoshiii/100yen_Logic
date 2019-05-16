#pragma once
#include "Character.h"
#include <string>
#include <memory>
#include <MyLib.h>

class Camera;

// �萔
typedef
struct ConstParam
{
	static const float SPEED;		// ���x
	static const float JUMP_POW;	// �W�����v��
	static const float GR;			// �d��
	static const float GROUND;		// �n��
}Const;

class Player : 
	public Character
{
public:
	Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
	~Player();

	// �X�V
	void Update();

	// �`��
	void Draw();

	Vec2f GetLocalPos()const;
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

	// �W�����v�t���O
	bool jumpFlag;

	// ���W
	Vec2f pos;

	std::weak_ptr<Camera> cam;
};