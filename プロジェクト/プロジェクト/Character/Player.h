#pragma once
#include "Character.h"

class Camera;


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
	// ��ԂƊ֐����o�C���h
	void InitFunc();
	// �ҋ@
	void NeutralUpdate();
	// ���s
	void WalkUpdate();
	void Walk();
	// �W�����v
	void JumpUpdate();
	void Jump();
	// �_�b�V��
	void DushUpdate();
	void Dush();
	// �U��1
	void Attack1Update();
	// �U��2
	void Attack2Update();
	// �U��3
	void Attack3Update();
	// �_���[�W
	void DamageUpdate();
	// ���S
	void DeathUpdate();

	// �W�����v�t���O
	bool jumpFlag;

	// �_�b�V���t���O
	bool dushFlag;

	std::weak_ptr<Camera> cam;
};