#pragma once
#include "Character.h"
#include <string>
#include <memory>
#include <MyLib.h>

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

	// ���W�擾
	Vec2f GetPos()const;

	// �T�C�Y�擾
	Vec2f GetSize()const;
private:
	// �ҋ@
	void NeutralUpdate();
	// ���s
	void WalkUpdate();
	// ���
	void(Player::* update)();

	// �v���C���[
	Texture pl;

	// ���]�t���O
	bool turnFlag;
};