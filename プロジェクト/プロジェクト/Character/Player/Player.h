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

	// ���S�t���O�擾
	bool GetDeadEndFlag()const;

	// �������W�擾
	Vec2f GetFirstPos()const;

	// �q�b�g�t���O��ݒ�
	void SetHitFlag(const bool flag);

	// ���W��ݒ�
	void SetPos(const Vec2f& pos);
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
	// 4�i�ڂ̍U��
	void FourthAttackUpdate();
	// ���̍U���ֈڂ�
	void CheckNextAttack(const unsigned int attackInterval);
	// �W�����v�U��
	void JumpAttackUpdate();
	void CheckJumpAttack();
	// �_���[�W
	void DamageUpdate();
	// ���S
	void DeathUpdate();
	// �ϐg
	void TransformUpdate();
	void CheckTransform();
	void Transform(const CharacterType& next, const CharacterType& now, const CharacterParameter& param);

	// �q�b�g�X�g�b�v����
	bool CheckHitStop();

	// �_���[�W���̕`��
	void DamageDraw();

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

	// ���ŃJ�E���g
	unsigned int disappearCnt;

	// �N���e�B�J���_���[�W
	Texture critical;

	// �N���e�B�J���_���[�W�J�E���g
	unsigned int criticalCnt;

	// �_���[�W���o�̃A���t�@�l
	float criticalAlpha;

	// ���S�t���O
	bool deadEndFlag;

	// hp�摜
	std::vector<Texture> hpImg;
};