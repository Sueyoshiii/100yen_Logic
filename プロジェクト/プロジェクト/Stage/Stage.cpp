#include "Stage.h"

const float Stage::ConstParam::GR = 1.9f;
const float Stage::ConstParam::GROUND = 1000.0f;

// コンストラクタ
Stage::Stage()
{
	range = Stage::Rect(Vec2f(0, 0), 640 * 10, 640);
}

// デストラクタ
Stage::~Stage()
{
}

// インスタンス
Stage& Stage::Get()
{
	static Stage instance;
	return instance;
}

// ステージデータ読み込み
int Stage::Load(const std::string& filePath)
{
	return 0;
}

// 範囲取得
StageRange Stage::GetRange() const
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left() / 10, range.Right());
	return stRange;
}

// 地面取得
float Stage::GetGround() const
{
	return ConstParam::GROUND;
}

// 重力取得
float Stage::GetGravity() const
{
	return ConstParam::GR;
}
