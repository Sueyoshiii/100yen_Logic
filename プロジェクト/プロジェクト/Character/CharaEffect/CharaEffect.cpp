#include "CharaEffect.h"
#include "../Player/Player.h"

// �R���X�g���N�^
CharaEffect::CharaEffect() :
	deleteFlag(false)
{
}

// �f�X�g���N�^
CharaEffect::~CharaEffect()
{
}

bool CharaEffect::GetDeleteFlag() const
{
	return deleteFlag;
}
