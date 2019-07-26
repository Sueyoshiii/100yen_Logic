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

bool StageManager::CheckWall(const Vec2f& pos, const Vec2f& size, const bool turnFlag, const Dir& obj)
{
	return stage->CheckWall(pos, size, turnFlag, obj);
}

bool StageManager::CheckMapChip(const Vec2f& pos)
{
	return stage->CheckMapChip(pos);
}

void StageManager::CheckMapCol(Vec2f& pos, const Vec2f& size, const Vec2f& vel)
{
	stage->CheckMapCol(pos, size, vel);
}

// �͈͎擾
StageRange StageManager::GetRange()
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left(), range.Right());
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
	auto stSize = stage->GetStageSize();
	range = StageManager::Rect(Vec2f(), float(stSize.x), float(stSize.y));
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
