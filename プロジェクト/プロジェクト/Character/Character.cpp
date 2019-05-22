#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
}

// ��ԑJ��
void Character::ChangeState(const std::string& state)
{
	if (func.find(state) == func.end())
	{
		return;
	}
	flame = 0;
	this->state = state;
}

// �ǂݍ���
void Character::LoadData(const std::string& filePath)
{
	Info::Get().Load(filePath);
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

std::string Character::ConvertToString(const ST state)
{
	if (stMap.find(state) != stMap.end())
	{
		return stMap[state];
	}

	return std::string();
}
