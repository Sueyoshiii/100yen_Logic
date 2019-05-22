#include "Character.h"
#include "../Info/Info.h"

Character::Character()
{
	stMap[ST::Neutral] = "Neutral";
	stMap[ST::Walk]    = "Walk";
	stMap[ST::Attack1] = "Attack1";
	stMap[ST::Attack2] = "Attack2";
	stMap[ST::Attack3] = "Attack3";
	stMap[ST::Jump]    = "Jump";
	stMap[ST::Dash]    = "Dash";
	stMap[ST::Damage]  = "Damage";
	stMap[ST::Death]   = "Deth";
}

Character::~Character()
{
}

// ��ԑJ��
void Character::ChangeState(const std::string& state)
{
	flame = 0;
	this->state = state;
}

// �ǂݍ���
void Character::LoadData(const std::string& filePath)
{

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
