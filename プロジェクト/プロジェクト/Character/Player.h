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
	void DashUpdate();
	void Dash();
	// �U��1
	void Attack1Update();
	void Attack1();
	// �U��2
	void Attack2Update();
	// �U��3
	void Attack3Update();
	// ���̍U���ֈڂ�
	void NextAttack();
	// �_���[�W
	void DamageUpdate();
	// ���S
	void DeathUpdate();

	// �W�����v�t���O
	bool jumpFlag;

	// �_�b�V���t���O
	bool dashFlag;

	// �U���t���O
	bool AttackFlag;

	std::weak_ptr<Camera> cam;
};