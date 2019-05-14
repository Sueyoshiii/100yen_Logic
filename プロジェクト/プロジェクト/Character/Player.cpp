#include "Player.h"

Player::Player(std::weak_ptr<MyLib> lib) :
	pl(Texture("img/nino8.jpg")), turnFlag(false)
{
	this->lib = lib;
	update = &Player::NeutralUpdate;
	pl.size = Vec2f(50, 100);
}

Player::~Player()
{
}

// �X�V
void Player::Update()
{
	(this->*update)();
}

// �`��
void Player::Draw()
{
	lib.lock()->Draw(pl, 1.0f, turnFlag);
}

// �ҋ@
void Player::NeutralUpdate()
{
	if (In.IsKey(Key::Num4) || In.IsKey(Key::Num6))
	{
		update = &Player::WalkUpdate;
	}
}

// ���s
void Player::WalkUpdate()
{
	if (In.IsKey(Key::Num4))
	{
		turnFlag = true;
		pl.pos.x--;
	}
	else if (In.IsKey(Key::Num6))
	{
		turnFlag = false;
		pl.pos.x++;
	}
	else {
		update = &Player::NeutralUpdate;
	}
}

// ���W�擾
Vec2f Player::GetPos() const
{
	return pl.pos;
}

// �T�C�Y�擾
Vec2f Player::GetSize() const
{
	return pl.size;
}
