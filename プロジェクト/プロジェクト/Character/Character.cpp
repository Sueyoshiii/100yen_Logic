#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
}

// 状態遷移
void Character::ChangeState(const ST& state)
{
	this->state = state;
}

// 座標取得
Vec2f Character::GetPos() const
{
	return tex.pos;
}

// サイズ取得
Vec2f Character::GetSize() const
{
	return tex.size;
}
