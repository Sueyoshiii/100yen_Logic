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

// 削除フラグ取得
bool CharaEffect::GetDeleteFlag() const
{
	return deleteFlag;
}
