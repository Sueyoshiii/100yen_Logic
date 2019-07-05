#pragma once
#include "../Character.h"

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

	// ���[���h���W�擾
	Vec2f GetWorldPos()const;

	// �q�b�g�t���O�擾
	bool GetHitFlag();

	// �q�b�g�t���O��ݒ�
	void SetHitFlag(const bool flag);
private:
	// ��ԂƊ֐����o�C���h
	void InitFunc();
	// �ҋ@
	void NeutralUpdate();
	// ���s
	void WalkUpdate();
	void CheckWalk();
	// �W�����v
	void JumpUpdate();
	void CheckJump();
	// ����
	void FallUpdate();
	void CheckFall();
	// �_�b�V��
	void DashUpdate();
	void CheckDash();
	// �ŏ��̍U��
	void FirstAttackUpdate();
	void CheckFirstAttack();
	// 2�i�ڂ̍U��
	void SecondAttackUpdate();
	// 3�i�ڂ̍U��
	void ThirdAttackUpdate();
	// ���̍U���ֈڂ�
	void CheckNextAttack(const unsigned int attackInterval);
	// �_���[�W
	void DamageUpdate();
	// ���S
	void DeathUpdate();

	// �q�b�g�X�g�b�v����
	bool CheckHitStop();

	// �W�����v�t���O
	bool jumpFlag;

	// �_�b�V���t���O
	bool dashFlag;

	// �U���t���O
	bool attackFlag;

	// �U���J�E���g
	unsigned int attackCnt;

	// �q�b�g�t���O
	bool hitFlag;

	// �q�b�g�X�g�b�v�J�E���g
	unsigned int hitStopCnt;
};