#include "Character.h"

Character::Character() :
	state("Neutral"), vel(Vec2f()), turnFlag(false), frame(0), animCnt(0), index(0)
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
	frame   = 0;
	animCnt = 0;
	index   = 0;
	this->state = state;
}

// �ǂݍ���
void Character::LoadData(const std::string& filePath)
{
	Info::Get().Load(filePath);
}

// �A�j���[�V�����̏I���𒲂ׂ�
bool Character::CheckAnimEnd()
{
	if (frame >= info.lock()->at(state).rect.size() - 1)
	{
		return true;
	}

	return false;
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
