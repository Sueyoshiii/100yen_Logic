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
	stage->Update();
}

void StageManager::Draw()
{
	stage->Draw();
}

void StageManager::DrawFront()
{
	stage->DrawFront();
}

void StageManager::DrawBox()
{
	stage->DrawBox();
}

void StageManager::SetRoom()
{
	stage.reset(stage->GetNextRoom());
}

void StageManager::SetRoom(Stage* room)
{
	stage.reset(room);
}

bool StageManager::CheckWall(const Vec2f& pos, const Vec2f& size)
{
	return stage->CheckWall(pos, size);
}

// �͈͎擾
StageRange StageManager::GetRange()
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left() / 10, range.Right());
	return stRange;
}

bool StageManager::GetNextRoomFlag() const
{
	return stage->GetNextRoomFlag();
}

float StageManager::GetBoxAlpha() const
{
	return stage->GetBoxAlpha();
}

void StageManager::SetRange(const Vec2& size)
{
	range = StageManager::Rect(Vec2f(), float(size.x * 10), float(size.y));
}

void StageManager::SetNextRoomFlag(const bool flag)
{
	stage->SetNextRoomFlag(flag);
}

float StageManager::GetGround() const
{
	return GROUND;
}

float StageManager::GetGravity() const
{
	return GR;
}
