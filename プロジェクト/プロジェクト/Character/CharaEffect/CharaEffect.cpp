#include "CharaEffect.h"
#include "../Player/Player.h"

// コンストラクタ
CharaEffect::CharaEffect() :
	deleteFlag(false)
{
}

// デストラクタ
CharaEffect::~CharaEffect()
{
}

bool CharaEffect::GetDeleteFlag() const
{
	return deleteFlag;
}
