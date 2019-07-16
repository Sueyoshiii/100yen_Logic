#include "StageManager.h"

namespace {
	const float GR = 1.9f;
	const float GROUND = 1200.0f;
}

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
}

StageManager& StageManager::Get()
{
	static StageManager instance;
	return instance;
}

void StageManager::Update()
{
}

void StageManager::Draw()
{
}

// �͈͎擾
StageRange StageManager::GetRange()
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left() / 10, range.Right());
	return stRange;
}

void StageManager::SetRange(const Vec2& size)
{
	range = StageManager::Rect(Vec2f(), float(size.x * 10), float(size.y));
}

float StageManager::GetGround() const
{
	return GROUND;
}

float StageManager::GetGravity() const
{
	return GR;
}
