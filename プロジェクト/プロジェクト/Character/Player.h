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
	// ��ԏ�����
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

	// �W�����v�t���O
	bool jumpFlag;

	// �_�b�V���t���O
	bool dushFlag;

	// ���W
	Vec2f pos;

	std::weak_ptr<Camera> cam;
};