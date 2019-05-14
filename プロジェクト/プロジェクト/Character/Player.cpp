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

// 更新
void Player::Update()
{
	(this->*update)();
}

// 描画
void Player::Draw()
{
	lib.lock()->Draw(pl, 1.0f, turnFlag);
}

// 待機
void Player::NeutralUpdate()
{
	if (In.IsKey(Key::Num4) || In.IsKey(Key::Num6))
	{
		update = &Player::WalkUpdate;
	}
}

// 歩行
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

// 座標取得
Vec2f Player::GetPos() const
{
	return pl.pos;
}

// サイズ取得
Vec2f Player::GetSize() const
{
	return pl.size;
}
