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

// �폜�t���O�擾
bool CharaEffect::GetDeleteFlag() const
{
	return deleteFlag;
}
