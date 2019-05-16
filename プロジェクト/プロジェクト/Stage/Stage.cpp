#include "Stage.h"

Stage::Stage()
{
	range = Rect(Vec2f(0, 0), 640 * 10, 640);
}

Stage::~Stage()
{
}

Stage& Stage::Get()
{
	static Stage instance;
	return instance;
}

// �͈͎擾
StageRange Stage::GetRange() const
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left() / 10, range.Right());
	return stRange;
}
