#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
}

// 状態遷移
void Character::ChangeState(const std::string& state)
{
	if (func.find(state) == func.end())
	{
		return;
	}
	flame = 0;
	this->state = state;
}

// 読み込み
void Character::LoadData(const std::string& filePath)
{
	Info::Get().Load(filePath);
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

std::string Character::ConvertToString(const ST state)
{
	if (stMap.find(state) != stMap.end())
	{
		return stMap[state];
	}

	return std::string();
}
