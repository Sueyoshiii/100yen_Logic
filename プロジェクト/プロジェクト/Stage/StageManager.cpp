#include "StageManager.h"

namespace {
	const float GR = 1.9f;
	const float GROUND = 1200.0f;
}

StageManager::StageManager() :
	clearFlag(false)
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
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left(), range.Right()*2);
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

void StageManager::ResetRange()
{
	auto stSize = stage->GetStageSize();
	range = StageManager::Rect(Vec2f(), float(stSize.x), float(stSize.y));
}

void StageManager::SetNextRoomFlag(const bool flag)
{
	stage->SetNextRoomFlag(flag);
}

int StageManager::GetMapChip(const Vec2f& pos)
{
	return stage->GetMapChip(pos);
}

float StageManager::GetGround() const
{
	return GROUND;
}

float StageManager::GetGravity() const
{
	return GR;
}

Vec2 StageManager::GetChipSize() const
{
	return stage->GetChipSize();
}

Vec2 StageManager::GetStageSize() const
{
	auto tmp = stage->GetStageSize();
	return Vec2(tmp.x, tmp.y);
}

bool StageManager::GetClearFlag() const
{
	return clearFlag;
}

void StageManager::SetClearFlag(const bool flag)
{
	clearFlag = flag;
}
