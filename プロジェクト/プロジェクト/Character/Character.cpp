#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
}

// ��ԑJ��
void Character::ChangeState(const ST& state)
{
	this->state = state;
}

// ���W�擾
Vec2f Character::GetPos() const
{
	return tex.pos;
}

// �T�C�Y�擾
Vec2f Character::GetSize() const
{
	return tex.size;
}
